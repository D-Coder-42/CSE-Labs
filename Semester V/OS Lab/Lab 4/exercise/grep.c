#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
    	char *err = "Usage: ./grep <filename> <word>\n";
        write(2, err, strlen(err));
        exit(EXIT_FAILURE);
    }

    char *word = argv[2];
    char buffer[BUFFER_SIZE];
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("Cannot open source file");
        exit(EXIT_FAILURE);
    }

    int bytesRead;
    char line[BUFFER_SIZE];
    int idx = 0;

    while ((bytesRead = read(fd, buffer, BUFFER_SIZE)) > 0) {
        for (int i = 0; i < bytesRead; i++) {
            line[idx++] = buffer[i];
            if (buffer[i] == '\n' || idx == BUFFER_SIZE - 1) {
                line[idx] = '\0';
                if (strstr(line, word) != NULL) {
                    write(1, line, strlen(line));
                }
                idx = 0;
            }
        }
    }
    close(fd);
    return 0;
}

