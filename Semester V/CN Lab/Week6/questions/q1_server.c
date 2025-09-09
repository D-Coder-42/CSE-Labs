#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORTNO 10200

int main() {
    int sock_fd, new_sock_fd, cli_len;
    struct sockaddr_in ser_addr, cli_addr;

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = inet_addr("172.16.48.161");
    ser_addr.sin_port = htons(PORTNO);
    bind(sock_fd, (struct sockaddr*) &ser_addr, sizeof(ser_addr));

    listen(sock_fd, 5);
    printf("Math Server running...\n");

    while (1) {
        cli_len = sizeof(cli_addr);
        new_sock_fd = accept(sock_fd, (struct sockaddr*) &cli_addr, &cli_len);

        if (fork() == 0) {
            close(sock_fd); // Child doesn't need listening socket

            char buf[256];
            int n = read(new_sock_fd, buf, sizeof(buf));
            buf[n] = '\0';

            int num1, num2;
            char operator;
            float result;
            char response[256];

            sscanf(buf, "%d %c %d", &num1, &operator, &num2);

            switch (operator) {
                case '+': result = num1 + num2; break;
                case '-': result = num1 - num2; break;
                case '*': result = num1 * num2; break;
                case '/':
                    if (num2 == 0) {
                        strcpy(response, "Error: Division by zero");
                        write(new_sock_fd, response, strlen(response) + 1);
                        close(new_sock_fd);
                        exit(0);
                    }
                    result = (float)num1 / num2;
                    break;
                case '%':
                    if (num2 == 0) {
                        strcpy(response, "Error: Division by zero");
                        write(new_sock_fd, response, strlen(response) + 1);
                        close(new_sock_fd);
                        exit(0);
                    }
                    result = num1 % num2;
                    break;
                default:
                    strcpy(response, "Error: Invalid operator");
                    write(new_sock_fd, response, strlen(response) + 1);
                    close(new_sock_fd);
                    exit(0);
            }

            sprintf(response, "Result: %.2f", result);
            write(new_sock_fd, response, strlen(response) + 1);

            close(new_sock_fd);
            exit(0);
        } else {
            close(new_sock_fd); // Parent doesn't need connected socket
        }
    }

    return 0;
}
