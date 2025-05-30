#include <intdef.h>
#include <stdio.h>
#include "piece.h"
#include "board.h"
#include "ui.h"

int main(void)
{
    board_t* board = board__init(piece__init());
    if (!board)
        return 1;

    if (!ui__SDL3_init())
    {
        board__free(board);
        return 1;
    }
    board->pieces[4].king = true;
    board->pieces[21].king = true;
    piece_t* piece = piece__move_piece(coord__from_xyn(3,5,22),coord__from_xyn(4,4,19), board);
    ui__loop(board);
    ui__quit_SDL3();
    board__free(board);
    return 0;
}