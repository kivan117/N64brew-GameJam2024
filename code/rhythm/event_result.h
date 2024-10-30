#pragma once

#include <libdragon.h>

#include "player.h"
#include "vec2.h"

#include <stdint.h>

#define EVENT_RESULT_DECAY_TIME 1.0f

typedef struct {
    Vec2 position;
    Player* player;
    rdpq_font_t* font;
    const char* text;
    float decay_time;
    uint8_t player_index;
    uint8_t font_id;
} EventResult;

void event_result_init(EventResult* result, const Vec2* position, Player* player, uint8_t player_index, rdpq_font_t* font, uint8_t font_id);
void event_result_update(EventResult* result, float time_delta);
void event_result_reset(EventResult* result);
void event_result_draw(EventResult* result);