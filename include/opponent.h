#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "events.h"

typedef struct Opponent {
    bool finished;
    uint8_t milk_consumed;
    uint32_t finish_time_ms;
} Opponent;

Opponent* opponent_create(uint32_t finish_time_ms);
void opponent_update(Opponent* self, Event event);
void opponent_drink(Opponent* self);
bool opponent_is_finished(Opponent* self);
void opponent_destroy(Opponent* self);
