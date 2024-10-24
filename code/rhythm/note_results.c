#include "note_results.h"

#include <string.h>

void note_results_init(NoteResults* note_results,rdpq_font_t* font, uint8_t font_id) {
    note_results->font = font;
    note_results->font_id = font_id;

    #define TEXT_COLOR          0x6CBB3CFF
    rdpq_font_style(font, font_id, &(rdpq_fontstyle_t){.color = color_from_packed32(TEXT_COLOR) });
    note_results_reset(note_results);
}

void note_results_reset(NoteResults* note_results) {
    memset(note_results, 0 , sizeof(NoteResults));
}

void note_results_push(NoteResults* note_results, float x, float y, SimfileInputTrackerResultType type) {
    NoteResult* note_result = &note_results->buffer[note_results->tail];

    note_result->x = x;
    note_result->y = y;
    note_result->type = type;
    note_result->time_remaining = NOTE_RESULT_LIFETIME;

    note_results->tail += 1;
    if (note_results->tail == NOTE_RESULT_BUFFER_SIZE) {
        note_results->tail = 0;
    }
}

#define NOTE_RESULTS_SPEED 20.0f

void note_results_update(NoteResults* note_results, float deltatime) {
    uint32_t current = note_results->head;

    while (current != note_results->tail) {
        NoteResult* note_result = &note_results->buffer[current];
        note_result->time_remaining -= deltatime;
        note_result->y -= NOTE_RESULTS_SPEED * deltatime;

        // if the note result's lifetime has run out then remove it from the buffer
        if (note_result->time_remaining <= 0 && current == note_results->head) {
            note_results->head += 1;
            if (note_results->head == NOTE_RESULT_BUFFER_SIZE) {
                note_results->head = 0;
            }
        }

        current += 1;
        if (current == NOTE_RESULT_BUFFER_SIZE) {
            current = 0;
        }
    }
}

void note_results_draw(NoteResults* note_results) {
    uint32_t current = note_results->head;

    while (current != note_results->tail) {
        NoteResult* note_result = &note_results->buffer[current];

        const char* message = NULL;

        switch(note_result->type) {
            case INPUT_TRACKER_RESULT_NONE:
            case INPUT_TRACKER_RESULT_COUNT:
                break; // should not happen

            case INPUT_TRACKER_RESULT_MARVELOUS:
                message = "Marvelous";
                break;

            case INPUT_TRACKER_RESULT_PERFECT:
                message = "Perfect";
                break;

            case INPUT_TRACKER_RESULT_GREAT:
                message = "Great";
                break;

            case INPUT_TRACKER_RESULT_GOOD:
                message = "Good";
                break;

            case INPUT_TRACKER_RESULT_BOO:
                message = "Boo";
                break;

            case INPUT_TRACKER_RESULT_MISS:
                message = "Miss";
                break;
        }

        if (note_result->time_remaining > 0.0f) {
            rdpq_text_print(&(rdpq_textparms_t){ .style_id = 0 }, note_results->font_id, note_result->x, note_result->y, message);
        }

        current += 1;
        if (current == NOTE_RESULT_BUFFER_SIZE) {
            current = 0;
        }
    }
}