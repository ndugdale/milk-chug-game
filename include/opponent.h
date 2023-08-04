#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "events.h"

typedef struct Opponent {
    bool finished;
    uint16_t milk_consumed;
    uint64_t start_time;
    uint64_t drink_duration;
} Opponent;

Opponent* opponent_create(uint64_t drink_duration);
void opponent_update(Opponent* self, Event event);
void opponent_destroy(Opponent* self);
bool opponent_is_finished(Opponent* self);
