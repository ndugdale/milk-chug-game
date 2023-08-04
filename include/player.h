#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>

#include "events.h"

#define MILK_CAPACITY 4000
#define MILK_SIP 100

typedef struct {
    bool finished;
    uint16_t milk_consumed;
    uint64_t drink_duration;
    SDL_Texture* texture;

} Player;

Player* player_create(SDL_Renderer* renderer);
void player_update(Player* self, Event event);
void player_destroy(Player* self);
bool player_is_finished(Player* self);
