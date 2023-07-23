#include "game.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "input.h"
#include "render.h"
#include "stage.h"

void gameInit(Game* self) {
    const int windowFlags = 0;
    const int rendererFlags = SDL_RENDERER_ACCELERATED;
    const int imageFlags = IMG_INIT_PNG | IMG_INIT_JPG;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Failed to initialise SDL: %s\n", SDL_GetError());
    }

    self->window = SDL_CreateWindow(
        "Milk Chug",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        BACKGROUND_WIDTH * RENDERER_SCALE_FACTOR,
        BACKGROUND_HEIGHT * RENDERER_SCALE_FACTOR,
        windowFlags
    );

    if (!self->window) {
        SDL_Log(
            "Failed to open %d x %d window: %s\n",
            BACKGROUND_WIDTH * RENDERER_SCALE_FACTOR,
            BACKGROUND_HEIGHT * RENDERER_SCALE_FACTOR,
            SDL_GetError()
        );
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    self->renderer = SDL_CreateRenderer(self->window, -1, rendererFlags);

    if (!self->renderer) {
        SDL_Log("Failed to create renderer: %s\n", SDL_GetError());
    }

    if (IMG_Init(imageFlags) != (imageFlags)) {
        printf("Failed to initialise SDL image: %s\n", IMG_GetError());
    }

    SDL_RenderSetScale(
        self->renderer, RENDERER_SCALE_FACTOR, RENDERER_SCALE_FACTOR
    );

    Stage stage;
    self->currentStage = &stage;
    stageInit(self->currentStage, self->renderer);
}

void gameLoop(Game* self) {
    gameHandleInput(self);
    gameRender(self);
    SDL_Delay(16);
}

void gameDestroy(Game* self) {
    SDL_DestroyWindow(self->window);
    SDL_DestroyRenderer(self->renderer);
    SDL_Quit();
}
