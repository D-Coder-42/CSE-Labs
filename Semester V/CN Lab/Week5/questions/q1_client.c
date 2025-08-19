#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORTNO 10200

int main() {
    int sock_fd, n = 1;
    struct sockaddr_in addr;
    char input[256], buf[256];

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("172.16.48.161");
    addr.sin_port = htons(PORTNO);
    
    connect(sock_fd, (struct sockaddr*) &addr, sizeof(addr));
    
    while(1) {
        printf("Enter string:\t");
        gets(input);
        input[strlen(input)] = '\0';

        write(sock_fd, input, strlen(input) + 1);

        if (strcmp(input, "stop") == 0) {
            break;
        }

        n = read(sock_fd, buf, sizeof(buf));
        printf("Processed string from server: ");
        puts(buf);
    }

    return 0;
}
