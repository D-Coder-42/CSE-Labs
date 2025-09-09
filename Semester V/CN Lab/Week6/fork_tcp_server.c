#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORTNO 10200

// struct sockaddr {
//     unsigned short sa_family;
//     char sa_data[14];
// };

// struct in_addr {
//     unsigned long int s_addr;
// };

// struct sockaddr_in {
//     short int sin_family;
//     struct in_addr sin_addr;
// };

int main() {
    int sock_fd, new_sock_fd, port_no, cli_len, n = 1;
    struct sockaddr_in ser_addr, cli_addr;
    int i, val;

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = inet_addr("172.16.48.161");
    ser_addr.sin_port = htons(PORTNO);
    int status = bind(sock_fd, (struct sockaddr*) &ser_addr, sizeof(ser_addr));

    listen(sock_fd, 5);
    
    while(1) {
        char buf[256];
        printf("Server waiting...\n");

        cli_len = sizeof(cli_addr);
        new_sock_fd = accept(sock_fd, (struct sockaddr*) &cli_addr, &cli_len);

        if (fork() == 0) {
            n = read(new_sock_fd, buf, sizeof(buf));
            printf("Message from client: %s\n", buf);
            n = write(new_sock_fd, buf, sizeof(buf));
            close(new_sock_fd);
            exit(0);
        } else {
            close(new_sock_fd);
        }
    }

    return 0;
}