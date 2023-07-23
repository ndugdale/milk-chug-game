#include "render.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "game.h"

static void prepareScene(Game* self);
static void presentScene(Game* self);

void gameRender(Game* self) {
    prepareScene(self);
    presentScene(self);
}

void stageRender(Stage* self, SDL_Renderer* renderer, SDL_Window* window) {
    int windowHeight;
    int windowWidth;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    const int scaledWindowWidth = (float)windowWidth /
                                  (float)RENDERER_SCALE_FACTOR;

    const int scaledWindowHeight = (float)windowHeight /
                                   (float)RENDERER_SCALE_FACTOR;

    const int rectX = (scaledWindowWidth - BACKGROUND_WIDTH) / 2;
    const int rectY = (scaledWindowHeight - BACKGROUND_HEIGHT) / 2;

    const SDL_Rect dstRect = {rectX, rectY, BACKGROUND_WIDTH, BACKGROUND_HEIGHT};

    SDL_RenderCopy(renderer, self->background, NULL, &dstRect);
}

SDL_Texture* loadTexture(SDL_Renderer* renderer, char* filename) {
    SDL_Texture* texture;
    SDL_LogMessage(
        SDL_LOG_CATEGORY_APPLICATION,
        SDL_LOG_PRIORITY_INFO,
        "Loading %s",
        filename
    );
    texture = IMG_LoadTexture(renderer, filename);

    return texture;
}

static void prepareScene(Game* self) {
    SDL_SetRenderDrawColor(self->renderer, 0, 0, 0, 255);
    SDL_RenderClear(self->renderer);
    stageRender(self->currentStage, self->renderer, self->window);
}

static void presentScene(Game* self) {
    SDL_RenderPresent(self->renderer);
}
