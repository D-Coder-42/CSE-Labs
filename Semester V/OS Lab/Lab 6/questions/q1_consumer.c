#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_NAME "/tmp/fifoqueue"

int main() {
    int pipe_fd;
    int numbers[4];

    // Open FIFO for reading
    pipe_fd = open(FIFO_NAME, O_RDONLY);
    if (pipe_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Read 4 integers from FIFO
    if (read(pipe_fd, numbers, sizeof(numbers)) == -1) {
        perror("read");
        close(pipe_fd);
        exit(EXIT_FAILURE);
    }

    printf("Consumer: Received integers:\n");
    for (int i = 0; i < 4; i++) {
        printf("  %d\n", numbers[i]);
    }

    close(pipe_fd);
    return 0;
}
