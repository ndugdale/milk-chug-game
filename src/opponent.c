#include "opponent.h"

#include <stdbool.h>

#include "events.h"
#include "player.h"

Opponent* opponent_create(uint64_t drink_duration) {
    Opponent* self = (Opponent*)malloc(sizeof(Opponent));
    self->start_time = 0;
    self->drink_duration = drink_duration;
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
        uint64_t current_time = SDL_GetTicks64();
        uint64_t elapsed_time = current_time - self->start_time;

        self->milk_consumed = MILK_CAPACITY *
                              elapsed_time /
                              self->drink_duration;

        if (self->milk_consumed >= MILK_CAPACITY) {
            self->finished = true;
            SDL_Log("Opponent finished in %d ms", self->drink_duration);
        }
    }
}
bool opponent_is_finished(Opponent* self) {
    return self->finished;
}

void opponent_destroy(Opponent* self) {
    free(self);
}
