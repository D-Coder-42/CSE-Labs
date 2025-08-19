#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORTNO 10200

int main() {
    int sock_fd, new_sock_fd, port_no, cli_len, n = 1;
    struct sockaddr_in ser_addr, cli_addr;
    int i;

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = inet_addr("172.16.48.161");
    ser_addr.sin_port = htons(PORTNO);
    bind(sock_fd, (struct sockaddr*) &ser_addr, sizeof(ser_addr));

    listen(sock_fd, 5);
    
    while(1) {
        printf("Server running...\n");

        cli_len = sizeof(cli_addr);
        new_sock_fd = accept(sock_fd, (struct sockaddr*) &cli_addr, &cli_len);

        while(1) {
            char buf[256], result[256] = "";
            n = read(new_sock_fd, buf, sizeof(buf));
            buf[n] = '\0';

            if (strcmp(buf, "stop") == 0) {
                printf("Server stopped.\n");
                break;
            }

            char words[50][50];
            int count[50] = {0};
            int word_count = 0;

            char *token = strtok(buf, " ");
            while (token != NULL) {
                int found = 0;
                for (i = 0; i < word_count; i++) {
                    if (strcmp(words[i], token) == 0) {
                        count[i]++;
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    strcpy(words[word_count], token);
                    count[word_count]++;
                    word_count++;
                }
                token = strtok(NULL, " ");
            }

            for (i = 0; i < word_count; i++) {
                strcat(result, words[i]);
                strcat(result, " ");
            }

            write(new_sock_fd, result, strlen(result) + 1);
        }

        close(new_sock_fd);
    }

    return 0;
}
