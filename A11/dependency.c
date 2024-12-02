/*----------------------------------------------
* Author: Emily Lu
* Date: 12/1/2024
* Description: Implemented a program that uses N threads to build a binary search tree of a given set of files.
---------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include "tree.h"

struct tree_node* root = NULL;
pthread_mutex_t mutex;

struct thread_info {
    int thread_id;
    char** file_list;
    int start_idx;
    int end_idx;
};

void* process_files(void* arg) {
    struct thread_info* data = (struct thread_info*)arg;

    for (int i = data->start_idx; i <= data->end_idx; i++) {
        FILE* file = fopen(data->file_list[i], "r");
        if (!file) {
            printf("Invalid file: %s\n", data->file_list[i]);
            continue;
        }

        pthread_mutex_lock(&mutex);
        root = insert(data->file_list[i], root);
        pthread_mutex_unlock(&mutex);

        fclose(file);
    }
    return NULL;
}

void print_dependencies(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char* include_pos = strstr(line, "#include");
        if (include_pos) {
            char dependency[256] = {0};
            int idx = 0;
            char* start = strchr(include_pos, '<') ? strchr(include_pos, '<') + 1 :
                          strchr(include_pos, '"') ? strchr(include_pos, '"') + 1 : NULL;

            while (start && start[idx] != '\0' && start[idx] != '>' && start[idx] != '"') {
                dependency[idx] = start[idx];
                idx++;
            }
            dependency[idx] = '\0';
            if (strlen(dependency) > 0) {
                printf("  %s\n", dependency);
            }
        }
    }
    fclose(file);
}

int main(int argc, char* argv[]) {
    struct timeval tstart, tend;
    gettimeofday(&tstart, NULL);

    if (argc < 3) {
        fprintf(stderr, "Usage: ./dependency <NumThreads> <Files>\n");
        return EXIT_FAILURE;
    }

    int total_files = argc - 2;
    printf("Processing %d files\n", total_files);
    char** files = malloc(total_files * sizeof(char*));
    if (!files) {
        perror("Memory allocation error");
        return 1;
    }

    for (int i = 0; i < total_files; i++) {
        files[i] = argv[i + 2];
    }

    int num_threads = atoi(argv[1]);
    int files_per_thread = total_files / num_threads;
    int remaining_files = total_files % num_threads;

    pthread_t* threads = malloc(num_threads * sizeof(pthread_t));
    struct thread_info* thread_data = malloc(num_threads * sizeof(struct thread_info));
    if (!threads || !thread_data) {
        perror("Memory allocation error");
        free(files);
        free(threads);
        free(thread_data);
        return 1;
    }

    pthread_mutex_init(&mutex, NULL);

    int current_idx = 0;
    for (int i = 0; i < num_threads; i++) {
        thread_data[i].thread_id = i;
        thread_data[i].file_list = files;
        thread_data[i].start_idx = current_idx;
        thread_data[i].end_idx = current_idx + files_per_thread - 1 + (i < remaining_files ? 1 : 0);
        current_idx = thread_data[i].end_idx + 1;

        printf("Thread %d processing files %d to %d\n", i, thread_data[i].start_idx + 2, thread_data[i].end_idx + 2);
        pthread_create(&threads[i], NULL, process_files, &thread_data[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&tend, NULL);
    double elapsed_time = (tend.tv_sec - tstart.tv_sec) + (tend.tv_usec - tstart.tv_usec) / 1e6;
    printf("Elapsed time is %.6f seconds\n", elapsed_time);

    char command[256];
    while (1) {
        printf("$ ");
        if (!fgets(command, sizeof(command), stdin)) break;

        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "list") == 0) {
            printSorted(root);
        } else if (strcmp(command, "quit") == 0) {
            exit(0);
        } else {
            struct tree_node* node = find(command, root);
            if (node) {
                printf("%s has the following dependencies:\n", command);
                print_dependencies(command);
            } else {
                printf("%s not found\n", command);
            }
        }
    }

    free(files);
    free(threads);
    free(thread_data);
    clear(root);
    pthread_mutex_destroy(&mutex);

    return 0;
}
