#include <intdef.h>

typedef struct _piece
{
    u8 x;
    u8 y;
    u8 player;
    u8 dama;
    u8 valid;
} __attribute__((packed)) piece_t;

piece_t* piece__init(void);
