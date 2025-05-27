#include "board.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>

piece_t* piece__init(void)
{
    piece_t* piece = (piece_t*)calloc(12*2, sizeof(piece_t));
    if (piece == 0)
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
            piece[i].coord.y = ((i - (i % 4)) / 4) + 2; 
            piece[i].coord.n = i+1+8;
        }

        piece[i].coord.x = (i * 2) % 8 + (
            piece[i].coord.y % 2 != 0 
            ? 
                1
            :
                0
        );
    }
    return piece;
}

coord_t* piece__possible_moves(piece_t* piece, u8* indexes)
{
    if (!piece->dama)
    {
        if (piece->player)
        {
            u8 delta_nextl = piece->coord.n + ( piece->coord.y % 2 != 0 ? 4 : 3 );
            if (delta_nextl < 32 && indexes[delta_nextl-1] == 0)
            {
                u8 a = piece->coord.n;
                if (a != 1 && a != 9 && a != 17 && a != 25) // hardcode left wall hits -- BAD!!
                    printf("POSSIBLE: y = %u | x = %u | n = %u\n", 0, 0, delta_nextl);
                
            }
            u8 delta_nextr = piece->coord.n + ( piece->coord.y % 2 != 0 ? 5 : 4 );
            if (delta_nextr < 32 && indexes[delta_nextr-1 ] == 0)
            {
                u8 b = piece->coord.n;
                if (b != 8 && b != 16 && b != 24 && b != 32) // same thing -- MOVE UP ONE CONDITION
                    printf("POSSIBLE: y = %u | x = %u | n = %u\n", 0, 0, delta_nextr);
                
            }
        }
    }
    return NULL;
}