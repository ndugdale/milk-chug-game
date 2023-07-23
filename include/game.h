#pragma once

#include <SDL2/SDL.h>

#include "input.h"
#include "stage.h"

#define BACKGROUND_WIDTH 480
#define BACKGROUND_HEIGHT 270
#define RENDERER_SCALE_FACTOR 4

typedef struct {
    SDL_Renderer* renderer;
    SDL_Window* window;
    Stage* currentStage;
    InputEventQueue* inputEventQueue;
} Game;

void gameInit(Game* self);
void gameLoop(Game* self);
void gameHandleInput(Game* self);
void gameDestroy(Game* self);
