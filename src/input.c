#include "input.h"

#include <SDL2/SDL.h>
#include <stdbool.h>

void inputEventQueueInit(InputEventQueue* queue) {
    queue->front = EMPTY_QUEUE;
    queue->rear = EMPTY_QUEUE;
}

bool isInputEventQueueEmpty(InputEventQueue* queue) {
    return (queue->front == EMPTY_QUEUE && queue->rear == EMPTY_QUEUE);
}

void enqueueInputEvent(InputEventQueue* queue, InputEvent event) {
    if (isInputEventQueueEmpty(queue)) {
        queue->front = 0;
        queue->rear = 0;
    } else {
        queue->rear = (queue->rear + 1) % MAX_QUEUED_INPUT_EVENTS;
    }
    queue->events[queue->rear] = event;
}

InputEvent dequeueInputEvent(InputEventQueue* queue) {
    InputEvent event = queue->events[queue->front];
    if (queue->front == queue->rear) {
        queue->front = -1;
        queue->rear = -1;
    } else {
        queue->front = (queue->front + 1) % MAX_QUEUED_INPUT_EVENTS;
    }
    return event;
}
