#pragma once

#include "event_result.h"
#include "players.h"

typedef struct {
    EventResult r[4];
} EventResults;

void event_results_init(EventResults* results, Players* players, rdpq_font_t* font, uint8_t font_id);
void event_results_update(EventResults* results, float time_delta);
void event_results_draw(EventResults* results);
void event_results_reset(EventResults* results);