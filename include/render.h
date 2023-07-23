#pragma once

#include <SDL2/SDL.h>

#include "game.h"
#include "stage.h"

void gameRender(Game* self);
void stageRender(Stage* self, SDL_Renderer* renderer, SDL_Window* window);

SDL_Texture* loadTexture(SDL_Renderer* renderer, char* filename);
