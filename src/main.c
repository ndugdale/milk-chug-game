#include <stdio.h>

#include "game.h"

int main(int argc, char* argv[]) {
    Game* game = (Game*)calloc(1, sizeof(Game));
    gameInit(game);

    while (1) {
        gameLoop(game);
    }

    gameDestroy(game);
    free(game);

    return 0;
}
