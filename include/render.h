#pragma once

#include <SDL2/SDL.h>
#include <stdint.h>

#include "game.h"
#include "stage.h"

void blit_background(
    SDL_Renderer* renderer, SDL_Window* window, SDL_Texture* background,
    double scale, uint32_t w, uint32_t h
);
void blit_sprite(
    SDL_Renderer* renderer, SDL_Texture* sprite_sheet, uint8_t row, uint8_t col,
    int64_t window_x, int64_t window_y, uint32_t w, uint32_t h
);
// Convert between local and window coordinates:
// - local: origin at top-left of background texture
// - window: origin at top-left of window
void local_xy_to_window_xy(
    SDL_Window* window, int64_t local_x, int64_t local_y,
    uint32_t background_w, uint32_t background_h,
    int64_t* window_x, int64_t* window_y
);
