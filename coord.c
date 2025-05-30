#include "coord.h"
#include <stdlib.h>
#include <stdio.h>

coord_t coord__gen_n_from_xy(u8 n)
{
    coord_t coord = {0};
    coord.n = n;
    coord.n = n;
    coord.y = 
    (
        coord.n - (coord.n % 4) - 
        ( 
            (
                (
                    coord.n == 0 ?
                    1 : coord.n
                ) % 4
            ) == 0 ? 
            4 : 0 
        ) 
    ) / 4; // [x] subtraction underflows when 0
    coord.x = 
    (
        ((
            coord.n % 4 == 0 ?
            4 : coord.n % 4
        ) * 2) - 1 - 
        (
            coord.y % 2 != 0 ? 
            0 : 1
        )
    );
    return coord;
}

coord_t coord__gen_struct_from_xyn(u8 x, u8 y, u8 n)
{
    coord_t coord = {x,y,n};
    return coord;
}

bool coord__is_null(coord_t coord)
{
    return coord.x == 0 && coord.y == 0 && coord.n == 0;
}
