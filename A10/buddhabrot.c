/*----------------------------------------------
* Author: Emily Lu
* Date: 11/15/2024
* Description: compute a program that outputs a PPM image of the Buddhabrot using multiple threads.
---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include "read_ppm.h"
#include "write_ppm.h"

// Global variables
int *member;
int *count_array;
int size;
float xmin, xmax, ymin, ymax;
int max_count = 0;
pthread_mutex_t count_mutex;
pthread_barrier_t barrier;
struct ppm_pixel *pixels;

typedef struct {
    int start_row;
    int end_row;
    int start_col;
    int end_col;
} thread_data_t;

void *start(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    int maxIter = 1000;

    // Print sub-image block information
    printf("Thread %lu) sub-image block: cols (%d, %d) to rows (%d,%d)\n",
           pthread_self(), data->start_col, data->end_col, data->start_row, data->end_row);

    // Step 1: Determine Mandelbrot set membership
    for (int row = data->start_row; row < data->end_row; row++) {
        for (int col = data->start_col; col < data->end_col; col++) {
            float xfrac = (float)col / size;
            float yfrac = (float)row / size;
            float x0 = xmin + xfrac * (xmax - xmin);
            float y0 = ymin + yfrac * (ymax - ymin);
            float x = 0, y = 0;
            int iter = 0;
            while (iter < maxIter && (x * x + y * y) < 4.0) {
                float xtmp = x * x - y * y + x0;
                y = 2 * x * y + y0;
                x = xtmp;
                iter++;
            }
            // Set membership based on whether the point escaped
            if (iter < maxIter) { // Escaped
                member[row * size + col] = 0;
            } else {
                member[row * size + col] = 1;
            }
        }
    }

    // Step 2: Compute visited counts
    for (int row = data->start_row; row < data->end_row; row++) {
        for (int col = data->start_col; col < data->end_col; col++) {
            if (member[row * size + col]) continue; // If (row, col) belongs to the Mandelbrot set, continue

            float xfrac = (float)col / size;
            float yfrac = (float)row / size;
            float x0 = xmin + xfrac * (xmax - xmin);
            float y0 = ymin + yfrac * (ymax - ymin);
            float x = 0, y = 0;

            while ((x * x + y * y) < 4.0) {
                float xtmp = x * x - y * y + x0;
                y = 2 * x * y + y0;
                x = xtmp;

                int yrow = (int)round(size * (y - ymin) / (ymax - ymin));
                int xcol = (int)round(size * (x - xmin) / (xmax - xmin));
                if (yrow < 0 || yrow >= size || xcol < 0 || xcol >= size) continue;

                pthread_mutex_lock(&count_mutex);
                count_array[yrow * size + xcol]++;
                if (count_array[yrow * size + xcol] > max_count) {
                    max_count = count_array[yrow * size + xcol];
                }
                pthread_mutex_unlock(&count_mutex);
            }
        }
    }

    // Barrier to wait for all threads before computing colors
    pthread_barrier_wait(&barrier);

    // Step 3: Compute colors
    float gamma = 0.681;
    float factor = 1.0 / gamma;
    for (int row = data->start_row; row < data->end_row; row++) {
        for (int col = data->start_col; col < data->end_col; col++) {
            float value = 0;
            if (count_array[row * size + col] > 0) {
                value = log(count_array[row * size + col]) / log(max_count);
                value = pow(value, factor);
            }
            int color_value = (int)(value * 255);
            pixels[row * size + col].red = color_value;
            pixels[row * size + col].green = color_value;
            pixels[row * size + col].blue = color_value;
        }
    }
    printf("Thread %lu) finished\n", pthread_self());
    return NULL;
}

int main(int argc, char *argv[]) {
    // Default values
    size = 480;
    xmin = -2.0; 
    xmax = 0.47;  
    ymin = -1.12;
    ymax = 1.12;
    int numProcesses = 4;

    // Command-line arguments parsing
    int opt;
    while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
        switch (opt) {
            case 's': size = atoi(optarg); break;
            case 'l': xmin = atof(optarg); break;
            case 'r': xmax = atof(optarg); break;
            case 't': ymax = atof(optarg); break;
            case 'b': ymin = atof(optarg); break;
            case 'p': numProcesses = atoi(optarg); break;
            default:
                fprintf(stderr, "Usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax> -p <num_threads>\n", argv[0]);
                return 1;
        }
    }

    printf("Generating buddhabrot with size %dx%d\n", size, size);
    printf("  Num processes = %d\n", numProcesses);
    printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
    printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

    // Allocate memory
    member = malloc(size * size * sizeof(int));
    count_array = malloc(size * size * sizeof(int));
    pixels = malloc(size * size * sizeof(struct ppm_pixel));
    if (!member || !count_array || !pixels) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }
    for (int i = 0; i < size * size; i++) {
        count_array[i] = 0;
    }

    // Initialize mutex and barrier
    pthread_mutex_init(&count_mutex, NULL);
    pthread_barrier_init(&barrier, NULL, numProcesses);

    struct timeval tstart, tend;
    gettimeofday(&tstart, NULL);

    // Start threads
    pthread_t *threads = malloc(numProcesses * sizeof(pthread_t));
    thread_data_t *thread_data = malloc(numProcesses * sizeof(thread_data_t));
    int rows_per_thread = size / 2;
    int cols_per_thread = size / 2;
    for (int i = 0; i < numProcesses; i++) {
        int row_block = (i / 2) * rows_per_thread;
        int col_block = (i % 2) * cols_per_thread;
        thread_data[i].start_row = row_block;
        thread_data[i].end_row = row_block + rows_per_thread;
        thread_data[i].start_col = col_block;
        thread_data[i].end_col = col_block + cols_per_thread;
        pthread_create(&threads[i], NULL, start, &thread_data[i]);
    }

    // Join threads
    for (int i = 0; i < numProcesses; i++) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&tend, NULL);
    double timer = (tend.tv_sec - tstart.tv_sec) + ((tend.tv_usec - tstart.tv_usec) / 1.e6);
    printf("Computed buddhabrot set (%dx%d) in %f seconds\n", size, size, timer);

    // Write output
    char output_filename[256];
    snprintf(output_filename, sizeof(output_filename), "buddhabrot-%d-%ld.ppm", size, time(NULL));
    write_ppm(output_filename, pixels, size, size);
    printf("Writing file: %s\n", output_filename);

    // Cleanup
    free(member);
    free(count_array);
    free(pixels);
    pthread_mutex_destroy(&count_mutex);
    pthread_barrier_destroy(&barrier);
    free(threads);
    free(thread_data);

    return 0;
}
