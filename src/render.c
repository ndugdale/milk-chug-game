#include "render.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Texture* load_texture(SDL_Renderer* renderer, char* filename) {
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
