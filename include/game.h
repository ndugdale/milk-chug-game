#pragma once

#include <SDL2/SDL.h>

#include "stage.h"

#define BACKGROUND_WIDTH 480
#define BACKGROUND_HEIGHT 270
#define RENDERER_SCALE_FACTOR 4

typedef struct {
    SDL_Renderer* renderer;
    SDL_Window* window;
    Stage* currentStage;
} Game;

void gameInit(Game* self);
void gameLoop(Game* self);
void gameDestroy(Game* self);
