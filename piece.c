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

typedef struct _coord_bool_pair
{
    loc_state_t state;
    coord_t* coord;
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
        : 32
    );

    /*
    
    [3]   [2]
       [X]
    [0]   [1]
    
    */
    if (next < 32)
    {
        if 
        (
            ((loc == 0 || loc == 3) && start->n != 1 && start->n != 9 && start->n != 17 && start->n != 25)
            ||
            ((loc == 1 || loc == 2) && start->n != 8 && start->n != 16 && start->n != 24 && start->n != 32)
        ) 
        {
            coord_t* coord = coord__gen_xy(next);
            if (indexes[next-1] == 0)
            {
                printf("[%u]POSSIBLE: y = %u | x = %u | n = %u\n", loc, coord->y, coord->x, coord->n);
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
            .coord = NULL,
            .state = STATE_WALL // wall
        };
        return pair;
    }
    coord_state_t pair = 
    {
        .coord = NULL,
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

coord_t* piece__possible_moves(piece_t* piece, u8* indexes)
{
    if (!piece->king)
    {
        if (piece->player)
        {
            free(calculate_next(&piece->coord, indexes, 0).coord);
            free(calculate_next(&piece->coord, indexes, 1).coord);
        }
        else
        {
            free(calculate_next(&piece->coord, indexes, 2).coord);
            free(calculate_next(&piece->coord, indexes, 3).coord);
        }
    }
    else
    {
        free(calculate_next(&piece->coord, indexes, 0).coord);
        free(calculate_next(&piece->coord, indexes, 1).coord);
        free(calculate_next(&piece->coord, indexes, 2).coord);
        free(calculate_next(&piece->coord, indexes, 3).coord);
    }
    return NULL;
}

piece_t* piece__move_piece(coord_t src, coord_t dst, board_t* board)
{
    u8 src_indx = board->indexes[src.n-1];
    //check dest
    board->indexes[dst.n-1] = src_indx;
    printf("old piece = %u\n", src_indx);
    board->indexes[src.n-1] = 0;

    piece_t* curr_piece = &board->pieces[src_indx - 1];

    coord_t* tmp = coord__gen_xy(dst.n);
    memmove(&(curr_piece->coord), tmp, sizeof(coord_t));
    free(tmp);
    return curr_piece;
}

piece_t* get_piece_from_n(u8 n, board_t* board)
{
    return (
        board->indexes[n-1] > 0 && board->indexes[n-1] <= 24 ?
            &board->pieces[board->indexes[n-1]-1]
        :
            NULL
    );
}

loc_node_t** piece__possible_captures(piece_t* piece, board_t* board, coord_t* aux_coord, bool* both)
{
    if (!piece->king)
    {
        if (piece->player)
        {
            loc_node_t** node = NULL;
            coord_state_t pair0 = calculate_next(aux_coord == NULL ? &piece->coord : aux_coord, board->indexes, 0);
            if (pair0.coord)
            {
                if (pair0.state == STATE_BUSY)
                {
                    if (get_piece_from_n(pair0.coord->n, board) && !get_piece_from_n(pair0.coord->n, board)->player)
                    {
                        coord_state_t pair1l = calculate_next(pair0.coord, board->indexes, 0);
                        if (pair1l.coord)
                        {
                            if (pair1l.state == STATE_FREE)
                            {
                                node = (loc_node_t**)malloc(sizeof(loc_node_t*));
                                if (!node)
                                {
                                    perror("malloc()");
                                    free(pair1l.coord);
                                    free(pair0.coord);
                                    return NULL;
                                }
                                *node = (loc_node_t*)malloc(sizeof(loc_node_t));
                                if (!*node)
                                {
                                    perror("malloc()");
                                    free(pair1l.coord);
                                    free(pair0.coord);
                                    free(node);
                                    return NULL;
                                }
                                *both = false;
                                (*node)->next = piece__possible_captures(piece, board, pair1l.coord, &(*node)->has_lr);
                                memmove(&(*node)->capt, pair0.coord, sizeof(coord_t));
                                memmove(&(*node)->dest, pair1l.coord, sizeof(coord_t));
                                printf("--can cap left [%u:%u:%u]--\n", (*node)->capt.x, (*node)->capt.y, (*node)->capt.n);
                                printf("--can eat left [%u:%u:%u]--\n", (*node)->dest.x, (*node)->dest.y, (*node)->dest.n);
                            }
                            free(pair1l.coord);
                        }
                    }
                }
                free(pair0.coord);
            }
            pair0 = calculate_next(aux_coord == NULL ? &piece->coord : aux_coord, board->indexes, 1);
            if (pair0.coord)
            {
                if (pair0.state == STATE_BUSY)
                {
                    if (get_piece_from_n(pair0.coord->n, board) && !get_piece_from_n(pair0.coord->n, board)->player)
                    {
                        coord_state_t pair1l = calculate_next(pair0.coord, board->indexes, 1);
                        if (pair1l.coord)
                        {
                            if (pair1l.state == STATE_FREE)
                            {
                                loc_node_t* lnode = NULL;
                                if (!node)
                                {
                                    node = (loc_node_t**)malloc(sizeof(loc_node_t*));
                                    if (!node)
                                    {
                                        perror("malloc()");
                                        free(pair1l.coord);
                                        free(pair0.coord);
                                        return NULL;
                                    }
                                    *node = (loc_node_t*)malloc(sizeof(loc_node_t));
                                    if (!*node)
                                    {
                                        perror("malloc()");
                                        free(pair1l.coord);
                                        free(pair0.coord);
                                        free(node);
                                        return NULL;
                                    }
                                    lnode = *node;
                                    *both = false;
                                    lnode->next = piece__possible_captures(piece, board, pair1l.coord, &lnode->has_lr);
                                }
                                else
                                {
                                    if (*node)
                                    {
                                        loc_node_t** sib = (loc_node_t**)malloc(sizeof(loc_node_t*)*2);
                                        if (!sib)
                                        {
                                            perror("malloc()");
                                            free(pair1l.coord);
                                            free(pair0.coord);
                                            free(*node);
                                            free(node);
                                            return NULL;
                                        }
                                        memmove(&sib[0], node, sizeof(loc_node_t*));
                                        free(node);
                                        sib[1] = (loc_node_t*)malloc(sizeof(loc_node_t));
                                        if (!sib[1])
                                        {
                                            perror("malloc()");
                                            free(pair1l.coord);
                                            free(pair0.coord);
                                            free(sib[0]);
                                            free(sib);
                                            return NULL;
                                        }
                                        lnode = sib[1];
                                        node = sib;
                                        *both = true;
                                        lnode->next = piece__possible_captures(piece, board, pair1l.coord, &lnode->has_lr);
                                    }
                                }
                                memmove(&lnode->capt, pair0.coord, sizeof(coord_t));
                                memmove(&lnode->dest, pair1l.coord, sizeof(coord_t));
                                printf("--can cap right [%u:%u:%u]--\n", lnode->capt.x, lnode->capt.y, lnode->capt.n);
                                printf("--can eat right [%u:%u:%u]--\n", lnode->dest.x, lnode->dest.y, lnode->dest.n);
                            }
                            free(pair1l.coord);
                        }
                    }
                }
                free(pair0.coord);
            }
            //TODO: right case
            return node; //cannot eat
        }
        else //TODO: black player
        {

        }
    }
    else //TODO: king case
    {

    }
    return NULL;
}

void piece__free_capture_chain(loc_node_t** chain, bool both)
{
    if (chain)
    {
        if (chain[0]->next)
            piece__free_capture_chain(chain[0]->next, chain[0]->has_lr);
        
        printf("freeing DST[%u,%u,%u] CAP[%u,%u,%u]\n", chain[0]->dest.x, chain[0]->dest.y, chain[0]->dest.n, chain[0]->capt.x, chain[0]->capt.y, chain[0]->capt.n);
        free(chain[0]);
        if (both)
        {
            if (chain[1]->next)
                piece__free_capture_chain(chain[1]->next, chain[1]->has_lr);
            
            printf("freeing DST[%u,%u,%u] CAP[%u,%u,%u]\n", chain[1]->dest.x, chain[1]->dest.y, chain[1]->dest.n, chain[1]->capt.x, chain[1]->capt.y, chain[1]->capt.n);
            free(chain[1]);
        }
        free(chain);
    }
    return;
}
