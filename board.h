#ifndef BOARD_H
#define BOARD_H

//#include "piece.h"

typedef struct _board
{
    u8 state;
    u8* indexes;
    piece_t* pieces;
} board_t;


board_t* board__init(piece_t* pieces);
void board__free(board_t* board);

#endif