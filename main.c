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

    coord_t s = {3,5,22};
    coord_t d = {4,4,19};
    coord_t s1 = {6,2,12};
    coord_t d1 = {5,3,15};
    piece__possible_moves(piece__move_piece(s,d, board), board->indexes);
    piece__possible_moves(piece__move_piece(s1,d1, board), board->indexes);
    //piece__possible_moves(&board->pieces[11-1], board->indexes);
    bool both = false;
    loc_node_t** chain = piece__possible_captures(piece, board, NULL, &both);
    piece__free_capture_chain(chain, both);
    board__free(board);
    return 0;
}