#pragma once

#include <SDL2/SDL.h>

typedef struct {
    SDL_Texture* background;
} Stage;

void stageInit(Stage* self, SDL_Renderer* renderer);
