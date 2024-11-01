#include "simfile_playback.h"

void simfile_playback_init(SimfilePlayback* playback, Simfile* simfile, float event_lead_time) {
    playback->event_lead_time = event_lead_time;
    playback->simfile = simfile;
    playback->callback_entry_count = 0;

    metronome_init(&playback->metronome, playback->simfile->bpm);
    simfile_playback_reset(playback);
}

void simfile_playback_reset(SimfilePlayback* playback) {
    playback->current_time = playback->simfile->starting_time_offset;
    playback->next_event_index = 0;
    metronome_reset(&playback->metronome);
}

void simfile_playback_push_callback(SimfilePlayback* playback, SimfilePlaybackEventCallbackFunc callback, void* arg) {
    if (playback->callback_entry_count == SIMFILE_PLAYBACK_MAX_CALLBACKS_COUNT) {
        return;
    }

    SimfilePlaybackEventCallbackEntry* callback_entry = playback->callback_entries + playback->callback_entry_count++;
    callback_entry->func = callback;
    callback_entry->arg = arg;
}

void simfile_playback_update(SimfilePlayback* playback, float time) {
    // the song itself has finished// nothing to do
    if (playback->current_time >= playback->simfile->total_time) {
        playback->current_time = playback->simfile->total_time;
        return;
    }

    playback->current_time += time;
    metronome_update(&playback->metronome, time);

    // we have dispatched all remaining events nothing more to do
    if (playback->next_event_index >= playback->simfile->event_count) {
        return;
    }

    const float event_time = playback->current_time + playback->event_lead_time;
    SimfileEvent* current_event = playback->simfile->events + playback->next_event_index;

    // dispatch all events that are within the time window
    while (event_time >= current_event->time) {
        for (size_t i = 0; i < playback->callback_entry_count; i++) {
            SimfilePlaybackEventCallbackEntry* callback_entry = &playback->callback_entries[i];
            callback_entry->func(current_event, callback_entry->arg);
        }

        playback->next_event_index += 1;
        if (playback->next_event_index >= playback->simfile->event_count) {
            break;
        } else {
            current_event = playback->simfile->events + playback->next_event_index;
        }
    }
}

int simfile_playback_finished(SimfilePlayback* playback) {
    return playback->current_time >= playback->simfile->total_time;
}

void simfile_playback_sync(SimfilePlayback* playback) {
    playback->current_time = 0;
    playback->metronome.time_to_next_beat = playback->metronome.beat_time;
}