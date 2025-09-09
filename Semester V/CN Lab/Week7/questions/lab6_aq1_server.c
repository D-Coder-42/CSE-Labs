#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORTNO 10200

int main() {
    int sock_fd, new_sock_fd, cli_len;
    struct sockaddr_in ser_addr, cli_addr;

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = INADDR_ANY;
    ser_addr.sin_port = htons(PORTNO);

    if (bind(sock_fd, (struct sockaddr*) &ser_addr, sizeof(ser_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    listen(sock_fd, 5);
    printf("Daytime Server running...\n");

    while (1) {
        cli_len = sizeof(cli_addr);
        new_sock_fd = accept(sock_fd, (struct sockaddr*) &cli_addr, &cli_len);
        if (new_sock_fd < 0) {
            perror("Accept failed");
            continue;
        }

        if (fork() == 0) {
            close(sock_fd);

            time_t now = time(NULL);
            char *time_str = ctime(&now);
            char response[256];

            time_str[strcspn(time_str, "\n")] = '\0';

            sprintf(response, "Current Time: %s | Process ID: %d", time_str, getpid());

            write(new_sock_fd, response, strlen(response) + 1);

            close(new_sock_fd);
            exit(0);
        } else {
            close(new_sock_fd);
        }
    }

    return 0;
}