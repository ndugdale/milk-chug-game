#pragma once

#include <SDL2/SDL.h>

#include "events.h"
#include "player.h"
#include "stage.h"

#define BACKGROUND_WIDTH 480
#define BACKGROUND_HEIGHT 270
#define RENDERER_SCALE_FACTOR 4

typedef struct {
    SDL_Renderer* renderer;
    SDL_Window* window;
    Player* player;
    Stage* current_stage;
    EventQueue* event_queue;
} Game;

Game* game_create(void);
void game_loop(Game* self);
void game_handle_input(Game* self);
void game_update(Game* self);
void game_render(Game* self);
void game_destroy(Game* self);
