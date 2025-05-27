#include <intdef.h>
#include <stdio.h>
#include "piece.h"

int main(void)
{
    piece_t* piece = piece__init();
    if (!piece)
        return 1;

    for (int i = 0; i < 24; ++i)
    {
        printf("piece[%02d] { y = %u, x = %u }\n", i%12, piece[i].y, piece[i].x);
    }
    return 0;
}