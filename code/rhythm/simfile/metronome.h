#pragma once

#include <stdint.h>

typedef struct {
    uint32_t beat_num;
    float beat_time;
    float time_to_next_beat;
} Metronome;

void metronome_init(Metronome* metronome, float bpm);
void metronome_reset(Metronome* metronome);
void metronome_update(Metronome* metronome, float time_delta);