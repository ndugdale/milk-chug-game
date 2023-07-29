#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "events.h"

#define MILK_CAPACITY 4000
#define MILK_SIP 100

typedef struct {
    bool finished;
    uint16_t milk_consumed;
    uint64_t drink_duration;
} Player;

Player* player_create(void);
void player_reset(Player* self);
void player_update(Player* self, Event event);
void player_drink(Player* self);
bool player_is_finished(Player* self);
void player_destroy(Player* self);
