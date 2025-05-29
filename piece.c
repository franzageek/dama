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

typedef struct _coord_bool_pair
{
    loc_state_t state;
    coord_t* coord;
} __attribute__((packed)) coord_bool_pair_t;

typedef struct _loc_node
{
    coord_t dest;
    coord_t capt;
    struct _loc_node** next;
    bool has_lr;
} loc_node_t;

coord_bool_pair_t calculate_ld(coord_t* start, u8* indexes) // a
{   // TODO: EXPAND TO X,Y,N
    u8 nextld = start->n + ( start->y % 2 != 0 ? 4 : 3 );
    if (nextld < 32)
    {
        if (start->n != 1 && start->n != 9 && start->n != 17 && start->n != 25) 
        {
            coord_t* coord_ld = coord__gen_xy(nextld);
            if (indexes[nextld-1] == 0)
            {
                printf("[A]POSSIBLE: y = %u | x = %u | n = %u\n", coord_ld->y, coord_ld->x, coord_ld->n);
                coord_bool_pair_t pair = 
                {
                    .coord = coord_ld,
                    .state = STATE_FREE, //casella vuota
                };
                return pair;
            }
            coord_bool_pair_t pair = 
            {
                .coord = coord_ld,
                .state = STATE_BUSY, //casella piena
            };
            return pair;
        }
        coord_bool_pair_t pair = 
        {
            .coord = NULL,
            .state = STATE_WALL, //muro
        };
        return pair;
    }
    coord_bool_pair_t pair = 
    {
        .coord = NULL,
        .state = STATE_OUT_OF_RANGE, //fuori dalla schacchiera
    };
    return pair;
}

coord_bool_pair_t calculate_rd(coord_t* start, u8* indexes) // b
{   // TODO: EXPAND TO X,Y,N
    u8 nextrd = start->n + ( start->y % 2 != 0 ? 5 : 4 );
    if (nextrd < 32)
    {
        if (start->n != 8 && start->n != 16 && start->n != 24 && start->n != 32) 
        {
            coord_t* coord_rd = coord__gen_xy(nextrd);
            if (indexes[nextrd-1] == 0)
            {
                printf("[B]POSSIBLE: y = %u | x = %u | n = %u\n", coord_rd->y, coord_rd->x, coord_rd->n);
                coord_bool_pair_t pair = 
                {
                    .coord = coord_rd,
                    .state = STATE_FREE, //casella vuota
                };
                return pair;
            }
            coord_bool_pair_t pair = 
            {
                .coord = coord_rd,
                .state = STATE_BUSY, //casella piena
            };
            return pair;
        }
        coord_bool_pair_t pair = 
        {
            .coord = NULL,
            .state = STATE_WALL, //muro
        };
        return pair;
    }
    coord_bool_pair_t pair = 
    {
        .coord = NULL,
        .state = STATE_OUT_OF_RANGE, //fuori dalla schacchiera
    };
    return pair;
}


coord_bool_pair_t calculate_ru(coord_t* start, u8* indexes) // c
{   // TODO: EXPAND TO X,Y,N
    u8 nextru = start->n - ( start->y % 2 != 0 ? 3 : 4 );
    if (nextru < 32)
    {
        if (start->n != 8 && start->n != 16 && start->n != 24 && start->n != 32) 
        {
            coord_t* coord_ru = coord__gen_xy(nextru);
            if (indexes[nextru-1] == 0)
            {
                printf("[C]POSSIBLE: y = %u | x = %u | n = %u\n", coord_ru->y, coord_ru->x, coord_ru->n);
                coord_bool_pair_t pair = 
                {
                    .coord = coord_ru,
                    .state = STATE_FREE, //casella vuota
                };
                return pair;
            }
            coord_bool_pair_t pair = 
            {
                .coord = coord_ru,
                .state = STATE_BUSY, //casella piena
            };
            return pair;
        }
        coord_bool_pair_t pair = 
        {
            .coord = NULL,
            .state = STATE_WALL, //muro
        };
        return pair;
    }
    coord_bool_pair_t pair = 
    {
        .coord = NULL,
        .state = STATE_OUT_OF_RANGE, //fuori dalla schacchiera
    };
    return pair;
}


coord_bool_pair_t calculate_lu(coord_t* start, u8* indexes) // d
{   // TODO: EXPAND TO X,Y,N
    u8 nextlu = start->n - ( start->y % 2 != 0 ? 4 : 5 );
    if (nextlu < 32)
    {
        if (start->n != 1 && start->n != 9 && start->n != 17 && start->n != 25) 
        {
            coord_t* coord_lu = coord__gen_xy(nextlu);
            if (indexes[nextlu-1] == 0)
            {
                printf("[D]POSSIBLE: y = %u | x = %u | n = %u\n", coord_lu->y, coord_lu->x, coord_lu->n);
                coord_bool_pair_t pair = 
                {
                    .coord = coord_lu,
                    .state = STATE_FREE, //casella vuota
                };
                return pair;
            }
            coord_bool_pair_t pair = 
            {
                .coord = coord_lu,
                .state = STATE_BUSY, //casella piena
            };
            return pair;
        }
        coord_bool_pair_t pair = 
        {
            .coord = NULL,
            .state = STATE_WALL, //muro
        };
        return pair;
    }
    coord_bool_pair_t pair = 
    {
        .coord = NULL,
        .state = STATE_OUT_OF_RANGE, //fuori dalla schacchiera
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
            free(calculate_ld(&piece->coord, indexes).coord);
            free(calculate_rd(&piece->coord, indexes).coord);
        }
        else
        {
            free(calculate_lu(&piece->coord, indexes).coord);
            free(calculate_ru(&piece->coord, indexes).coord);
        }
    }
    else
    {
        free(calculate_ld(&piece->coord, indexes).coord);
        free(calculate_rd(&piece->coord, indexes).coord);
        free(calculate_lu(&piece->coord, indexes).coord);
        free(calculate_ru(&piece->coord, indexes).coord);
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

/*
void rec_check_ld(coord_t* coord, u8* indexes, u8 level)
{
    if (level < 10)
    {
        coord_bool_pair_t pair = calculate_ld(coord, indexes);
        if (!pair.state && pair.coord != NULL)
        {
            rec_check_ld(&pair.coord, indexes);
        }
    }
}

coord_t** calculate_possible_captures(piece_t* piece, board_t* board)
{
    coord_t** capt_list = (coord_t**)malloc(sizeof(coord_t*));
    if (!capt_list)
    {
        perror("malloc()");
        return NULL;
    }

} */
/*
loc_node_t* piece__can_eat(piece_t* piece, board_t* board, coord_t* aux_coord, loc_node_t* prev)
{
    if(!piece->king)    
    {
        if(piece->player)
        {
            coord_bool_pair_t pair = calculate_ld(aux_coord, board->indexes);
            if (pair.coord)
            {
                if (pair.state == STATE_BUSY)
                {
                    if (get_piece_from_n(pair.coord->n, board) && !get_piece_from_n(pair.coord->n, board)->player)
                    {
                        loc_node_t* lnode = NULL;
                        coord_bool_pair_t pair1 = calculate_ld(&pair.coord, board->indexes);
                        if (pair1.coord)
                        {
                            if (pair1.state == STATE_FREE)
                            {
                                lnode = (loc_node_t*)malloc(sizeof(loc_node_t));
                                if (!lnode)
                                {
                                    perror("malloc()");
                                    return NULL;
                                }
                                memmove(&lnode->dest, pair1.coord, sizeof(coord_t));
                                memmove(&lnode->captured, pair.coord, sizeof(coord_t));
                                lnode->next = piece__can_eat(piece, board, &pair1.coord, lnode);
                                lnode->prev = prev;
                            }
                            free(pair1.coord);
                        }
                        pair1 = calculate_rd(&pair.coord, board->indexes);
                        if (pair1.coord)
                        {
                            if (pair1.state == STATE_FREE)
                            {
                                if (lnode)
                                lnode = (loc_node_t*)malloc(sizeof(loc_node_t));
                                if (!lnode)
                                {
                                    perror("malloc()");
                                    return NULL;
                                }
                                memmove(&lnode->dest, pair1.coord, sizeof(coord_t));
                                memmove(&lnode->captured, pair.coord, sizeof(coord_t));
                                lnode->next = piece__can_eat(piece, board, &pair1.coord, lnode);
                                lnode->prev = prev;
                            }
                            free(pair1.coord);
                        }
                    }
                }
                free(pair.coord);
            }
        }
    }
}*/


loc_node_t** piece__possible_captures(piece_t* piece, board_t* board, coord_t* aux_coord, bool* both)
{
    if (!piece->king)
    {
        if (piece->player)
        {
            loc_node_t** node = NULL;
            coord_bool_pair_t pair0 = calculate_ld(aux_coord, board->indexes);
            if (pair0.coord)
            {
                if (pair0.state == STATE_BUSY)
                {
                    if (get_piece_from_n(pair0.coord->n, board) && !get_piece_from_n(pair0.coord->n, board)->player)
                    {
                        coord_bool_pair_t pair1l = calculate_ld(pair0.coord, board->indexes);
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
                            }
                            free(pair1l.coord);
                        }
                    }
                }
                free(pair0.coord);
            }
            pair0 = calculate_rd(aux_coord, board->indexes);
            if (pair0.coord)
            {
                if (pair0.state == STATE_BUSY)
                {
                    if (get_piece_from_n(pair0.coord->n, board) && !get_piece_from_n(pair0.coord->n, board)->player)
                    {
                        coord_bool_pair_t pair1l = calculate_rd(pair0.coord, board->indexes);
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