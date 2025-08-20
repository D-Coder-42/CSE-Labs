#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int status;
    pid_t pid;

    pid = fork();

    if (pid == -1) {
        printf("\nERROR: child not created");
    } else if (pid == 0) {  // Child process
        printf("I'm the child!\n");
        exit(0);
    } else {  // Parent process
        wait(&status);
        printf("I'm the parent!\n");
        printf("Child returned: %d\n", status);
    }

    return 0;
}

