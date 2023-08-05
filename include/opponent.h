#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "events.h"

#define MAX_FILENAME_LENGTH 100

typedef struct Opponent {
    bool finished;
    uint16_t milk_consumed;
    uint64_t start_time;
    uint64_t drink_duration;
    SDL_Texture* sprite_sheet;
    uint8_t sprite;
    size_t index;
} Opponent;

Opponent* opponent_create(
    SDL_Renderer* renderer, uint64_t drink_duration, size_t index
);
void opponent_update(Opponent* self, Event event);
void opponent_render(Opponent* self, SDL_Renderer* renderer);
void opponent_destroy(Opponent* self);
bool opponent_is_finished(const Opponent* self);
