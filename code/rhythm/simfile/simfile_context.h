#pragma once

#include "simfile.h"

typedef void(*SimfileEventCallback)(const SimfileEvent* event, void* arg);
#define SIMFILE_CONTEXT_MAX_CALLBACKS_COUNT 4

typedef struct {
    SimfileEventCallback func;
    void* arg;
} SimfileEventCallbackEntry;

/** TODO: THis should be renamed playback context */
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