#pragma once

#include "event_track.h"
#include "simfile/simfile_playback.h"

#include <libdragon.h>

/** 
 * The event stream is a visual representation of upcomming (and past) events in a simfile
 * Each column is represneted by a track.
 * THe event stream will handle spawning items on tracks based on an event's columns.
 * */
typedef struct {
    EventTrack tracks[SIMFILE_DEFAULT_COLUMN_COUNT];
    int track_count;
    SimfilePlayback* playback;
} EventStream;

/**
 * Registeres callback with the playback context to listen for new events
 */
void event_stream_init(EventStream* stream, SimfilePlayback* playback);

/** 
 * Adds a new track into the stream.
 * Note: all tracks should be created before playback begins! 
 * */
void create_track(EventStream* stream, const Vec2* source_pos, const Vec2* target_pos, sprite_t* target_texture, sprite_t* item_texture);

void event_stream_update(EventStream* stream, float time_delta);