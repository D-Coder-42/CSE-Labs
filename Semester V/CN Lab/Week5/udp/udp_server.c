#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORTNO 9704

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

int main () {
    int sd;
    char buf[25];
    struct sockaddr_in ser_addr, cli_addr;

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = inet_addr("172.16.48.161");
    ser_addr.sin_port = htons(PORTNO);

    int status = bind(sd, (struct sockaddr*) &ser_addr, sizeof(ser_addr));
    int len = sizeof(cli_addr);
    int m = recvfrom(sd, buf, sizeof(buf), 0, (struct sockaddr*) &cli_addr, &len);

    printf("Server received: %s\n", buf);
    int n = sendto(sd, buf, sizeof(buf), 0, (struct sockaddr*) &cli_addr, len);

    return 0;
}

