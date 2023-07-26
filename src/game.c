#include "game.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>
#include <stdlib.h>

#include "events.h"
#include "player.h"
#include "stage.h"

static void on_key_down(Game* self, SDL_KeyboardEvent* event);
static void on_key_up(Game* self, SDL_KeyboardEvent* event);

Game* game_create(void) {
    Game* self = (Game*)calloc(1, sizeof(Game));

    const uint32_t window_flags = 0;
    const uint32_t renderer_flags = SDL_RENDERER_ACCELERATED;
    const uint32_t image_flags = IMG_INIT_PNG | IMG_INIT_JPG;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Failed to initialise SDL: %s\n", SDL_GetError());
    }

    self->window = SDL_CreateWindow(
        "Milk Chug",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        BACKGROUND_WIDTH * RENDERER_SCALE_FACTOR,
        BACKGROUND_HEIGHT * RENDERER_SCALE_FACTOR,
        window_flags
    );

    if (!self->window) {
        SDL_Log(
            "Failed to open %d x %d window: %s\n",
            BACKGROUND_WIDTH * RENDERER_SCALE_FACTOR,
            BACKGROUND_HEIGHT * RENDERER_SCALE_FACTOR,
            SDL_GetError()
        );
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    self->renderer = SDL_CreateRenderer(self->window, -1, renderer_flags);

    if (!self->renderer) {
        SDL_Log("Failed to create renderer: %s\n", SDL_GetError());
    }

    if (IMG_Init(image_flags) != (image_flags)) {
        printf("Failed to initialise SDL image: %s\n", IMG_GetError());
    }

    SDL_RenderSetScale(
        self->renderer, RENDERER_SCALE_FACTOR, RENDERER_SCALE_FACTOR
    );

    self->player = player_create();
    self->current_stage = stage_create(self->renderer, self->player);
    self->event_queue = event_queue_create();

    return self;
}

void game_loop(Game* self) {
    game_handle_input(self);
    game_update(self);
    game_render(self);
    SDL_Delay(16);
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
        stage_update(self->current_stage, event);

        switch (event) {
            default:
                break;
        }
    }
}

void game_render(Game* self) {
    SDL_SetRenderDrawColor(self->renderer, 0, 0, 0, 255);
    SDL_RenderClear(self->renderer);
    stage_render(self->current_stage, self->renderer, self->window);

    SDL_RenderPresent(self->renderer);
}

void game_destroy(Game* self) {
    event_queue_destroy(self->event_queue);
    stage_destroy(self->current_stage);
    player_destroy(self->player);

    SDL_DestroyWindow(self->window);
    SDL_DestroyRenderer(self->renderer);
    SDL_Quit();

    free(self);
}

static void on_key_down(Game* self, SDL_KeyboardEvent* event) {
    if (event->repeat == 0 && event->keysym.scancode < SDL_NUM_SCANCODES) {
        switch (event->keysym.scancode) {
            case DRINK_MILK_INPUT:
                event_queue_enqueue(self->event_queue, EVENT_DRINK);
                break;
            default:
                break;
        }
    }
}

static void on_key_up(Game* self, SDL_KeyboardEvent* event) {
    if (event->repeat == 0 && event->keysym.scancode < SDL_NUM_SCANCODES) {
        switch (event->keysym.scancode) {
            default:
                break;
        }
    }
}
