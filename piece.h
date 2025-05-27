#ifndef PIECE_H
#define PIECE_H

#include <intdef.h>

typedef struct _coord
{
    u8 x;
    u8 y;
    u8 n;
} __attribute__((packed)) coord_t;

typedef struct _piece
{
    coord_t coord;
    u8 player;
    u8 dama;
    u8 valid;
} __attribute__((packed)) piece_t;

piece_t* piece__init(void);

#endif