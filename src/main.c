#include <stdio.h>

#include "game.h"

uint32_t main(uint32_t argc, char* argv[]) {
    Game* game = game_create();

    while (1) {
        game_loop(game);
    }
    game_destroy(game);

    return 0;
}
