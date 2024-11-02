#include "metronome.h"

void metronome_init(Metronome* metronome, float bpm) {
    metronome->beat_time = 60.0f / bpm;
    metronome_reset(metronome);
}

void metronome_reset(Metronome* metronome) {
    metronome->time_to_next_beat = 0;
    metronome->beat_num = 0;
    metronome->current_beat_progress = 1.0f;
}

void metronome_update(Metronome* metronome, float time_delta) {
    metronome->time_to_next_beat -= time_delta;

    if (metronome->time_to_next_beat <= 0.0f) {
        metronome->beat_num += 1;
        metronome->time_to_next_beat = metronome->beat_time - metronome->time_to_next_beat;
    }

    metronome->current_beat_progress = metronome->time_to_next_beat / metronome->beat_time;
}
