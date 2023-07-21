#include "input.h"

#include <SDL2/SDL.h>

#include "game.h"

void gameHandleInput(Game* self) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                exit(0);
                break;
            default:
                break;
        }
    }
}
