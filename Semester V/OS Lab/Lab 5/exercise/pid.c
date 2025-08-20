#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        printf("Child: PID = %d, Parent PID = %d\n", getpid(), getppid());
    } else {
        printf("Parent: PID = %d, Child PID = %d\n", getpid(), pid);
        wait(NULL);
    }

    return 0;
}

