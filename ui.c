#include "ui.h"
#include "coord.h"
#include <SDL3/SDL.h>
#include <stdio.h>

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

/*SDL_Color light_tile = {255, 220, 150, 255};
SDL_Color dark_tile = {181-60, 136-60, 99-60, 255};

SDL_Color light_piece = {255, 255, 230, 200};
SDL_Color dark_piece = {0, 0, 0, 150};*/

/*SDL_Color light_piece = {248,112,96, 255};
SDL_Color dark_piece = {179, 163, 148, 255};

SDL_Color light_tile = {205, 215, 214, 255};
SDL_Color dark_tile = {16,37,66, 255};*/

SDL_Color light_piece = {244,211,94, 255};
SDL_Color dark_piece = {218, 65, 103, 255};

SDL_Color light_tile = {235, 235, 211, 255};
SDL_Color dark_tile = {8,61,119, 255};

bool ui__SDL3_init(void)
{
    printdb("starting SDL3 initialization...\n");
    printdb("initializing video...");
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "[SDL3 error] SDL_Init(): \'%s\'\n", SDL_GetError());
        return false;    
    }
    printdb("done\n");
    printdb("initializing window...");
    window = SDL_CreateWindow("dama v0.1", WINDOW_WIDTH, WINDOW_HEIGHT, 0|SDL_WINDOW_HIGH_PIXEL_DENSITY|SDL_WINDOW_VULKAN);
    if (!window)
    {
        fprintf(stderr, "[SDL3 error] SDL_CreateWindow(): %s\n", SDL_GetError());
        return false;
    }
    printdb("created window <%p> of size %ux%u\n", window, WINDOW_WIDTH, WINDOW_HEIGHT);

    printdb("initializing renderer...");
    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer)
    {
        fprintf(stderr, "[SDL3 error] SDL_CreateRenderer(): %s\n", SDL_GetError());
        return false;
    }
    printdb("created renderer <%p>\n", renderer);
    printdb("SDL3 initialization completed\n");
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
                SDL_SetRenderDrawColor(renderer, dark_tile.r, dark_tile.g, dark_tile.b, dark_tile.a); // dark
            
            else
                SDL_SetRenderDrawColor(renderer, light_tile.r, light_tile.g, light_tile.b, light_tile.a); // light

            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void render_circle(SDL_Color color, u16 cx, u16 cy, i32 radius)
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
    return;
}

void draw_rect_at_xy(coord_t coord, SDL_Color color)
{
    SDL_FRect rect = 
    {
        .x = coord.x * CELL_SIZE,
        .y = coord.y * CELL_SIZE,
        .w = CELL_SIZE,
        .h = CELL_SIZE
    };
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
    return;
}

void draw_circle_at_xy(coord_t coord, u16 radius, SDL_Color color)
{
    u16 col = coord.x;
    u16 row = coord.y;
    u16 cx = col * CELL_SIZE + CELL_SIZE / 2;
    u16 cy = row * CELL_SIZE + CELL_SIZE / 2;
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    render_circle(color, cx, cy, radius);
}

void ui__draw_hints(coord_t coord, board_t* board, bool capture_available)
{
    if (!coord__is_null(coord) && !capture_available)
    {
        piece_t* piece = piece__get_from_n(coord.n, board);
        coord_vec_t* vec = piece__possible_moves(piece, board->indexes);
        for (u8 i = 0; i < vec->len; ++i)
        {
            u16 radius = CELL_SIZE / 2 - 6;
            SDL_Color color = (SDL_Color){100, 200, 200, 200};
            draw_circle_at_xy(coord__from_xy(vec->table[i]->x, vec->table[i]->y), radius, color);
        }
        piece__free_coord_vec(vec);
    }
    return;
}

void draw_capture_hint_circle(loc_node_t** chain, u8 count)
{
    if (chain)
    {
        for (u8 i = 0; i < count; ++i)
        {
            if (chain[i]->next)
                draw_capture_hint_circle(chain[i]->next, chain[i]->count);
    
            u16 radius = CELL_SIZE / 2 - 2;
            SDL_Color color = (SDL_Color){200, 200, 100, 100};
            draw_rect_at_xy(chain[i]->capt, color);
            radius = CELL_SIZE / 2 - 6;
            color = (SDL_Color){255, 100, 50, 255};
            draw_circle_at_xy(chain[i]->capt, radius, color);
            radius = CELL_SIZE / 2 - 6;
            color = (SDL_Color){200, 200, 100, 100};
            draw_circle_at_xy(chain[i]->dest, radius, color);
        }
    }
    return;
}

bool ui__draw_capture_hints(coord_t coord, board_t* board)
{
    if (!coord__is_null(coord))
    {
        piece_t* piece = piece__get_from_n(coord.n, board);
        u8 count = 0;
        loc_node_t** chain = piece__possible_captures(piece, board, NULL, &count, 4);
        if (chain)
        {
            draw_capture_hint_circle(chain, count);
            piece__free_capture_chain(chain, count);
            return true;
        }
    }
    return false;
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
                        render_circle(light_piece, cx, cy, radius);
                    }
                    else    
                    {
                        render_circle(dark_piece, cx, cy, radius);
                    }
                }
                else
                {
                    if (piece__get_from_n(i+1, board)->player)
                    {
                        render_circle(light_piece, cx, cy, radius);
                        //render_circle(color, cx, cy, radius-4);
                        //color = (SDL_Color){255, 255, 230, 150};
                        render_circle(dark_tile, cx, cy, radius-10);
                        render_circle(light_piece, cx, cy, radius-20);
                    }
                    else    
                    {
                        render_circle(dark_piece, cx, cy, radius);
                        render_circle(dark_tile, cx, cy, radius-10);
                        render_circle(dark_piece, cx, cy, radius-20);
                    }
                }
            }
        }
    }
    return;
}

void ui__quit_SDL3(void)
{
    printdb("freeing SDL3 resources...");
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    printdb("done\n");
    return;
}

void* ui__get_renderer(void)
{
    return renderer;
}

void* ui__get_window(void)
{
    return window;
}