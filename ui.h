#ifndef UI_H
#define UI_H

#include <intdef.h> 
#include "piece.h"

#define WINDOW_WIDTH 650
#define WINDOW_HEIGHT 650
#define BOARD_SIZE 8
#define CELL_SIZE (WINDOW_WIDTH/BOARD_SIZE)

bool ui__SDL3_init(void);
void ui__quit_SDL3(void);
void ui__draw_board(void);
void ui__draw_pieces(board_t* board);
void ui__draw_hints(coord_t coord, board_t* board, bool capture_available);
bool ui__draw_capture_hints(coord_t coord, board_t* board);
void* ui__get_renderer(void); // use void* to prevent including SDL.h
void* ui__get_window(void);

extern int window;

#endif