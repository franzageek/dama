#include "board.h"
#include <stdlib.h>
#include <stdio.h>

board_t* board__init(piece_t* pieces)
{
    u8* presence = (u8*)calloc(32, sizeof(u8));
    board_t* board = (board_t*)calloc(1, sizeof(board_t));
    if (!presence || !pieces || !board)
    {
        perror("calloc()");
        return NULL;
    }
    for (u8 i = 0; i < 32; ++i)
    {
        if (i < 12 || i >= 20)
            presence[i] = true;
    }
    board->state = true;
    board->presence = presence;
    board->pieces = pieces;
    return board;
}

void board__free(board_t* board)
{
    free(board->pieces);
    free(board->presence);
    free(board);
    return;
}