#pragma once

#include <stdint.h>

typedef struct {
    /** Counts the number of beats that have ticked since the last call to init / reset */
    uint32_t beat_num;

    /** The time between each beat*/
    float beat_time;

    /** The number of seconds to the next beat */
    float time_to_next_beat;

    /** The progress in the current beat */
    float current_beat_progress;
} Metronome;

void metronome_init(Metronome* metronome, float bpm);
void metronome_reset(Metronome* metronome);
void metronome_update(Metronome* metronome, float time_delta);