#pragma once

#include <libdragon.h>

#include "simfile/simfile_input_tracker.h"

#define NOTE_RESULT_LIFETIME 0.5f
#define NOTE_RESULT_BUFFER_SIZE 4

typedef struct {
    float x, y;
    float time_remaining;
    SimfileInputTrackerResultType type;
} NoteResult;

typedef struct {
    NoteResult buffer[NOTE_RESULT_BUFFER_SIZE];
    uint32_t head, tail;
    rdpq_font_t* font;
    uint8_t font_id;
} NoteResults;

void note_results_init(NoteResults* results, rdpq_font_t* font, uint8_t font_id);
void note_results_reset(NoteResults* results);
void note_results_push(NoteResults* results, float x, float y, SimfileInputTrackerResultType type);
void note_results_update(NoteResults* note_results, float deltatime);
void note_results_draw(NoteResults* note_results);