#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>

#define FIFO_PC "fifo_pc"
#define FIFO_CP "fifo_cp"
#define MAX_ITEMS 5

void ensure_fifo(const char *path) {
    if (access(path, F_OK) == -1 && mkfifo(path, 0666) == -1) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }
}

int random_sleep_ms() {
    return (rand() % 1500) + 500; // 500ms to 2000ms
}

int main() {
    srand(time(NULL));
    ensure_fifo(FIFO_PC);
    ensure_fifo(FIFO_CP);

    int shelf_count = 0;
    int fd_write = open(FIFO_PC, O_WRONLY);
    int fd_read  = open(FIFO_CP, O_RDONLY | O_NONBLOCK);

    if (fd_write == -1 || fd_read == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    while (1) {
        if (shelf_count < MAX_ITEMS) {
            shelf_count++;
            printf("Producer: Added item. Shelf count = %d\n", shelf_count);
        } else {
            printf("Producer: Shelf full. Waiting...\n");
        }

        write(fd_write, &shelf_count, sizeof(int));

        int updated_count;
        ssize_t bytes = read(fd_read, &updated_count, sizeof(int));
        if (bytes > 0) {
            shelf_count = updated_count;
            printf("Producer: Synced shelf count from consumer = %d\n", shelf_count);
        }

        usleep(random_sleep_ms() * 1000);
    }

    close(fd_write);
    close(fd_read);
    return 0;
}
