#pragma once

#include <SDL2/SDL.h>

#include "game.h"
#include "stage.h"

SDL_Texture* load_texture(SDL_Renderer* renderer, char* filename);
