#include "texture.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <string.h>

static void load_texture(
    TextureManager* self, SDL_Renderer* renderer, const char* id,
    const char* path
);

TextureManager* texture_manager_create(SDL_Renderer* renderer) {
    TextureManager* self = calloc(1, sizeof(TextureManager));

    // Load player character textures
    load_texture(self, renderer, "player", "assets/images/player.png");

    // Load non-player character textures
    load_texture(self, renderer, "opponent0", "assets/images/opponent0.png");
    load_texture(self, renderer, "opponent1", "assets/images/opponent1.png");
    load_texture(self, renderer, "opponent2", "assets/images/opponent2.png");
    load_texture(self, renderer, "opponent3", "assets/images/opponent3.png");
    load_texture(self, renderer, "opponent4", "assets/images/opponent4.png");
    load_texture(self, renderer, "opponent5", "assets/images/opponent5.png");
    load_texture(self, renderer, "opponent6", "assets/images/opponent6.png");
    load_texture(self, renderer, "opponent7", "assets/images/opponent7.png");
    load_texture(self, renderer, "opponent8", "assets/images/opponent8.png");
    load_texture(self, renderer, "opponent9", "assets/images/opponent9.png");
    load_texture(self, renderer, "opponent10", "assets/images/opponent10.png");

    // Load background textures
    load_texture(self, renderer, "stage0", "assets/images/stage0.png");

    // Load UI textures
    load_texture(self, renderer, "indicator", "assets/images/indicator.png");

    return self;
}

SDL_Texture* texture_manager_get(TextureManager* self, const char* id) {
    for (size_t i = 0; i < self->insert_index; i++) {
        if (strcmp(self->textures[i]->id, id) == 0) {
            return self->textures[i]->texture;
        }
    }
    SDL_LogError(
        SDL_LOG_CATEGORY_APPLICATION,
        "No texture with id %s is stored in the texture manager",
        id
    );
    return NULL;
}

void texture_manager_destroy(TextureManager* self) {
    for (size_t i = 0; i < self->insert_index; i++) {
        SDL_DestroyTexture(self->textures[i]->texture);
        free(self->textures[i]);
    }
    free(self);
}

static void load_texture(
    TextureManager* self, SDL_Renderer* renderer, const char* id,
    const char* path
) {
    if (self->insert_index > MAX_NUM_TEXTURES) {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "Failed to load texture from file %s: "
            "texture manager has run out of capacity",
            path
        );
        exit(1);
    }

    SDL_Log("Loading texture: %s", path);
    SDL_Texture* texture = IMG_LoadTexture(renderer, path);

    if (texture == NULL) {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "Failed to load texture from file %s: %s",
            path,
            SDL_GetError()
        );
        exit(1);
    }

    Texture* insert_item = malloc(sizeof(Texture));
    insert_item->id = id;
    insert_item->texture = texture;

    self->textures[self->insert_index] = insert_item;
    self->insert_index++;
}
