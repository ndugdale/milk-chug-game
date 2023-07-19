#include "game.h"

#include <SDL2/SDL.h>

void gameInit(Game* self) {
    int rendererFlags;
    int windowFlags;

    rendererFlags = SDL_RENDERER_ACCELERATED;
    windowFlags = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Failed to initialise SDL: %s\n", SDL_GetError());
    }

    self->window = SDL_CreateWindow(
        "Milk Chug",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        windowFlags
    );

    if (!self->window) {
        SDL_Log("Failed to open %d x %d window: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    self->renderer = SDL_CreateRenderer(self->window, -1, rendererFlags);

    if (!self->renderer) {
        SDL_Log("Failed to create renderer: %s\n", SDL_GetError());
    }
}

void gameDestroy(Game* self) {
    SDL_DestroyWindow(self->window);
    SDL_DestroyRenderer(self->renderer);
    SDL_Quit();
}
