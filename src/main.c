#include <SDL2/SDL.h>
#include <stdio.h>

#include "game.h"

int main(int argc, char *argv[]) {
    Game game;
    gameInit(&game);

    SDL_Event event;
    while (1) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    exit(0);
                    break;
                default:
                    break;
            }
        }
        SDL_Delay(16);
    }

    gameDestroy(&game);
    return 0;
}
