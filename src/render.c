#include "render.h"

#include <SDL2/SDL.h>

#include "game.h"

static void prepareScene(Game* self);
static void presentScene(Game* self);

void gameRender(Game* self) {
    prepareScene(self);
    presentScene(self);
}

static void prepareScene(Game* self) {
    SDL_SetRenderDrawColor(self->renderer, 0, 0, 0, 255);
    SDL_RenderClear(self->renderer);
}

static void presentScene(Game* self) {
    SDL_RenderPresent(self->renderer);
}
