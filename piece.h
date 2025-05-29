#ifndef PIECE_H
#define PIECE_H

#include "coord.h"

typedef struct _piece
{
    coord_t coord;
    u8 player;
    u8 king;
    u8 valid;
} __attribute__((packed)) piece_t;

#include "board.h"

piece_t* piece__init(void);
coord_t* piece__possible_moves(piece_t* piece, u8* indexes);
piece_t* piece__move_piece(coord_t src, coord_t dst, board_t* board);

#endif