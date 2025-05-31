#include "ui.h"
#include "game.h"
#include <SDL3/SDL.h>
#include <stdio.h>

void get_max_capture_depth(loc_node_t** chain, u8 count, u8* depth, u8 level)
{
    if (chain)
    {
        for (u8 i = 0; i < count; ++i)
        {
            if (chain[i]->next)
                get_max_capture_depth(chain[i]->next, chain[i]->count, depth, level+1);

            if (level > *depth)
                *depth = level;
        }
    }
    return;
}

void capture(piece_t* piece, board_t* board, loc_node_t** chain, u8 count, u8 max_level, u8 level, bool* reached)
{
    if (chain)
    {
        for (u8 i = 0; i < count; ++i)
        {
            if (chain[i]->next)
                capture(piece, board, chain[i]->next, chain[i]->count, max_level, level+1, reached);
            
            if (*reached)
            {
                piece__get_from_n(chain[i]->capt.n, board)->valid = false;
                board->indexes[chain[i]->capt.n-1] = 0; //merge or move to bottom
                return;
            }

            if (level == max_level)
            {
                *reached = true;
                piece__move_piece(piece->coord, chain[i]->dest, board);
                piece__get_from_n(chain[i]->capt.n, board)->valid = false;
                board->indexes[chain[i]->capt.n-1] = 0;
                return;
            }
        }
    }
    return;
}

void game__loop(board_t* board)
{
    bool running = 1;
    coord_t from = {0};
    coord_t to = {0};
    SDL_Event e;

    while (running)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
                running = 0;

            else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            {
                int x = e.button.x; // [ ] fix integer consistency
                int y = e.button.y;
                int col = x / CELL_SIZE;
                int row = y / CELL_SIZE;
                if ((col % 2 == 0 && row %2 == 0) || (col % 2 != 0 && row % 2 != 0))
                {
                    if (coord__is_null(from)) 
                    {
                        if (board->indexes[coord__from_xy(col, row).n-1] != 0)
                        {
                            if (piece__get_from_n(coord__from_xy(col, row).n, board)->player == board->state)
                            {
                                from.x = col;                        
                                from.y = row;                        
                                from.n = coord__from_xy(col, row).n; // [x] impose STATE==FREE

                                printf("selected piece at [ x = %d | y = %d | n = %d ]\n", row, col, coord__from_xy(col, row).n);
                            }
                            else
                                printf("[E] it's player %u's turn!\n", board->state);
                            
                            
                        }
                        else
                            printf("[E] tile at [ x = %d | y = %d | n = %d ] is free\n", row, col, coord__from_xy(col, row).n);

                    }
                    else
                    {
                        to.x = col;                        
                        to.y = row;                        
                        to.n = coord__from_xy(col, row).n;
                        if (from.n != to.n)
                        {
                            printf("mark location at [ x = %d | y = %d | n = %d ] as destination\n", row, col, coord__from_xy(col, row).n);
                            piece_t* piece = piece__get_from_n(from.n, board);
                            u8 count = 0;
                            loc_node_t** chain = piece__possible_captures(piece, board, NULL, &count);
                            if (chain)
                            {
                                bool reached = false;
                                u8 max_depth = 0;
                                get_max_capture_depth(chain, count, &max_depth, 0);
                                capture(piece, board, chain, count, max_depth, 0, &reached);
                                piece__free_capture_chain(chain, count);
                                board->state = !board->state;
                                from.x = from.y = from.n = to.x = to.y = to.n = 0;
                                continue;
                            }
                            coord_vec_t* vec = piece__possible_moves(piece, board->indexes);
                            for (u8 i = 0; i < vec->len; ++i)
                            {
                                if (vec->table[i]->n == to.n)
                                {
                                    piece__move_piece(from, to, board);
                                    printf("piece was moved successfully\n");
                                    board->state = !board->state;
                                    break;
                                }
                            }
                            piece__free_coord_vec(vec);
                            
                            if (piece->coord.n != to.n)
                                printf("[E] cannot move piece: illegal move\n");
                                
                        }
                        else
                            printf("[E] cannot move piece: destination is equal to source\n");

                        from.x = from.y = from.n = to.x = to.y = to.n = 0;
                    }
                }
                
                else
                {
                    from.x = from.y = from.n = to.x = to.y = to.n = 0;
                    printf("[E] white tiles are discarded\n");
                }
            }
        }

        SDL_SetRenderDrawColor((SDL_Renderer*)ui__get_renderer(), 0, 0, 0, 255);
        SDL_RenderClear((SDL_Renderer*)ui__get_renderer());

        ui__draw_board();
        ui__draw_pieces(board);
        ui__draw_hints(from, board, ui__draw_capture_hints(from, board));
        SDL_RenderPresent((SDL_Renderer*)ui__get_renderer());
        SDL_Delay(16);
    }
}