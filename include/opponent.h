#pragma once

#include <stdbool.h>

#include "events.h"

typedef struct Opponent {
    bool finished;
    int milk_consumed;
} Opponent;

Opponent* opponent_create(void);
void opponent_reset(Opponent* self);
void opponent_update(Opponent* self, Event event);
void opponent_drink(Opponent* self);
bool opponent_is_finished(Opponent* self);
void opponent_destroy(Opponent* self);
