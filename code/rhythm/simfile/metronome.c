#include "metronome.h"

void metronome_init(Metronome* metronome, float bpm) {
    metronome->beat_time = 60.0f / bpm;
    metronome_reset(metronome);
}

void metronome_reset(Metronome* metronome) {
    metronome->time_to_next_beat = metronome->beat_time;
    metronome->beat_num = 0;
    metronome->started = 0;
}

void metronome_update(Metronome* metronome, float time_delta) {
    if (!metronome->started) {
        return;
    }

    metronome->time_to_next_beat -= time_delta;
    if (metronome->time_to_next_beat <= 0.0f) {
        metronome->beat_num += 1;
        metronome->time_to_next_beat = metronome->beat_time - metronome->time_to_next_beat;
    }
}
