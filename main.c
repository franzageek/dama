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

    piece_t* piece = piece__move_piece(coord__from_xyn(3,5,22),coord__from_xyn(4,4,19), board);
    piece_t* piece1 = piece__move_piece(coord__from_xyn(6,2,12), coord__from_xyn(5,3,15), board);
    piece_t* piece2 = piece__move_piece(coord__from_xyn(7,5,24), coord__from_xyn(4,6,20), board);
    piece__move_piece(coord__from_xyn(7,1,8), coord__from_xyn(6,2,12), board);
    piece__get_from_n(11, board)->player = false;
    piece__get_from_n(12, board)->player = false;
    piece__get_from_n(6, board)->valid = false;
    piece1->king = true;
    /*printf("0--%u:%u:%u\n", piece->coord.x, piece->coord.y, piece->coord.n);
    printf("1--%u:%u:%u\n", piece1->coord.x, piece1->coord.y, piece1->coord.n);
    printf("2--%u:%u:%u\n", piece2->coord.x, piece2->coord.y, piece2->coord.n);
    piece__possible_moves(piece, board->indexes);
    piece__possible_moves(piece1, board->indexes);
    piece__possible_moves(piece2, board->indexes);*/
    u8 count = 0;
    loc_node_t** chain = piece__possible_captures(piece1, board, NULL, &count);
    piece__free_capture_chain(chain, count);
    //piece__possible_moves(&board->pieces[11-1], board->indexes);
    board__free(board);
    return 0;
}