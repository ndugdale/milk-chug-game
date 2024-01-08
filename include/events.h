#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>

#define ACTION_KEY SDL_SCANCODE_X
#define TOGGLE_AUDIO_KEY SDL_SCANCODE_M
#define QUIT_GAME_KEY SDL_SCANCODE_Q
#define MAX_QUEUED_EVENTS INT8_MAX
#define EMPTY_QUEUE -1

typedef enum {
    EVENT_ACTION,
    EVENT_TICK,
    EVENT_TOGGLE_AUDIO,
    EVENT_WINDOW_RESIZE,
    NUM_EVENTS
} Event;

typedef struct {
    Event events[MAX_QUEUED_EVENTS];
    int8_t front;
    int8_t rear;
} EventQueue;

EventQueue* event_queue_create(void);
bool event_queue_is_empty(const EventQueue* self);
void event_queue_enqueue(EventQueue* self, Event event);
Event event_queue_dequeue(EventQueue* self);
void event_queue_destroy(EventQueue* self);
