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

#endif
