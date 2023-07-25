#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

#define DRINK_MILK_INPUT SDL_SCANCODE_X
#define MAX_QUEUED_EVENTS 64
#define EMPTY_QUEUE -1

typedef enum {
    EVENT_DRINK,
    EVENT_TICK,
    NUM_EVENTS
} Event;

typedef struct {
    Event events[MAX_QUEUED_EVENTS];
    int front;
    int rear;
} EventQueue;

EventQueue* event_queue_create(void);
bool event_queue_is_empty(EventQueue* self);
void event_queue_enqueue(EventQueue* self, Event event);
Event event_queue_dequeue(EventQueue* self);
void event_queue_destroy(EventQueue* self);
