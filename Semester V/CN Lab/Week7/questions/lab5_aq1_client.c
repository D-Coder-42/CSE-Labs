#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORTNO 10200
#define ENCRYT_KEY 3

int main() {
    int sock_fd;
    struct sockaddr_in addr;
    char input[256], encrypted[256];
    int i;

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("172.16.48.161");
    addr.sin_port = htons(PORTNO);

    connect(sock_fd, (struct sockaddr*) &addr, sizeof(addr));

    printf("Enter message to encrypt: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    for (i = 0; input[i] != '\0'; i++) {
        encrypted[i] = input[i] + ENCRYT_KEY;
    }
    encrypted[i] = '\0';

    write(sock_fd, encrypted, strlen(encrypted) + 1);

    close(sock_fd);
    return 0;
}
