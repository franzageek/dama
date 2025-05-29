#include "coord.h"
#include <stdlib.h>
#include <stdio.h>

coord_t* coord__gen_xy(u8 n)
{
    coord_t* coord = (coord_t*)malloc(sizeof(coord_t));
    if (!coord)
    {
        perror("malloc()");
        return NULL;
    }

    coord->n = n;
    coord->y = (coord->n - (coord->n % 4) - ( (coord->n % 4) == 0 ? 4 : 0 ) ) / 4; // subtraction underflows when 0
    coord->x =  (((coord->n % 4) * 2) - 1 - (coord->y % 2 != 0 ? 0 : 1));
    return coord;
}