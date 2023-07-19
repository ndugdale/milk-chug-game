#pragma once

#include <SDL2/SDL.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

typedef struct {
    SDL_Renderer* renderer;
    SDL_Window* window;
} Game;

void gameInit(Game* self);
void gameDestroy(Game* self);
