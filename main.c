#include <intdef.h>
#include <stdio.h>
#include "piece.h"
#include "board.h"

int main(void)
{
    board_t* board = board__init(piece__init());
    if (!board)
        return 1;

    /*for (int i = 0; i < 24; ++i)
    {
        printf("piece[%02d] { y = %u, x = %u, n = %u, present = %u }\n", i, board->pieces[i].coord.y, board->pieces[i].coord.x, board->pieces[i].coord.n, board->indexes[board->pieces[i].coord.n-1]);
    }*/
    //board->indexes[11] = 0;
    piece__possible_moves(&board->pieces[22-8-1], board->indexes);
    board__free(board);
    return 0;
}