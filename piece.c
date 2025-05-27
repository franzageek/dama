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
            piece[i].player = false;
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
    if (!piece->king)
    {
        if (piece->player == true)
        {
            u8 nextl = piece->coord.n + ( piece->coord.y % 2 != 0 ? 4 : 3 );
            if (nextl < 32 && indexes[nextl-1] == 0)
            {
                if (piece->coord.n != 1 && piece->coord.n != 9 && piece->coord.n != 17 && piece->coord.n != 25) // hardcode left wall hits -- BAD!!
                    printf("w POSSIBLE: y = %u | x = %u | n = %u\n", 0, 0, nextl);
                
            }
            u8 nextr = piece->coord.n + ( piece->coord.y % 2 != 0 ? 5 : 4 );
            if (nextr < 32 && indexes[nextr-1 ] == 0)
            {
                if (piece->coord.n != 8 && piece->coord.n != 16 && piece->coord.n != 24 && piece->coord.n != 32) // same thing -- MOVE UP ONE CONDITION
                    printf("w POSSIBLE: y = %u | x = %u | n = %u\n", 0, 0, nextr);
                
            }
        }
        else if (piece->player == false)
        {
            u8 nextl = piece->coord.n - ( piece->coord.y % 2 != 0 ? 4 : 5 );
            if (nextl < 32 && indexes[nextl-1] == 0)
            {
                if (piece->coord.n != 1 && piece->coord.n != 9 && piece->coord.n != 17 && piece->coord.n != 25) // hardcode left wall hits -- BAD!!
                    printf("b POSSIBLE: y = %u | x = %u | n = %u\n", 0, 0, nextl);
                
            }
            u8 nextr = piece->coord.n - ( piece->coord.y % 2 != 0 ? 3 : 4 );
            if (nextr < 32 && indexes[nextr-1 ] == 0)
            {
                if (piece->coord.n != 8 && piece->coord.n != 16 && piece->coord.n != 24 && piece->coord.n != 32) // same thing -- MOVE UP ONE CONDITION
                    printf("b POSSIBLE: y = %u | x = %u | n = %u\n", 0, 0, nextr);
                
            }
        }
    }
    else
    {
        u8 nextlu = piece->coord.n - ( piece->coord.y % 2 != 0 ? 4 : 5 ); //d
        if (nextlu < 32 && indexes[nextlu-1] == 0)
        {
            if (piece->coord.n != 1 && piece->coord.n != 9 && piece->coord.n != 17 && piece->coord.n != 25) // hardcode left wall hits -- BAD!!
                printf("POSSIBLE: y = %u | x = %u | n = %u\n", 0, 0, nextlu);
            
        }

        u8 nextld = piece->coord.n + ( piece->coord.y % 2 != 0 ? 4 : 3 ); //a
        if (nextld < 32 && indexes[nextld-1] == 0)
        {
            if (piece->coord.n != 1 && piece->coord.n != 9 && piece->coord.n != 17 && piece->coord.n != 25) // hardcode left wall hits -- BAD!!
                printf("POSSIBLE: y = %u | x = %u | n = %u\n", 0, 0, nextld);
            
        }
        
        u8 nextru = piece->coord.n - ( piece->coord.y % 2 != 0 ? 3 : 4 ); //c
        if (nextru < 32 && indexes[nextru-1] == 0)
        {
            if (piece->coord.n != 8 && piece->coord.n != 16 && piece->coord.n != 24 && piece->coord.n != 32) // hardcode right wall hits -- BAD!!
                printf("POSSIBLE: y = %u | x = %u | n = %u\n", 0, 0, nextru);
            
        }
        
        u8 nextrd = piece->coord.n + ( piece->coord.y % 2 != 0 ? 5 : 4 ); //b
        if (nextrd < 32 && indexes[nextrd-1] == 0)
        {
            if (piece->coord.n != 8 && piece->coord.n != 16 && piece->coord.n != 24 && piece->coord.n != 32) // hardcode right wall hits -- BAD!!
                printf("POSSIBLE: y = %u | x = %u | n = %u\n", 0, 0, nextrd);
            
        }
    }
    return NULL;
}