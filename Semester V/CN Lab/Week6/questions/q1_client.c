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
    char buf[256], operator;
    int num1, num2;

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("172.16.48.161");
    addr.sin_port = htons(PORTNO);

    connect(sock_fd, (struct sockaddr*) &addr, sizeof(addr));

    printf("Enter first number: ");
    scanf("%d", &num1);
    printf("Enter operator (+, -, *, /, %%): ");
    scanf(" %c", &operator);
    printf("Enter second number: ");
    scanf("%d", &num2);

    // Format: "num1 operator num2"
    sprintf(buf, "%d %c %d", num1, operator, num2);
    write(sock_fd, buf, strlen(buf) + 1);

    read(sock_fd, buf, sizeof(buf));
    printf("Result from server: %s\n", buf);

    close(sock_fd);
    return 0;
}
