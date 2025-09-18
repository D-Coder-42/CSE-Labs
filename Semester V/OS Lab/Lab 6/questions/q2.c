#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    int fd[2];					// fd[0] for reading, fd[1] for writing
    char buf[1024];
	char data[1024];
	
    // Create the pipe
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
	
	printf("Enter message to pipe: ");
	fgets(data, 1024, stdin);
	
    // Write data to the pipe
    ssize_t bytes_written = write(fd[1], data, strlen(data));
    if (bytes_written == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    // Read data from the pipe
    ssize_t bytes_read = read(fd[0], buf, 1024);
    if (bytes_read >= 0) {
        buf[bytes_read - 1] = '\0';
        printf("Read %ld bytes from the pipe: \"%s\"\n", bytes_read, buf);
    } else {
        perror("read");
        exit(EXIT_FAILURE);
    }

    return 0;
}
