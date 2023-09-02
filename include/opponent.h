#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "events.h"
#include "indicator.h"
#include "texture.h"

#define MAX_OPPONENT_NAME_CHARS 100
#define MAX_FILENAME_LENGTH 100

typedef struct Opponent {
    char name[MAX_OPPONENT_NAME_CHARS];
    bool finished;
    uint16_t milk_consumed;
    uint64_t start_time;
    uint64_t drink_duration;
    SDL_Texture* sprite_sheet;
    uint8_t sprite;
    size_t index;
    uint64_t x;
    uint64_t y;
    Indicator* indicator;
} Opponent;

Opponent* opponent_create(
    SDL_Renderer* renderer, TextureManager* texture_manager, const char* name,
    const char* texture_id, uint64_t drink_duration, uint64_t x, uint64_t y
);
void opponent_update(Opponent* self, Event event);
void opponent_render(Opponent* self, SDL_Renderer* renderer, SDL_Window* window);
void opponent_destroy(Opponent* self);
