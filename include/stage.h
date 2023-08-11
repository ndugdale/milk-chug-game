#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "events.h"
#include "opponent.h"
#include "player.h"

#define NUM_OPPONENTS 4
#define INDICATOR_VERTICAL_OFFSET 4

typedef struct {
    Player* player;
    Opponent* opponents[NUM_OPPONENTS];
    SDL_Texture* background;
    bool complete;
    uint64_t min_drink_duration;
    uint64_t max_drink_duration;
} Stage;

Stage* stage_create(SDL_Renderer* renderer, Player* player);
void stage_update(Stage* self, Event event);
void stage_render(Stage* self, SDL_Renderer* renderer, SDL_Window* window);
void stage_destroy(Stage* self);
bool stage_is_complete(const Stage* self);
