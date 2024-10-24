#pragma once

#include "simfile.h"

typedef void(*SimfilePlaybackEventCallbackFunc)(const SimfileEvent* event, void* arg);
#define SIMFILE_PLAYBACK_MAX_CALLBACKS_COUNT 4

typedef struct {
    SimfilePlaybackEventCallbackFunc func;
    void* arg;
} SimfilePlaybackEventCallbackEntry;

/** 
 * This structure tracks the current playback of a simfile.
 * It's main job is to dispatch events to listerenrs as time progresses during the playback of a simfile.
 * */
typedef struct {
    Simfile* simfile;
    float current_time;
    float event_lead_time;
    size_t next_event_index;
    SimfilePlaybackEventCallbackEntry callback_entries[SIMFILE_PLAYBACK_MAX_CALLBACKS_COUNT];
    size_t callback_entry_count;
} SimfilePlayback;

void simfile_playback_init(SimfilePlayback* playback, Simfile* simfile, float event_lead_time);
void simfile_playback_push_callback(SimfilePlayback* playback, SimfilePlaybackEventCallbackFunc callback, void* arg);
void simfile_playback_update(SimfilePlayback* playback, float time);

void simfile_playback_reset(SimfilePlayback* playback);
int simfile_playback_finished(SimfilePlayback* playback);