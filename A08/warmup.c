/*----------------------------------------------
 * Author: Emily Lu
 * Date: Nov 1 2024
 * Description: implement a program that spawns processes according to the following diagram.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    printf("%d] A0\n", getpid());

    pid = fork();
    if (pid == 0) { // Child process B1
        printf("%d] B1\n", getpid());
        printf("%d] Bye\n", getpid());
    } else { // Parent process A
        printf("%d] B0\n", getpid());

        // Wait for B1 to finish
        wait(NULL);

        pid = fork();
        if (pid == 0) { // Child process C1
            printf("%d] C1\n", getpid());
            printf("%d] Bye\n", getpid());
        } else { // Parent process A (printing C0)
            printf("%d] C0\n", getpid());
            printf("%d] Bye\n", getpid());
        }
    }

    return 0;
}
