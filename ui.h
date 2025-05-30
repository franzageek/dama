#ifndef UI_H
#define UI_H

#include <intdef.h> 
#include "piece.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 640
#define BOARD_SIZE 8
#define CELL_SIZE (WINDOW_WIDTH/BOARD_SIZE)

bool ui__SDL3_init(void);
void ui__quit_SDL3(void);
void ui__draw_board(void);
void ui__draw_pieces(board_t* board);
void ui__loop(board_t* board);

#endif