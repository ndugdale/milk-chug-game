#pragma once

#include <SDL2/SDL.h>
#include <stdint.h>

#include "game.h"
#include "stage.h"

SDL_Texture* load_texture(SDL_Renderer* renderer, const char* filename);
void blit_sprite(
    SDL_Renderer* renderer, SDL_Texture* sprite_sheet, uint8_t row, uint8_t col,
    uint8_t dest_x, uint8_t dest_y, uint32_t w, uint32_t h
);
