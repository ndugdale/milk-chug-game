#include "game.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "input.h"
#include "render.h"
#include "stage.h"
#include "update.h"

static void onKeyDown(Game* self, SDL_KeyboardEvent* event);
static void onKeyUp(Game* self, SDL_KeyboardEvent* event);

void gameInit(Game* self) {
    const int windowFlags = 0;
    const int rendererFlags = SDL_RENDERER_ACCELERATED;
    const int imageFlags = IMG_INIT_PNG | IMG_INIT_JPG;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Failed to initialise SDL: %s\n", SDL_GetError());
    }

    self->window = SDL_CreateWindow(
        "Milk Chug",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        BACKGROUND_WIDTH * RENDERER_SCALE_FACTOR,
        BACKGROUND_HEIGHT * RENDERER_SCALE_FACTOR,
        windowFlags
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
    self->renderer = SDL_CreateRenderer(self->window, -1, rendererFlags);

    if (!self->renderer) {
        SDL_Log("Failed to create renderer: %s\n", SDL_GetError());
    }

    if (IMG_Init(imageFlags) != (imageFlags)) {
        printf("Failed to initialise SDL image: %s\n", IMG_GetError());
    }

    SDL_RenderSetScale(
        self->renderer, RENDERER_SCALE_FACTOR, RENDERER_SCALE_FACTOR
    );

    Stage* stage = (Stage*)calloc(1, sizeof(Stage));
    self->currentStage = stage;
    stageInit(self->currentStage, self->renderer);

    InputEventQueue* queue = (InputEventQueue*)calloc(1, sizeof(InputEventQueue));
    self->inputEventQueue = queue;
    inputEventQueueInit(self->inputEventQueue);
}

void gameLoop(Game* self) {
    gameHandleInput(self);
    gameUpdate(self);
    gameRender(self);
    SDL_Delay(16);
}

void gameHandleInput(Game* self) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                exit(0);
                break;
            case SDL_KEYDOWN:
                onKeyDown(self, &event.key);
                break;
            case SDL_KEYUP:
                onKeyUp(self, &event.key);
                break;
            default:
                break;
        }
    }
}

void gameDestroy(Game* self) {
    free(self->inputEventQueue);
    free(self->currentStage);

    SDL_DestroyWindow(self->window);
    SDL_DestroyRenderer(self->renderer);
    SDL_Quit();
}

static void onKeyDown(Game* self, SDL_KeyboardEvent* event) {
    if (event->repeat == 0 && event->keysym.scancode < SDL_NUM_SCANCODES) {
        switch (event->keysym.scancode) {
            case DRINK_MILK_INPUT:
                enqueueInputEvent(self->inputEventQueue, EVENT_DRINK);
                break;
            default:
                break;
        }
    }
}

static void onKeyUp(Game* self, SDL_KeyboardEvent* event) {
    if (event->repeat == 0 && event->keysym.scancode < SDL_NUM_SCANCODES) {
        switch (event->keysym.scancode) {
            default:
                break;
        }
    }
}
