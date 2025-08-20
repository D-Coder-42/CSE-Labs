#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        printf("Child: PID = %d\n", getpid());
        sleep(2);
        printf("Child: Done\n");
        exit(0);
    } else {
        printf("Parent: Waiting for child to complete...\n");
        wait(NULL);
        printf("Parent: Child completed\n");
    }

    return 0;
}

