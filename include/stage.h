#pragma once

#include <SDL2/SDL.h>

typedef struct {
    SDL_Texture* background;
} Stage;

Stage* stage_create(SDL_Renderer* renderer);
void stage_render(Stage* self, SDL_Renderer* renderer, SDL_Window* window);
void stage_destroy(Stage* self);
