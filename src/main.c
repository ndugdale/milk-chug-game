#include <SDL2/SDL.h>
#include <stdio.h>

#include "game.h"

int main(int argc, char *argv[]) {
    Game game;
    gameInit(&game);

    SDL_Event event;
    while (1) {
        gameLoop(&game);
    }

    gameDestroy(&game);
    return 0;
}
