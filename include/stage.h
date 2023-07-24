#pragma once

#include <SDL2/SDL.h>

#include "input.h"
#include "player.h"

typedef struct {
    Player* player;
    SDL_Texture* background;
} Stage;

Stage* stage_create(SDL_Renderer* renderer, Player* player);
void stage_update(Stage* self, InputEvent event);
void stage_render(Stage* self, SDL_Renderer* renderer, SDL_Window* window);
void stage_destroy(Stage* self);
