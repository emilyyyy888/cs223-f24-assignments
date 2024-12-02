/*----------------------------------------------
* Author: Emily Lu
* Date: 12/1/2024
* Description: Implemented a program that uses N threads to search for a keyword in a set of files.
---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

#define MAX_LINE_LENGTH 1024

typedef struct {
    int thread_id;
    int num_threads;
    char **files;
    int num_files;
    char *keyword;
    int *results;
    pthread_mutex_t *mutex;
} ThreadData;

void *search_keyword(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int count = 0;

    printf("Thread %d is searching %d files\n", data->thread_id, 
           (data->num_files + data->num_threads - 1 - data->thread_id) / data->num_threads);

    for (int i = data->thread_id; i < data->num_files; i += data->num_threads) {
        FILE *file = fopen(data->files[i], "r");
        if (!file) {
            fprintf(stderr, "Thread %d: Unable to open file %s\n", data->thread_id, data->files[i]);
            continue;
        }

        char line[MAX_LINE_LENGTH];
        while (fgets(line, MAX_LINE_LENGTH, file)) {
            if (strstr(line, data->keyword)) {
                count++;
                pthread_mutex_lock(data->mutex);
                printf("Thread %d: %s: %s", data->thread_id, data->files[i], line);
                pthread_mutex_unlock(data->mutex);
            }
        }

        fclose(file);
    }

    data->results[data->thread_id] = count;
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "usage: ./grep <NumThreads> <Keyword> <Files>\n");
        return 1;
    }

    int num_threads = atoi(argv[1]);
    char *keyword = argv[2];
    char **files = &argv[3];
    int num_files = argc - 3;

    pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
    ThreadData *thread_data = malloc(num_threads * sizeof(ThreadData));
    int *results = malloc(num_threads * sizeof(int));
    pthread_mutex_t mutex;

    if (!threads || !thread_data || !results) {
        fprintf(stderr, "Error: Failed to allocate memory.\n");
        free(threads);
        free(thread_data);
        free(results);
        return 1;
    }

    printf("Searching %d files with %d threads for keyword: '%s'\n", num_files, num_threads, keyword);

    pthread_mutex_init(&mutex, NULL);

    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);

    for (int i = 0; i < num_threads; i++) {
        thread_data[i].thread_id = i;
        thread_data[i].num_threads = num_threads;
        thread_data[i].files = files;
        thread_data[i].num_files = num_files;
        thread_data[i].keyword = keyword;
        thread_data[i].results = results;
        thread_data[i].mutex = &mutex;
        pthread_create(&threads[i], NULL, search_keyword, &thread_data[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    gettimeofday(&end_time, NULL);
    double elapsed = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1e6;
    printf("Elapsed time: %.6f seconds\n", elapsed);

    int total_count = 0;
    for (int i = 0; i < num_threads; i++) {
        total_count += results[i];
        printf("Thread %d found %d lines containing keyword: %s\n", i, results[i], keyword);
    }


    printf("Total lines containing keyword: %d\n", total_count);

    free(threads);
    free(thread_data);
    free(results);

    return 0;
}
