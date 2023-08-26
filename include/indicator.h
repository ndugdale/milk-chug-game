#pragma once

#include <SDL2/SDL.h>

#include "events.h"
#include "texture.h"

#define INDICATOR_SPRITE_WIDTH 16
#define INDICATOR_SPRITE_HEIGHT 20
#define INDICATOR_SPRITE_NUM 17

typedef struct {
    SDL_Texture* sprite_sheet;
    uint8_t sprite;
    int64_t x;
    int64_t y;
} Indicator;

Indicator* indicator_create(
    SDL_Renderer* renderer, TextureManager* texture_manager, int64_t x,
    int64_t y
);
void indicator_update(
    Indicator* self, Event event, uint16_t capacity, uint16_t max
);
void indicator_render(
    Indicator* self, SDL_Renderer* renderer, SDL_Window* window
);
void indicator_destroy(Indicator* self);
