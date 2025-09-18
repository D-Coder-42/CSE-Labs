#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_NAME "/tmp/fifoqueue"

int main() {
    int pipe_fd;
    int numbers[4];
    
    for (int i = 0; i < 4; i++) {
		printf("Enter number %d: ", i+1);
		scanf("%d", &numbers[i]);
	}

    // Create FIFO if it doesn't exist
    if (access(FIFO_NAME, F_OK) == -1) {
        if (mkfifo(FIFO_NAME, 0666) != 0) {
            perror("mkfifo");
            exit(EXIT_FAILURE);
        }
    }

    // Open FIFO for writing
    pipe_fd = open(FIFO_NAME, O_WRONLY);
    if (pipe_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Write 4 integers to FIFO
    if (write(pipe_fd, numbers, sizeof(numbers)) == -1) {
        perror("write");
        close(pipe_fd);
        exit(EXIT_FAILURE);
    }

    printf("Producer: Sent 4 integers to FIFO.\n");
    close(pipe_fd);
    return 0;
}
