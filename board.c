#include "piece.h"
#include <stdlib.h>
#include <stdio.h>

board_t* board__init(piece_t* pieces)
{
    printdb("initializing board...");
    u8* presence = (u8*)calloc(32, sizeof(u8));
    board_t* board = (board_t*)calloc(1, sizeof(board_t));
    if (!presence || !pieces || !board)
    {
        perror("calloc()");
        free(presence);
        free(board);
        return NULL;
    }
    u8 j = 1;
    for (u8 i = 0; i < 32; ++i)
    {
        if (i < 12 || i >= 20)
        {
            presence[i] = j;
            ++j;
        }
    }
    board->state = true;
    board->indexes = presence;
    board->pieces = pieces;
    printdb("done\n");
    return board;
}

void board__free(board_t* board)
{
    printdb("freeing pieces...");
    free(board->pieces);
    printdb("done\n");
    printdb("freeing tile array...");
    free(board->indexes);
    printdb("done\n");
    printdb("freeing board...");
    free(board);
    printdb("done\n");
    return;
}
