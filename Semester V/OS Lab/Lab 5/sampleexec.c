#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid;

    // Fork another process
    pid = fork();

    if (pid < 0) { // Error occurred
        fprintf(stderr, "Fork Failed\n");
        exit(-1);
    } else if (pid == 0) { // Child process
        execlp("/bin/ls", "ls", NULL);
        // If execlp fails
        perror("execlp failed");
        exit(1);
    } else { // Parent process
        wait(NULL); // Wait for child to complete
        printf("Child Complete\n");
        exit(0);
    }
}

