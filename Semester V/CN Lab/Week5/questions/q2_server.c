#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORTNO 9704

int main () {
    int sd;
    char buf[256];
    int matrix[10][10];
    int row_lengths[10];
    int row = 0;

    struct sockaddr_in ser_addr, cli_addr;

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = inet_addr("172.16.48.161");
    ser_addr.sin_port = htons(PORTNO);

    bind(sd, (struct sockaddr*) &ser_addr, sizeof(ser_addr));
    int len = sizeof(cli_addr);

    while (1) {
        recvfrom(sd, buf, sizeof(buf), 0, (struct sockaddr*) &cli_addr, &len);
        if (strcmp(buf, "stop") == 0) {
            break;
        }

        int col = 0;
        char *token = strtok(buf, " ");
        while (token != NULL) {
            matrix[row][col] = atoi(token);
            col++;
            token = strtok(NULL, " ");
        }
        row_lengths[row] = col;
        row++;

        sendto(sd, "Row received", 13, 0, (struct sockaddr*) &cli_addr, len);
    }

    char result[1024] = "";
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < row_lengths[i]; j++) {
            char temp[10];
            sprintf(temp, "%d ", matrix[i][j]);
            strcat(result, temp);
        }
        strcat(result, "\n");
    }

    sendto(sd, result, sizeof(result), 0, (struct sockaddr*) &cli_addr, len);

    return 0;
}
