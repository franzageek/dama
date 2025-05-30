#include "ui.h"
#include "coord.h"
#include <SDL3/SDL.h>
#include <stdio.h>

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

bool ui__SDL3_init(void)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "[SDL3 error] SDL_Init(): \'%s\'\n", SDL_GetError());
        return false;    
    }

    window = SDL_CreateWindow("dama v0.1", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window)
    {
        fprintf(stderr, "[SDL3 error] SDL_CreateWindow(): \'%s\'\n", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer)
    {
        fprintf(stderr, "[SDL3 error] SDL_CreateRenderer(): \'%s\'\n", SDL_GetError());
        return false;
    }

    return true;
}

void ui__draw_board(void)
{
    for (u8 row = 0; row < BOARD_SIZE; row++)
    {
        for (u8 col = 0; col < BOARD_SIZE; col++)
        {
            SDL_FRect rect = 
            {
                .x = (float)(col * CELL_SIZE), 
                .y = (float)(row * CELL_SIZE),
                .w = (float)CELL_SIZE,
                .h = (float)CELL_SIZE,
            };

            if ((row + col) % 2 == 0)
                SDL_SetRenderDrawColor(renderer, 181-60, 136-60, 99-60, 255); // dark
            
            else
                SDL_SetRenderDrawColor(renderer, 255, 220, 150, 255); // light

            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void render_point(SDL_Color color, u16 cx, u16 cy, u16 radius)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (u16 w = 0; w < radius * 2; w++)
    {
        for (u16 h = 0; h < radius * 2; h++)
        {
            int dx = radius - w;
            int dy = radius - h;
            if (dx * dx + dy * dy <= radius * radius)
                SDL_RenderPoint(renderer, cx + dx, cy + dy);

        }
    }
}

void ui__draw_pieces(board_t* board)
{
    for (u8 i = 0; i < 32; ++i)
    {
        if (board->indexes[i] > 0)
        {
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            u16 col = coord__from_n(i+1).x;
            u16 row = coord__from_n(i+1).y;
            u16 cx = col * CELL_SIZE + CELL_SIZE / 2;
            u16 cy = row * CELL_SIZE + CELL_SIZE / 2;
            u16 radius = CELL_SIZE / 2 - 6;

            SDL_Color color;
            if (piece__get_from_n(i+1, board)->valid)
            {
                if (!piece__get_from_n(i+1, board)->king)
                {
                    if (piece__get_from_n(i+1, board)->player)
                    {
                        color = (SDL_Color){255, 255, 230, 200};
                        render_point(color, cx, cy, radius);
                    }
                    else    
                    {
                        color = (SDL_Color){0, 0, 0, 150};
                        render_point(color, cx, cy, radius);
                    }
                }
                else
                {
                    if (piece__get_from_n(i+1, board)->player)
                    {
                        color = (SDL_Color){255, 255, 230, 200};
                        render_point(color, cx, cy, radius);
                        color = (SDL_Color){181-60, 136-60, 99-60, 255};
                        //render_point(color, cx, cy, radius-4);
                        //color = (SDL_Color){255, 255, 230, 150};
                        render_point(color, cx, cy, radius-10);
                        color = (SDL_Color){255, 255, 230, 200};
                        render_point(color, cx, cy, radius-20);
                    }
                    else    
                    {
                        color = (SDL_Color){0, 0, 0, 150};
                        render_point(color, cx, cy, radius);
                        color = (SDL_Color){181-60, 136-60, 99-60, 255};
                        render_point(color, cx, cy, radius-10);
                        color = (SDL_Color){0, 0, 0, 150};
                        render_point(color, cx, cy, radius-20);
                    }
                }
            }
        }
    }
    return;
}

void ui__loop(board_t* board)
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
                                printf("[E] player %u should move!\n", board->state);
                            
                            
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
                            if (piece->coord.n != to.n)
                                printf("[E] cannot move piece: illegal move\n");
                                
                            piece__free_coord_vec(vec);
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

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        ui__draw_board();
        ui__draw_pieces(board);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}

void ui__quit_SDL3(void)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}