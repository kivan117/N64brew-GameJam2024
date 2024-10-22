#pragma once

#include <stddef.h>
#include <stdint.h>

typedef enum {
    SIMFILE_EVENT_NONE,
    SIMFILE_EVENT_TAP,
    SIMFILE_EVENT_HOLD,
    SIMFILE_EVENT_ROLL,
    SIMFILE_EVENT_MINE
} SimfileEventType;

typedef struct {
    float time;
    uint16_t duration;
    uint8_t type;
    uint8_t columns;
} SimfileEvent;

typedef struct {
    /** The total time of the simfile. */
    float total_time;

    /** defines an offset to the current time of a context that is evaluating this simfile. */
    float starting_time_offset;
    uint32_t event_count;
    SimfileEvent* events;
} Simfile;

void simfile_init_debug(Simfile* simfile, float total_time, float starting_time_offset, uint32_t event_count, SimfileEvent* events);
int simfile_open(Simfile* simfile, const char* path);
void simfile_uninit(Simfile* simfile);

typedef void(*SimfileEventCallback)(const SimfileEvent* event, void* arg);
#define SIMFILE_CONTEXT_MAX_CALLBACKS_COUNT 4

typedef struct {
    SimfileEventCallback func;
    void* arg;
} SimfileEventCallbackEntry;

typedef struct {
    Simfile* simfile;
    float current_time;
    float event_lead_time;
    size_t next_event_index;
    SimfileEventCallbackEntry callback_entries[SIMFILE_CONTEXT_MAX_CALLBACKS_COUNT];
    size_t callback_entry_count;
} SimfileContext;

void simfile_context_init(SimfileContext* context, Simfile* simfile, float event_lead_time);
void simfile_context_push_callback(SimfileContext* context, SimfileEventCallback callback, void* arg);
void simfile_context_update(SimfileContext* context, float time);

void simfile_context_reset(SimfileContext* context);
int simfile_context_finished(SimfileContext* context);