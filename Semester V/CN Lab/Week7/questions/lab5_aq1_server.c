#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORTNO 10200
#define ENCRYT_KEY 3

int main() {
    int sock_fd, new_sock_fd, cli_len;
    struct sockaddr_in ser_addr, cli_addr;
    char encrypted[256], decrypted[256];
    int i, n;

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = inet_addr("172.16.48.161");
    ser_addr.sin_port = htons(PORTNO);
    bind(sock_fd, (struct sockaddr*) &ser_addr, sizeof(ser_addr));

    listen(sock_fd, 1);
    printf("Server ready. Waiting for encrypted message...\n");

    cli_len = sizeof(cli_addr);
    new_sock_fd = accept(sock_fd, (struct sockaddr*) &cli_addr, &cli_len);

    n = read(new_sock_fd, encrypted, sizeof(encrypted));
    encrypted[n] = '\0';

    for (i = 0; encrypted[i] != '\0'; i++) {
        decrypted[i] = encrypted[i] - ENCRYT_KEY;
    }
    decrypted[i] = '\0';

    printf("Encrypted message: %s\n", encrypted);
    printf("Decrypted message: %s\n", decrypted);

    close(new_sock_fd);
    close(sock_fd);
    return 0;
}
