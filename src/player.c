#include "player.h"

#include <stdbool.h>
#include <stdlib.h>

#include "input.h"

Player* player_create(void) {
    Player* self = (Player*)malloc(sizeof(Player));
    player_reset(self);

    return self;
}

void player_reset(Player* self) {
    self->finished = false;
    self->milk_consumed = 0;
}

void player_update(Player* self, InputEvent event) {
    switch (event) {
        case EVENT_DRINK:
            player_drink(self);
        default:
            break;
    }
}

void player_drink(Player* self) {
    if (!self->finished) {
        self->milk_consumed++;

        if (self->milk_consumed >= MILK_CAPACITY) {
            self->finished = true;
        }
        SDL_Log("GLUG: %d/%d", self->milk_consumed, MILK_CAPACITY);
    }
}

bool player_is_finished(Player* self) {
    return self->finished;
}

void player_destroy(Player* self) {
    free(self);
}
