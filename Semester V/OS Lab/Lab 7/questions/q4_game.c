#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include "ttt_shm.h"

int main(int argc, char* argv[]) {
    if (argc != 2 || (strcmp(argv[1], "1") != 0 && strcmp(argv[1], "2") != 0)) {
        printf("Usage: %s [1|2]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int player = atoi(argv[1]);
    void* shared_mem = NULL;
    shared_game* game;
    int shmid;

    shmid = shmget((key_t)5678, sizeof(shared_game), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }

    shared_mem = shmat(shmid, NULL, 0);
    if (shared_mem == (void*)-1) {
        perror("shmat failed");
        exit(EXIT_FAILURE);
    }

    game = (shared_game*)shared_mem;

    // Initialize board if first time
    if (game->turn == 0) {
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                game->board[i][j] = 0;
        game->turn = 1;
        game->winner = 0;
    }

    while (game->winner == 0) {
        if (game->turn != player) {
            sleep(1);
            continue;
        }

        display_board(game);
        printf("Player %d's turn. Enter row and column (0-2): ", player);
        int r, c;
        scanf("%d %d", &r, &c);

        if (r < 0 || r >= SIZE || c < 0 || c >= SIZE || game->board[r][c] != 0) {
            printf("Invalid move. Try again.\n");
            continue;
        }

        game->board[r][c] = player;
        check_game_status(game, player);
        if (game->winner == 0)
            game->turn = (player == 1) ? 2 : 1;
    }

    display_board(game);
    if (game->winner == player)
        printf("Player %d wins!\n", player);
    else if (game->winner == -1)
        printf("It's a draw!\n");
    else
        printf("Player %d loses.\n", player);

    shmdt(shared_mem);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
