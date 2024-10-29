#include "event_stream.h"

#include <stdint.h>

static void on_simfile_event(const SimfileEvent* event, void* arg);

void event_stream_init(EventStream* stream, SimfilePlayback* playback) {
    stream->playback = playback;
    stream->track_count = 0;

    simfile_playback_push_callback(playback, on_simfile_event, playback);
}

/** Adds a new track into the stream. */
void create_track(EventStream* stream, const Vec2* source_pos, const Vec2* target_pos, sprite_t* item_texture, sprite_t* target_texture) {
    if (stream->track_count == SIMFILE_DEFAULT_COLUMN_COUNT) {
        return;
    }

    event_track_init(&stream->tracks[stream->track_count++], source_pos, target_pos, item_texture, target_texture, stream->playback->event_lead_time);
}

void event_stream_update(EventStream* stream, float time_delta) {

}


/** Look at all columns in the new event and add them to the appropriate track */
void on_simfile_event(const SimfileEvent* event, void* arg) {
    EventStream* stream = (EventStream*)arg;
    const float time_remaining = event->time -stream->playback->current_time;

    for (int i = 0; i < SIMFILE_DEFAULT_COLUMN_COUNT; i++) {
        uint16_t mask = 1 << i;
        if (event->columns & mask) {

            event_track_add_item(&stream->tracks[i], time_remaining);
        }
    }
}