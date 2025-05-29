#ifndef COORD_H
#define COORD_H

#include <intdef.h>

typedef struct _coord
{
    u8 x;
    u8 y;
    u8 n;
} __attribute__((packed)) coord_t;

coord_t* coord__gen_xy(u8 n);

#endif