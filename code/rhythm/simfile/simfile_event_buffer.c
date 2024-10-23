#include "simfile_event_buffer.h"

#include <string.h>

void simfile_ring_buffer_init(SimfileEventBuffer* event_buffer) {
    memset(event_buffer, 0, sizeof(SimfileEventBuffer));
}

int simfile_event_buffer_push_back(SimfileEventBuffer* event_buffer, const SimfileEvent* event) {
    if (event_buffer->count == SIMFILE_EVENT_BUFFER_SIZE) {
        return 0;
    }

    event_buffer->events[event_buffer->tail] = event;
    event_buffer->tail += 1;
    if (event_buffer->tail == SIMFILE_EVENT_BUFFER_SIZE) {
        event_buffer->tail = 0;
    }

    event_buffer->count += 1;

    return 1;
}

const SimfileEvent* simfile_event_buffer_front(SimfileEventBuffer* event_buffer) {
    return event_buffer->count > 0 ? event_buffer->events[event_buffer->head] : NULL;
}

const SimfileEvent* simfile_event_buffer_get_at_index(SimfileEventBuffer* event_buffer, uint32_t index) {
    if (event_buffer->count <= index) {
        return NULL;
    }

    index += event_buffer->head;
    if (index >= SIMFILE_EVENT_BUFFER_SIZE) {
        index -= SIMFILE_EVENT_BUFFER_SIZE;
    }

    return event_buffer->events[index];
}

void simfile_event_buffer_pop_front(SimfileEventBuffer* event_buffer) {
    if (event_buffer->count == 0) {
        return;
    }

    event_buffer->head += 1;
    if (event_buffer->head == SIMFILE_EVENT_BUFFER_SIZE) {
        event_buffer->head = 0;
    }

    event_buffer->count -= 1;
}