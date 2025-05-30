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

typedef enum _rel_loc_name
{
    BTM_LEFT = 0,
    BTM_RIGHT = 1,
    TOP_RIGHT = 2,
    TOP_LEFT = 3
    /*
    [3]   [2]
       [X]
    [0]   [1]
    */
} rel_loc_name_t; // counter-clockwise

typedef struct _loc_node
{
    coord_t dest;
    coord_t capt;
    struct _loc_node** next;
    u8 count;
    rel_loc_name_t loc;
} loc_node_t;

#include "board.h"

piece_t* piece__init(void);
loc_node_t** piece__possible_captures(piece_t* piece, board_t* board, coord_t* aux_coord, u8* count);
coord_vec_t* piece__possible_moves(piece_t* piece, u8* indexes);
void piece__free_coord_vec(coord_vec_t* vec);
void piece__free_capture_chain(loc_node_t** chain, u8 count);
piece_t* piece__get_from_n(u8 n, board_t* board);
piece_t* piece__move_piece(coord_t src, coord_t dst, board_t* board);

#endif