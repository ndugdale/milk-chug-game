#pragma once

#include <SDL.h>

#include "audio.h"
#include "font.h"
#include "player.h"
#include "scene.h"
#include "texture.h"

#define BACKGROUND_WIDTH 480
#define BACKGROUND_HEIGHT 270
#define RENDERER_SCALE_FACTOR 8

#define TARGET_FRAME_TIME_WHOLE 16
#define TARGET_FRAME_TIME_FRACTION 0.667

typedef struct {
    SDL_Renderer* renderer;
    SDL_Window* window;
    Scene* scene;
    Player* player;
    EventQueue* event_queue;
    AudioManager* audio_manager;
    FontManager* font_manager;
    TextureManager* texture_manager;
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
