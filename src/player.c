#include "player.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "events.h"

Player* player_create(void) {
    Player* self = (Player*)malloc(sizeof(Player));
    player_reset(self);

    return self;
}

void player_reset(Player* self) {
    self->finished = false;
    self->milk_consumed = 0;
    self->drink_duration = UINT64_MAX;
}

void player_update(Player* self, Event event) {
    switch (event) {
        case EVENT_DRINK:
            player_drink(self);
        default:
            break;
    }
}

void player_drink(Player* self) {
    if (!self->finished) {
        self->milk_consumed += MILK_SIP;
        SDL_Log("GLUG: %d/%d", self->milk_consumed, MILK_CAPACITY);

        if (self->milk_consumed >= MILK_CAPACITY) {
            self->finished = true;
            SDL_Log("You finished");
        }
    }
}

bool player_is_finished(Player* self) {
    return self->finished;
}

void player_destroy(Player* self) {
    free(self);
}
