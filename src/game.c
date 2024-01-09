#include "game.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdint.h>
#include <stdlib.h>

#include "audio.h"
#include "events.h"
#include "font.h"
#include "player.h"
#include "stage.h"
#include "texture.h"

static void on_key_down(Game* self, const SDL_KeyboardEvent* event);
static void on_key_up(Game* self, const SDL_KeyboardEvent* event);

Game* game_create(void) {
    Game* self = malloc(sizeof(Game));

    const uint32_t window_flags = 0;
    const uint32_t renderer_flags = SDL_RENDERER_ACCELERATED;
    const uint32_t image_flags = IMG_INIT_PNG | IMG_INIT_JPG;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "Failed to initialise SDL: %s\n",
            SDL_GetError()
        );
        exit(1);
    }

    SDL_DisplayMode display_mode;
    if (SDL_GetDesktopDisplayMode(0, &display_mode) != 0) {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "Failed to get desktop display mode: %s",
            SDL_GetError()
        );
        exit(1);
    }

    self->window = SDL_CreateWindow(
        "Milk Chug",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        display_mode.w,
        display_mode.h,
        window_flags
    );

    if (!self->window) {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "Failed to open %d x %d window: %s\n",
            display_mode.w,
            display_mode.h,
            SDL_GetError()
        );
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    self->renderer = SDL_CreateRenderer(self->window, -1, renderer_flags);

    if (!self->renderer) {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "Failed to create renderer: %s\n",
            SDL_GetError()
        );
        exit(1);
    }

    if (SDL_RenderSetScale(self->renderer, RENDERER_SCALE_FACTOR, RENDERER_SCALE_FACTOR) != 0) {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "Failed to set render scale: %s\n",
            SDL_GetError()
        );
        exit(1);
    };

    if (IMG_Init(image_flags) != (image_flags)) {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "Failed to initialise SDL image: %s\n",
            IMG_GetError()
        );
        exit(1);
    }

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, MIX_CHUNK_SIZE) != 0) {
        SDL_LogError(
            SDL_LOG_CATEGORY_AUDIO,
            "Failed to open audio: %s\n",
            Mix_GetError()
        );
        exit(1);
    }

    if (TTF_Init() != 0) {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "Failed to initialise SDL ttf: %s\n",
            TTF_GetError()
        );
        exit(1);
    }

    self->audio_manager = audio_manager_create();
    self->font_manager = font_manager_create();
    self->texture_manager = texture_manager_create(self->renderer);

    const int64_t player_x = (BACKGROUND_WIDTH - PLAYER_SPRITE_WIDTH) / 2;
    const int64_t player_y = (BACKGROUND_HEIGHT + PLAYER_SPRITE_HEIGHT) / 2;

    self->player = player_create(
        self->audio_manager, self->texture_manager,
        "Nate Chersbrew", "player", player_x, player_y
    );
    self->scene = scene_create(self->audio_manager, self->font_manager, self->texture_manager, self->player);
    self->event_queue = event_queue_create();

    self->last_frame_time = 0;
    self->remainder_time = 0;

    audio_manager_play_music(self->audio_manager, "main_theme");

    return self;
}

void game_loop(Game* self) {
    game_handle_input(self);
    game_update(self);
    game_render(self);
    game_frame_rate_limit(self);
}

void game_handle_input(Game* self) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                exit(0);
                break;
            case SDL_KEYDOWN:
                on_key_down(self, &event.key);
                break;
            case SDL_KEYUP:
                on_key_up(self, &event.key);
                break;
            default:
                break;
        }
    }
}

void game_update(Game* self) {
    event_queue_enqueue(self->event_queue, EVENT_TICK);

    while (!event_queue_is_empty(self->event_queue)) {
        Event event = event_queue_dequeue(self->event_queue);
        scene_update(self->scene, event);

        switch (event) {
            case EVENT_TOGGLE_AUDIO:
                audio_manager_toggle(self->audio_manager);
            default:
                break;
        }
    }
}

void game_render(Game* self) {
    SDL_SetRenderDrawColor(self->renderer, 0, 0, 0, 255);
    SDL_RenderClear(self->renderer);
    scene_render(self->scene, self->renderer, self->window);
    SDL_RenderPresent(self->renderer);
}

void game_frame_rate_limit(Game* self) {
    uint64_t current_time = SDL_GetTicks64();
    uint64_t elapsed_time = current_time - self->last_frame_time;
    self->remainder_time += TARGET_FRAME_TIME_FRACTION;

    double adjusted_frame_remainder = (uint64_t)self->remainder_time;
    uint64_t adjusted_frame_whole = TARGET_FRAME_TIME_WHOLE +
        adjusted_frame_remainder;

    if (elapsed_time < adjusted_frame_whole) {
        SDL_Delay(adjusted_frame_whole - elapsed_time);
    }

    self->remainder_time -= (uint64_t)self->remainder_time;
    self->last_frame_time = SDL_GetTicks64();
}

void game_destroy(Game* self) {
    texture_manager_destroy(self->texture_manager);
    font_manager_destroy(self->font_manager);
    audio_manager_destroy(self->audio_manager);
    event_queue_destroy(self->event_queue);
    scene_destroy(self->scene);
    player_destroy(self->player);

    IMG_Quit();
    Mix_Quit();
    SDL_DestroyWindow(self->window);
    SDL_DestroyRenderer(self->renderer);
    SDL_Quit();

    free(self);
}

static void on_key_down(Game* self, const SDL_KeyboardEvent* event) {
    if (event->repeat == 0 && event->keysym.scancode < SDL_NUM_SCANCODES) {
        switch (event->keysym.scancode) {
            case ACTION_KEY:
                event_queue_enqueue(self->event_queue, EVENT_ACTION);
                break;
            case TOGGLE_AUDIO_KEY:
                event_queue_enqueue(self->event_queue, EVENT_TOGGLE_AUDIO);
                break;
            case QUIT_GAME_KEY:
                exit(0);
                break;
            default:
                break;
        }
    }
}

static void on_key_up(Game* self, const SDL_KeyboardEvent* event) {
    if (event->repeat == 0 && event->keysym.scancode < SDL_NUM_SCANCODES) {
        switch (event->keysym.scancode) {
            default:
                break;
        }
    }
}
