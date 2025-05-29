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

typedef struct _loc_node
{
    coord_t dest;
    coord_t capt;
    struct _loc_node** next;
    bool has_lr;
} loc_node_t;

#include "board.h"

piece_t* piece__init(void);
loc_node_t** piece__possible_captures(piece_t* piece, board_t* board, coord_t* aux_coord, bool* both);
coord_t* piece__possible_moves(piece_t* piece, u8* indexes);
void piece__free_capture_chain(loc_node_t** chain, bool both);
piece_t* piece__move_piece(coord_t src, coord_t dst, board_t* board);

#endif