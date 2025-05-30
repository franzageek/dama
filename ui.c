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
                SDL_SetRenderDrawColor(renderer, 181, 136, 99, 255); // dark
            
            else
                SDL_SetRenderDrawColor(renderer, 240, 217, 181, 255); // light

            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void ui__loop(void)
{
    bool running = 1;
    SDL_Event e;

    while (running)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
                running = 0;

            else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            {
                int x = e.button.x;
                int y = e.button.y;
                int col = x / CELL_SIZE;
                int row = y / CELL_SIZE;
                if ((col % 2 == 0 && row %2 == 0) || (col % 2 != 0 && row % 2 != 0))
                    printf("click[%d:%d:%d]\n", row, col, coord__from_xy(col, row).n);
                
                else
                    printf("click[%d:%d] on white tile\n", row, col);
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        ui__draw_board();

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