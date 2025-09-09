#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORTNO 10200

int main() {
    int sock_fd;
    struct sockaddr_in addr;
    char buffer[256];

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(PORTNO);

    if (connect(sock_fd, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    read(sock_fd, buffer, sizeof(buffer));
    printf("Received from server:\n%s\n", buffer);

    close(sock_fd);
    return 0;
}
