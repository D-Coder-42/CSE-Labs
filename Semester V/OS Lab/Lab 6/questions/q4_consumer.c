#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>

#define FIFO_PC "fifo_pc"
#define FIFO_CP "fifo_cp"

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
    srand(time(NULL) + 1); // offset seed
    ensure_fifo(FIFO_PC);
    ensure_fifo(FIFO_CP);

    int shelf_count = 5;
    int fd_read  = open(FIFO_PC, O_RDONLY | O_NONBLOCK);
    int fd_write = open(FIFO_CP, O_WRONLY);

    if (fd_read == -1 || fd_write == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    while (1) {
        int updated_count;
        ssize_t bytes = read(fd_read, &updated_count, sizeof(int));
        if (bytes > 0) {
            shelf_count = updated_count;
            printf("Consumer: Received shelf count = %d\n", shelf_count);
        }

        if (shelf_count > 0) {
            shelf_count--;
            printf("Consumer: Removed item. Shelf count = %d\n", shelf_count);
        } else {
            printf("Consumer: Shelf empty. Waiting...\n");
        }

        write(fd_write, &shelf_count, sizeof(int));
        usleep(random_sleep_ms() * 1000);
    }

    close(fd_read);
    close(fd_write);
    return 0;
}
