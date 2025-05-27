#include "piece.h"
#include <stdlib.h>
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
            piece[i].y = (i - (i % 4)) / 4; 
            piece[i].x = (i * 2) % 8 + (
                piece[i].y % 2 != 0 
                ? 
                    1
                :
                    0
            );
        }
        else
        {
            piece[i].y = ((i - (i % 4)) / 4) + 2; 
            piece[i].x = (i * 2) % 8 + (
                piece[i].y % 2 != 0 
                ? 
                    1
                :
                    0
            );
        }
    }
    return piece;
}