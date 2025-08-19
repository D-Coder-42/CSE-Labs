#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORTNO 9704

int main () {
    int sd;
    struct sockaddr_in addr;
    sd = socket(AF_INET, SOCK_DGRAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("172.16.48.161");
    addr.sin_port = htons(PORTNO);

    char input[256], buf[1024];
    int len = sizeof(addr);

    while (1) {
        printf("Enter row:\t");
        gets(input);
        input[strlen(input)] = '\0';

        sendto(sd, input, sizeof(input), 0, (struct sockaddr*) &addr, len);

        if (strcmp(input, "stop") == 0) {
            break;
        }

        recvfrom(sd, buf, sizeof(buf), 0, (struct sockaddr*) &addr, &len);
        printf("Server reply:\t");
        puts(buf);
    }

    recvfrom(sd, buf, sizeof(buf), 0, (struct sockaddr*) &addr, &len);
    printf("Final matrix from server:\n");
    puts(buf);

    return 0;
}
