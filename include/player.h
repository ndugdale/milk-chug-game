#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "events.h"

#define MILK_CAPACITY 40

typedef struct {
    bool finished;
    uint8_t milk_consumed;
} Player;

Player* player_create(void);
void player_reset(Player* self);
void player_update(Player* self, Event event);
void player_drink(Player* self);
bool player_is_finished(Player* self);
void player_destroy(Player* self);
