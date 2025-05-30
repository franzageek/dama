#ifndef COORD_H
#define COORD_H

#include <intdef.h>

typedef struct _coord
{
    u8 x;
    u8 y;
    u8 n;
} __attribute__((packed)) coord_t;

coord_t coord__gen_n_from_xy(u8 n);
coord_t coord__gen_struct_from_xyn(u8 x, u8 y, u8 n);
bool coord__is_null(coord_t coord);

#endif