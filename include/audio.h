#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>

#define MAX_NUM_AUDIO 16
#define MIX_CHUNK_SIZE 1024

typedef struct {
    const char* id;
    Mix_Chunk* chunk;
} EffectAudio;

typedef struct {
    const char* id;
    Mix_Music* music;
} MusicAudio;

typedef struct {
    EffectAudio* effects[MAX_NUM_AUDIO];
    uint8_t effect_insert_index;
    MusicAudio* musics[MAX_NUM_AUDIO];
    uint8_t music_insert_index;
    bool muted;
} AudioManager;

AudioManager* audio_manager_create(void);
void audio_manager_play_effect(AudioManager* self, const char* id);
void audio_manager_play_music(AudioManager* self, const char* id);
void audio_manager_toggle(AudioManager* self);
void audio_manager_destroy(AudioManager* self);
