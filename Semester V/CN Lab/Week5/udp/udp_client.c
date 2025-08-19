#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
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

    char input[25], buf[25];
    printf("Enter string:\t");
    gets(input);
    input[strlen(input)] = '\0';

    int len = sizeof(addr);
    int m = sendto(sd, input, sizeof(input), 0, (struct sockaddr*) &addr, len);
    int n = recvfrom(sd, buf, sizeof(buf), 0, (struct sockaddr*) &addr, &len);
    
    printf("Server echo:\t");
    puts(buf);

    return 0;
}