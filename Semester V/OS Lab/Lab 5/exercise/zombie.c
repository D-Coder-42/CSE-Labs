#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        printf("Child: PID = %d exiting...\n", getpid());
        exit(0);
    } else {
        printf("Parent: PID = %d sleeping...\n", getpid());
        sleep(30);
        printf("Parent: Done sleeping\n");
    }

    return 0;
}

