#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define LINES_PER_PAGE 20
#define BUFFER_SIZE 1024

int lines = 0;

void show_file(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
    	perror("Cannot open file");
    	return;
    }

    char buffer[BUFFER_SIZE];
    int bytesRead;

    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
        for (int i = 0; i < bytesRead; i++) {
            write(1, &buffer[i], 1);
            if (buffer[i] == '\n') {
                lines++;
                if (lines >= LINES_PER_PAGE) {
                    char *msg = "\n--More-- (press Enter to continue)\n";
                    write(1, msg, strlen(msg));
                    char c;
                    read(0, &c, 1);
                    lines = 0;
                }
            }
        }
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
    	char *err = "Usage: ./more <files>\n";
        write(2, err, strlen(err));
        return 1;
    }
    for (int i = 1; i < argc; i++) {
        show_file(argv[i]);
    }
    return 0;
}

