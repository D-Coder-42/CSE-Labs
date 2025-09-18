#ifndef TTT_SHM_H
#define TTT_SHM_H

#include <stdio.h>

#define SIZE 3

typedef struct {
    int board[SIZE][SIZE];  // 3x3 Tic-Tac-Toe board
    int turn;               // 1 or 2: whose turn it is
    int winner;             // 0 = ongoing, 1 or 2 = winner, -1 = draw
} shared_game;

static inline void display_board(shared_game* game) {
    printf("\nCurrent Board:\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            char mark;
            if (game->board[i][j] == 1) mark = 'X';
            else if (game->board[i][j] == 2) mark = 'O';
            else mark = '.';
            printf(" %c ", mark);
        }
        printf("\n");
    }
    printf("\n");
}

static inline void check_game_status(shared_game* game, int player) {
    // Check rows and columns
    for (int i = 0; i < SIZE; i++) {
        if (game->board[i][0] == player && game->board[i][1] == player && game->board[i][2] == player) {
            game->winner = player;
            return;
        }
        if (game->board[0][i] == player && game->board[1][i] == player && game->board[2][i] == player) {
            game->winner = player;
            return;
        }
    }

    // Check diagonals
    if (game->board[0][0] == player && game->board[1][1] == player && game->board[2][2] == player) {
        game->winner = player;
        return;
    }
    if (game->board[0][2] == player && game->board[1][1] == player && game->board[2][0] == player) {
        game->winner = player;
        return;
    }

    // Check for draw
    int full = 1;
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (game->board[i][j] == 0)
                full = 0;

    if (full)
        game->winner = -1;
}

#endif
