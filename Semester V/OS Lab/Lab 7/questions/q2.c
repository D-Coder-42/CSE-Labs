#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

typedef struct {
    int written;
    char payload;
} shared;

int main() {
    int running = 1;
    void* shared_mem = NULL;
    shared* data;
    char ch;
    int shmid;

    shmid = shmget((key_t)1234, sizeof(shared), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget() failed");
        exit(EXIT_FAILURE);
    }

    shared_mem = shmat(shmid, NULL, 0);
    if (shared_mem == (void*)-1) {
        perror("shmat() failed");
        exit(EXIT_FAILURE);
    }

    printf("Memory attached at %p\n", shared_mem);
    data = (shared*)shared_mem;
    data->written = 0;

    while (running) {
        printf("Enter a character: ");
        scanf(" %c", &ch);

        data->payload = ch;
        data->written = 1;

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork() failed");
            break;
        }

        if (pid == 0) {
            if (data->written) {
                data->payload += 1;
                data->written = 0;
            }
            exit(EXIT_SUCCESS);
        } else {
            while (data->written == 1) {
                sleep(1);
            }

            wait(NULL);
            printf("Next character: %c\n", data->payload);

            if (ch == '0') {
                running = 0;
            }
        }
    }

    if (shmdt(shared_mem) == -1) {
        perror("shmdt() failed");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
