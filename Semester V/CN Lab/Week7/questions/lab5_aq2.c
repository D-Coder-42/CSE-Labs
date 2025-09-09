#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 4096
#define PORT 80

int main() {
    int sock_fd;
    struct sockaddr_in server_addr;
    struct hostent *server;
    char buffer[BUFFER_SIZE];
    char request[512];

    const char *hostname = "example.com";

    // Resolve hostname to IP
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr, "Error: No such host\n");
        exit(1);
    }

    // Create socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Setup server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    // Connect to server
    if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sock_fd);
        exit(1);
    }

    // Craft HTTP GET request
    snprintf(request, sizeof(request),
             "GET / HTTP/1.1\r\n
             Host: %s\r\n
             Connection: close\r\n
             \r\n", hostname);

    // Send request
    send(sock_fd, request, strlen(request), 0);

    // Receive and display response
    printf("HTTP Response:\n\n");
    int bytes_received;
    while ((bytes_received = recv(sock_fd, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';
        printf("%s", buffer);
    }

    close(sock_fd);
    return 0;
}
