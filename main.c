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

    u8 piece_src = 21;
    u8 piece_dest = 17;

    u8 src_indx = board->indexes[piece_src-1];

    board->indexes[piece_dest-1] = src_indx;
    printf("old piece = %u\n", src_indx);
    board->indexes[piece_src-1] = 0;

    piece_t* curr_piece = &board->pieces[src_indx - 1];

    curr_piece->king = true;

    curr_piece->coord.n = piece_dest;
    curr_piece->coord.y = (curr_piece->coord.n - (curr_piece->coord.n % 4) - ( (curr_piece->coord.n % 4) == 0 ? 4 : 0 ) ) / 4; // subtraction underflows when 0
    curr_piece->coord.x =  (((curr_piece->coord.n % 4) * 2) - 1 - (curr_piece->coord.y % 2 != 0 ? 0 : 1));
    
    piece__possible_moves(curr_piece, board->indexes);
    //piece__possible_moves(&board->pieces[11-1], board->indexes);
    board__free(board);
    return 0;
}