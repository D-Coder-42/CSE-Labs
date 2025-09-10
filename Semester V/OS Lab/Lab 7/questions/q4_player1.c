#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include "ttt_shm.h"

int main() {
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
        if (game->turn != 1) {
            sleep(1);
            continue;
        }
	
		display_board(game);
        printf("Player 1's turn. Enter row and column (0-2): ");
        int r, c;
        scanf("%d %d", &r, &c);

        if (r < 0 || r >= SIZE || c < 0 || c >= SIZE || game->board[r][c] != 0) {
            printf("Invalid move. Try again.\n");
            continue;
        }

        game->board[r][c] = 1;
        game->turn = 2;

        // Check for win
        for (int i = 0; i < SIZE; i++) {
            if (game->board[i][0] == 1 && game->board[i][1] == 1 && game->board[i][2] == 1)
                game->winner = 1;
            if (game->board[0][i] == 1 && game->board[1][i] == 1 && game->board[2][i] == 1)
                game->winner = 1;
        }
        if (game->board[0][0] == 1 && game->board[1][1] == 1 && game->board[2][2] == 1)
            game->winner = 1;
        if (game->board[0][2] == 1 && game->board[1][1] == 1 && game->board[2][0] == 1)
            game->winner = 1;

        // Check for draw
        int full = 1;
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                if (game->board[i][j] == 0)
                    full = 0;
        if (full && game->winner == 0)
            game->winner = -1;
    }

    if (game->winner == 1)
        printf("Player 1 wins!\n");
    else if (game->winner == -1)
        printf("It's a draw!\n");

    shmdt(shared_mem);
    return 0;
}
