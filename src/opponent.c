#include "opponent.h"

#include <stdbool.h>

#include "events.h"
#include "player.h"

Opponent* opponent_create(uint32_t finish_time_ms) {
    Opponent* self = (Opponent*)malloc(sizeof(Opponent));
    self->finish_time_ms = finish_time_ms;
    self->finished = false;
    self->milk_consumed = 0;

    return self;
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

        if (self->milk_consumed >= MILK_CAPACITY) {
            self->finished = true;
            SDL_Log("Opponent finished in %d ms", self->finish_time_ms);
        }
    }
}
bool opponent_is_finished(Opponent* self) {
    return self->finished;
}

void opponent_destroy(Opponent* self) {
    free(self);
}
