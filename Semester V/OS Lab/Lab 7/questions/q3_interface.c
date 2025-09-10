#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shm_com.h"

int main() {
    int running = 1;
    void* shared_mem = NULL;
    shared* data;
    char buffer[TEXT_SZ];
    int shmid;

    shmid = shmget((key_t)5678, sizeof(shared), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }

    shared_mem = shmat(shmid, NULL, 0);
    if (shared_mem == (void*)-1) {
        perror("shmat failed");
        exit(EXIT_FAILURE);
    }

    data = (shared*)shared_mem;
    data->written = 0;

    while (running) {
        while (data->written == 1) {
            sleep(1);
        }

        printf("Enter your message: ");
        fgets(buffer, TEXT_SZ, stdin);
        strncpy(data->payload, buffer, TEXT_SZ);
        data->written = 1;

        if (strncmp(buffer, "end", 3) == 0) {
            running = 0;
        }
    }

    shmdt(shared_mem);
    exit(EXIT_SUCCESS);
}
