/*----------------------------------------------
 * Author: Emily Lu
 * Date: Nov 1 2024
 * Description: implement a simple shell.
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

// Color codes for prompt and error messages
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// Function to print a basic prompt with the current directory
void print_prompt() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf(ANSI_COLOR_GREEN "myshell:%s$ " ANSI_COLOR_RESET, cwd);
    } else {
        perror("getcwd() error");
    }
}

// Function to execute a command or handle built-ins
void execute_command(char *line) {
    // Tokenize the input
    char *args[100];
    int i = 0;
    args[i] = strtok(line, " ");
    while (args[i] != NULL) {
        i++;
        args[i] = strtok(NULL, " ");
    }

    if (args[0] == NULL) {
        return;
    } else if (strcmp(args[0], "exit") == 0) {
        exit(0);
    } else if (strcmp(args[0], "cd") == 0) {  
        if (args[1] == NULL) {
            fprintf(stderr, "myshell: expected argument to \"cd\"\n");
        } else if (chdir(args[1]) != 0) {
            perror("myshell");
        }
        return;
    }

    // Fork a child process to execute other commands
    pid_t pid = fork();
    int status;
    if (pid == 0) {  // Child process
        if (execvp(args[0], args) == -1) {
            printf(ANSI_COLOR_RED "myshell: command not found: %s\n" ANSI_COLOR_RESET, args[0]);
            exit(1);
        }
    } else if (pid < 0) {  // Error forking
        perror("fork failed");
    } else {  // Parent process
        waitpid(pid, &status, 0);
    }
}

int main() {
    char *line;

    while (1) {
        // Print the prompt
        print_prompt();

        // Read input using readline
        line = readline("");
        if (line == NULL || strcmp(line, "exit") == 0) {  
            free(line);
            break;
        }

        // Add to history if the input is not empty
        if (strlen(line) > 0) {
            add_history(line);
            execute_command(line);
        }

        free(line);
    }

    printf("Exiting myshell. Goodbye!\n");
    return 0;
}
