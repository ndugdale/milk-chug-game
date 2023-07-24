#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

#define DRINK_MILK_INPUT SDL_SCANCODE_X
#define MAX_QUEUED_INPUT_EVENTS 64
#define EMPTY_QUEUE -1

typedef enum {
    EVENT_DRINK,
    NUM_EVENTS
} InputEvent;

typedef struct {
    InputEvent events[MAX_QUEUED_INPUT_EVENTS];
    int front;
    int rear;
} InputEventQueue;

InputEventQueue* input_event_queue_create(void);
bool input_event_queue_is_empty(InputEventQueue* self);
void input_event_queue_enqueue(InputEventQueue* self, InputEvent event);
InputEvent input_event_queue_dequeue(InputEventQueue* self);
void input_event_queue_destroy(InputEventQueue* self);
