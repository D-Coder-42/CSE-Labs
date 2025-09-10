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
        if (data->written == 1) {
            printf("You wrote: %s", data->payload);
            data->written = 0;

            if (strncmp(data->payload, "end", 3) == 0) {
                running = 0;
            }
        } else {
            sleep(1);
        }
    }

    shmdt(shared_mem);
    shmctl(shmid, IPC_RMID, 0);
    exit(EXIT_SUCCESS);
}
