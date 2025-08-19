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
    int sock_fd, n = 1;
    struct sockaddr_in addr;
    char input[256], buf[256];

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("172.16.48.161");
    addr.sin_port = htons(PORTNO);
    
    int status = connect(sock_fd, (struct sockaddr*) &addr, sizeof(addr));

    if (status == -1) {
        perror("Client Error");
        exit(1);
    }
    
    printf("Enter string:\t");
    gets(input);
    input[strlen(input)] = '\0';
    
    write(sock_fd, input, strlen(input));
    printf("String sent back from server: ");
    while(n) {
        n = read(sock_fd, buf, sizeof(buf));
        puts(buf);
    }

    return 0;
}