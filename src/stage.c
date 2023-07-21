#include "stage.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "render.h"

void stageInit(Stage* self, SDL_Renderer* renderer) {
    self->background = loadTexture(renderer, "assets/images/purple_sky.png");
}
