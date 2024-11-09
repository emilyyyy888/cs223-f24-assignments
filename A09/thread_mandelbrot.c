/*----------------------------------------------
* Author: Emily Lu
* Date: 11/8/2024
* Description: Implement a multi-threaded version of the mandelbrot set that I implemented for Assignment 09.
---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include "read_ppm.h"
#include "write_ppm.h"

// Struct to hold data for each thread
struct thread_data{
  int start_row, end_row;
  int start_col, end_col;
  int size;
  float xmin, xmax, ymin, ymax;
  int maxIterations;
  struct ppm_pixel* pixels;
  struct ppm_pixel* palette;
  pthread_t* tid;
};

// thread main function executed by each thread
void* mandelbrot_thread(void* arg){
  struct thread_data* data = (struct thread_data*)arg;
  int size = data->size;
  float xmin = data->xmin;
  float xmax = data->xmax;
  float ymin = data->ymin;
  float ymax = data->ymax;
  int maxIterations = data->maxIterations;
  struct ppm_pixel* pixels = data->pixels;
  struct ppm_pixel* palette = data->palette;

  printf("Thread %lu) sub-image block: cols (%d, %d) to rows (%d, %d)\n",
    *data->tid, data->start_col, data->end_col, data->start_row, data->end_row);

  for (int row = data->start_row; row < data->end_row; row++) {
    for (int col = data->start_col; col < data->end_col; col++) {
            float xfrac = (float)col / size;
            float yfrac = (float)row / size;
            float x0 = xmin + xfrac * (xmax - xmin);
            float y0 = ymin + yfrac * (ymax - ymin);
            float x = 0.0;
            float y = 0.0;
            int iter = 0;

            while (x * x + y * y < 4.0 && iter < maxIterations) {
                float xtmp = x * x - y * y + x0;
                y = 2 * x * y + y0;
                x = xtmp;
                iter++;
            }

            int index = row * size + col;
            if (iter < maxIterations) {
                pixels[index] = palette[iter];
            } else {
                pixels[index].red = 0;
                pixels[index].green = 0;
                pixels[index].blue = 0;
            }
        }
  }
  printf("Thread %lu) finished\n", *data->tid);
  return NULL;
}

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numProcesses = 4;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> "
        "-b <ymin> -t <ymax> -p <numProcesses>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your code here
  // generate pallet
  struct ppm_pixel* pixels = malloc(size * size * sizeof(struct ppm_pixel));
  if (!pixels) {
      printf("Error: Cannot allocate memory for pixels.\n");
      return 1;
  }

  struct ppm_pixel* palette = malloc(maxIterations * sizeof(struct ppm_pixel));
  if (!palette) {
      printf("Error: Cannot allocate memory for palette.\n");
      free(pixels);
      return 1;
  }

  srand(time(0));
  for (int i = 0; i < maxIterations; i++) {
      palette[i].red = rand() % 255;
      palette[i].green = rand() % 255;
      palette[i].blue = rand() % 255;
  }

  struct timeval tstart, tend; 
  gettimeofday(&tstart, NULL);

  pthread_t* threads = malloc(sizeof(pthread_t) * numProcesses);
  struct thread_data* thread_data = malloc(numProcesses * sizeof(struct thread_data));  
  int rows = size / 2;
  int cols = size / 2;

  for(int i = 0; i < numProcesses; i++) {
      thread_data[i].size = size;
      thread_data[i].xmin = xmin;
      thread_data[i].xmax = xmax;
      thread_data[i].ymin = ymin;
      thread_data[i].ymax = ymax;
      thread_data[i].maxIterations = maxIterations;
      thread_data[i].pixels = pixels;
      thread_data[i].palette = palette;
      thread_data[i].tid = &threads[i];
      
    // Divide image into quadrants for each thread
    if (i == 0) { // Top left quadrant
      thread_data[i].start_row = 0;
      thread_data[i].end_row = rows;
      thread_data[i].start_col = 0;
      thread_data[i].end_col = cols;
    } else if (i == 1) { //top right quadrant
      thread_data[i].start_row = 0;
      thread_data[i].end_row = rows;
      thread_data[i].start_col = cols;
      thread_data[i].end_col = size;
    } else if (i == 2) { // Bottom left
      thread_data[i].start_row = rows;
      thread_data[i].end_row = size;
      thread_data[i].start_col = 0;
      thread_data[i].end_col = cols;
    } else { // Bottom right
      thread_data[i].start_row = rows;
      thread_data[i].end_row = size;
      thread_data[i].start_col = cols;
      thread_data[i].end_col = size;
    }

    pthread_create(&threads[i], NULL, mandelbrot_thread, &thread_data[i]);
  }

  for(int i = 0; i <numProcesses; i++) {
    pthread_join(threads[i], NULL);
  }

  // compute the time 
  gettimeofday(&tend, NULL);
  double timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Computed mandelbrot set (%dx%d) in %f seconds\n", size, size, timer);

  // writing output file 
  char filename[256];
  snprintf(filename, sizeof(filename), "mandelbrot-%d-%ld.ppm", size, time(0));
  printf("Writing file: %s\n", filename);
  write_ppm(filename, pixels, size, size);

  free(pixels);
  free(palette);
  free(threads);
  free(thread_data);

  return 0;

}