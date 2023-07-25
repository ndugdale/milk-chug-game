#include "opponent.h"

#include <stdbool.h>

#include "events.h"
#include "player.h"

Opponent* opponent_create(void) {
    Opponent* self = (Opponent*)malloc(sizeof(Opponent));
    opponent_reset(self);

    return self;
}

void opponent_reset(Opponent* self) {
    self->finished = false;
    self->milk_consumed = 0;
}

void opponent_update(Opponent* self, Event event) {
    switch (event) {
        case EVENT_TICK:
            opponent_drink(self);
        default:
            break;
    }
}

void opponent_drink(Opponent* self) {
    if (!self->finished) {
        self->milk_consumed++;
        SDL_Log("Opponent drinks");

        if (self->milk_consumed >= MILK_CAPACITY) {
            self->finished = true;
            SDL_Log("Opponent finished");
        }
    }
}
bool opponent_is_finished(Opponent* self) {
    return self->finished;
}

void opponent_destroy(Opponent* self) {
    free(self);
}
