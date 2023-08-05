#pragma once

#include <SDL2/SDL.h>

#include "events.h"
#include "player.h"
#include "stage.h"

#define BACKGROUND_WIDTH 320
#define BACKGROUND_HEIGHT 180
#define RENDERER_SCALE_FACTOR 8

#define TARGET_FRAME_TIME_WHOLE 16
#define TARGET_FRAME_TIME_FRACTION 0.667

typedef struct {
    SDL_Renderer* renderer;
    SDL_Window* window;
    Player* player;
    Stage* current_stage;
    EventQueue* event_queue;
    uint64_t last_frame_time;
    double remainder_time;
} Game;

Game* game_create(void);
void game_loop(Game* self);
void game_handle_input(Game* self);
void game_update(Game* self);
void game_render(Game* self);
void game_frame_rate_limit(Game* self);
void game_destroy(Game* self);
