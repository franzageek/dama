#include <intdef.h>
#include <stdio.h>
#include "piece.h"
#include "board.h"

int main(void)
{
    board_t* board = board__init(piece__init());
    if (!board)
        return 1;

    for (int i = 0; i < 24; ++i)
    {
        printf("piece[%02d] { y = %u, x = %u, n = %u, present = %u }\n", i%12, board->pieces[i].coord.y, board->pieces[i].coord.x, board->pieces[i].coord.n, board->presence[board->pieces[i].coord.n-1]);
    }
    board__free(board);
    return 0;
}