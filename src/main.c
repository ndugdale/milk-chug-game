#include <stdio.h>
#include <time.h>

#include "game.h"

uint32_t main(uint32_t argc, char* argv[]) {
    srand(time(NULL));
    Game* game = game_create();

    while (1) {
        game_loop(game);
    }
    game_destroy(game);

    return 0;
}
