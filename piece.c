#include "piece.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum _loc_state
{
    STATE_FREE = 0,
    STATE_BUSY = 1,
    STATE_WALL = 2,
    STATE_OUT_OF_RANGE = 3
} loc_state_t;

typedef struct _coord_state
{
    loc_state_t state;
    coord_t coord;
} __attribute__((packed)) coord_state_t;

coord_state_t calculate_next(coord_t* start, u8* indexes, u8 loc) // 
{   // [x]: EXPAND TO X,Y,N
    u8 next = 
    (
        loc == 0 ?
            start->n + ( start->y % 2 != 0 ? 4 : 3 )
        : loc == 1 ?
            start->n + ( start->y % 2 != 0 ? 5 : 4 )
        : loc == 2 ?
            start->n - ( start->y % 2 != 0 ? 3 : 4 )
        : loc == 3 ?
            start->n - ( start->y % 2 != 0 ? 4 : 5 )
        : 33
    );

    /*
    
    [3]   [2]
       [X]
    [0]   [1]
    
    */
    if (next <= 32)
    {
        if 
        (
            ((loc == 0 || loc == 3) && start->n != 1 && start->n != 9 && start->n != 17 && start->n != 25)
            ||
            ((loc == 1 || loc == 2) && start->n != 8 && start->n != 16 && start->n != 24 && start->n != 32)
        ) 
        {
            coord_t coord = coord__from_n(next);
            if (indexes[next-1] == 0)
            {
                //debug(printf("possible move towards {%u}: [y = %u | x = %u | n = %u]\n", loc, coord.y, coord.x, coord.n));
                coord_state_t pair = 
                {
                    .coord = coord,
                    .state = STATE_FREE // empty location
                };
                return pair;
            }
            coord_state_t pair = 
            {
                .coord = coord,
                .state = STATE_BUSY // busy location
            };
            return pair;
        }
        coord_state_t pair = 
        {
            .coord = {0},
            .state = STATE_WALL // wall
        };
        return pair;
    }
    coord_state_t pair = 
    {
        .coord = {0},
        .state = STATE_OUT_OF_RANGE // out of the checkboard
    };
    return pair;
}

piece_t* piece__init(void)
{
    piece_t* piece = (piece_t*)calloc(12*2, sizeof(piece_t));
    if (!piece)
    {
        perror("calloc()");
        return NULL;
    }
    for (u8 i = 0; i < 24; ++i)
    {
        piece[i].valid = true;

        if (i < 12)
        {
            piece[i].player = true;
            piece[i].coord.y = (i - (i % 4)) / 4; 
            piece[i].coord.n = i+1;
        }
        else
        {
            piece[i].player = false;
            piece[i].coord.y = ((i - (i % 4)) / 4) + 2; 
            piece[i].coord.n = i+1+8;
        }

        piece[i].coord.x = (i * 2) % 8 + (
            piece[i].coord.y % 2 != 0 ? 
                1
            :
                0
        );
    }
    return piece;
}

void grow_coord_vec(coord_vec_t* vec, coord_t* coord, u8 loc, u8* indexes)
{
    coord_state_t state = calculate_next(coord, indexes, loc);
    if (!coord__is_null(state.coord) && state.state == STATE_FREE)
    {
        vec->table[vec->len] = (coord_t*)malloc(sizeof(coord_t));
        memmove(vec->table[vec->len], &state.coord, sizeof(coord_t));
        ++vec->len;
    }
    return;
}

coord_vec_t* piece__possible_moves(piece_t* piece, u8* indexes)
{
    coord_vec_t* vec = NULL;
    if (!piece->king)
    {
        if (piece->player)
        {
            vec = (coord_vec_t*)malloc(sizeof(coord_vec_t));
            if (!vec)
            {
                perror("malloc()");
                return NULL;
            }
            vec->table = (coord_t**)malloc(2*sizeof(coord_t*));
            if (!vec->table)
            {
                perror("malloc()");
                return NULL;
            }
            vec->len = 0;
            grow_coord_vec(vec, &piece->coord, BTM_LEFT, indexes);
            grow_coord_vec(vec, &piece->coord, BTM_RIGHT, indexes);
        }
        else
        {
            vec = (coord_vec_t*)malloc(sizeof(coord_vec_t));
            if (!vec)
            {
                perror("malloc()");
                return NULL;
            }
            vec->table = (coord_t**)malloc(2*sizeof(coord_t*));
            if (!vec->table)
            {
                perror("malloc()");
                return NULL;
            }
            vec->len = 0;
            grow_coord_vec(vec, &piece->coord, TOP_RIGHT, indexes);
            grow_coord_vec(vec, &piece->coord, TOP_LEFT, indexes);
        }
    }
    else
    {
        vec = (coord_vec_t*)malloc(sizeof(coord_vec_t));
        if (!vec)
        {
            perror("malloc()");
            return NULL;
        }
        vec->table = (coord_t**)malloc(4*sizeof(coord_t*));
        if (!vec->table)
        {
            perror("malloc()");
            return NULL;
        }
        vec->len = 0;
        grow_coord_vec(vec, &piece->coord, BTM_LEFT, indexes);
        grow_coord_vec(vec, &piece->coord, BTM_RIGHT, indexes);
        grow_coord_vec(vec, &piece->coord, TOP_RIGHT, indexes);
        grow_coord_vec(vec, &piece->coord, TOP_LEFT, indexes);
    }
    return vec;
}

void piece__free_coord_vec(coord_vec_t* vec)
{
    if (vec)
    {
        for (u8 i = 0; i < vec->len; ++i)
            free(vec->table[i]);
    
        free(vec->table);
        free(vec);
    }
    return;
}

piece_t* piece__move_piece(coord_t src, coord_t dst, board_t* board)
{
    u8 src_indx = board->indexes[src.n-1];
    //check dest
    board->indexes[dst.n-1] = src_indx;
    board->indexes[src.n-1] = 0;

    piece_t* curr_piece = &board->pieces[src_indx - 1];

    coord_t tmp = coord__from_n(dst.n);
    memmove(&(curr_piece->coord), &tmp, sizeof(coord_t));
    return curr_piece;
}

piece_t* piece__get_from_n(u8 n, board_t* board)
{
    return (
        board->indexes[n-1] > 0 && board->indexes[n-1] <= 24 ?
            &board->pieces[board->indexes[n-1]-1]
        :
            NULL
    );
}

loc_node_t** get_node(piece_t* piece, board_t* board, coord_t* aux_coord, u8* count, u8 loc, loc_node_t** start_node)
{
    loc_node_t** node = start_node;
    coord_state_t pair0 = calculate_next(aux_coord, board->indexes, loc);
    if (!coord__is_null(pair0.coord))
    {
        if (pair0.state == STATE_BUSY)
        {
            if (piece__get_from_n(pair0.coord.n, board) && piece__get_from_n(pair0.coord.n, board)->player != piece->player)
            {
                coord_state_t pair1l = calculate_next(&pair0.coord, board->indexes, loc);
                if (!coord__is_null(pair1l.coord))
                {
                    if (pair1l.state == STATE_FREE debug(|| !piece__get_from_n(pair1l.coord.n, board)->valid)) // allows to test pieces by setting them as invalid instead of removing them from the checkboard
                    {
                        loc_node_t* lnode = NULL;
                        if (!node)
                        {
                            node = (loc_node_t**)calloc(1, sizeof(loc_node_t*));
                            if (!node)
                            {
                                perror("calloc()");
                                return NULL;
                            }
                            *node = (loc_node_t*)calloc(1, sizeof(loc_node_t));
                            if (!*node)
                            {
                                perror("calloc()");
                                free(node);
                                return NULL;
                            }
                            lnode = *node;
                        }
                        else
                        {
                            if (*node)
                            {
                                loc_node_t** sib = (loc_node_t**)calloc((*count) + 1, sizeof(loc_node_t*));
                                if (!sib)
                                {
                                    perror("calloc()");
                                    free(*node);
                                    free(node);
                                    return NULL;
                                }
                                memmove(sib, node, sizeof(loc_node_t*)*(*count));
                                free(node);
                                sib[*count] = (loc_node_t*)calloc(1, sizeof(loc_node_t));
                                if (!sib[*count])
                                {
                                    perror("calloc()");
                                    free(sib[0]);
                                    free(sib);
                                    return NULL;
                                }
                                lnode = sib[*count];
                                node = sib;
                            }
                        }
                        *count += 1;
                        lnode->next = piece__possible_captures(piece, board, &pair1l.coord, &lnode->count);
                        lnode->loc = loc;
                        memmove(&lnode->capt, &pair0.coord, sizeof(coord_t));
                        memmove(&lnode->dest, &pair1l.coord, sizeof(coord_t));
                        //printf("--can cap %u [%u:%u:%u]--\n", loc, lnode->capt.x, lnode->capt.y, lnode->capt.n);
                        //printf("--can eat %u [%u:%u:%u]--\n", loc, lnode->dest.x, lnode->dest.y, lnode->dest.n);
                    }
                }
            }
        }
    }
    return node; //fix leak
}

loc_node_t** piece__possible_captures(piece_t* piece, board_t* board, coord_t* aux_coord, u8* count)
{
    loc_node_t** node = NULL;
    if (!piece->king)
    {
        if (piece->player)
        {
            node = get_node(piece, board, aux_coord == NULL ? &piece->coord : aux_coord, count, BTM_LEFT, NULL);
            node = get_node(piece, board, aux_coord == NULL ? &piece->coord : aux_coord, count, BTM_RIGHT, node);
        }
        else
        {
            node = get_node(piece, board, aux_coord == NULL ? &piece->coord : aux_coord, count, TOP_RIGHT, NULL);
            node = get_node(piece, board, aux_coord == NULL ? &piece->coord : aux_coord, count, TOP_LEFT, node);
        }
    }
    else // [x]: king case
    {
        node = get_node(piece, board, aux_coord == NULL ? &piece->coord : aux_coord, count, BTM_LEFT, NULL);
        node = get_node(piece, board, aux_coord == NULL ? &piece->coord : aux_coord, count, BTM_RIGHT, node);
        node = get_node(piece, board, aux_coord == NULL ? &piece->coord : aux_coord, count, TOP_RIGHT, node);
        node = get_node(piece, board, aux_coord == NULL ? &piece->coord : aux_coord, count, TOP_LEFT, node);
    }
    return node;
}

void piece__free_capture_chain(loc_node_t** chain, u8 count)
{
    if (chain)
    {
        if (count > 0)
        {
            for (u8 i = 0; i < count; ++i)
            {
                if (chain[i]->next)
                    piece__free_capture_chain(chain[i]->next, chain[i]->count);
                
                //printf("freeing DST[%u,%u,%u] CAP[%u,%u,%u]\n", chain[i]->dest.x, chain[i]->dest.y, chain[i]->dest.n, chain[i]->capt.x, chain[i]->capt.y, chain[i]->capt.n);
                free(chain[i]);
            }
        }
        free(chain);
    }
    return;
}
