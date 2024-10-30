#include "event_result.h"

void event_result_init(EventResult* result, const Vec2* position, Player* player, uint8_t player_index, rdpq_font_t* font, uint8_t font_id) {
    result->position = *position;
    result->player = player;
    result->player_index = player_index;
    result->font = font;
    result->font_id = font_id;

    event_result_reset(result);
}

static inline const char* get_result_type(SimfileInputTrackerResultType player_result_type) {
    switch(player_result_type) {
        case INPUT_TRACKER_RESULT_PERFECT: return "Perfect";
        case INPUT_TRACKER_RESULT_GREAT: return "Great";
        case INPUT_TRACKER_RESULT_GOOD: return "Good";
        case INPUT_TRACKER_RESULT_BOO: return "Boo";
        case INPUT_TRACKER_RESULT_MISS: return "Miss";
        default: return NULL;
    }
}

void event_result_update(EventResult* result, float time_delta) {
    SimfileInputTrackerResultType player_result_type = result->player->current_result.type;

    if (player_result_type == INPUT_TRACKER_RESULT_NONE) {
        result->decay_time -= time_delta;
        return;
    }

    result->decay_time = EVENT_RESULT_DECAY_TIME;
    result->text = get_result_type(player_result_type);
}

void event_result_reset(EventResult* result) {
    result->text = NULL;
    result->decay_time = 0.0f;
}

void event_result_draw(EventResult* result) {
    if (result->decay_time <= 0.0f) {
        return;
    }

    rdpq_text_print(&(rdpq_textparms_t){ .style_id = result->player_index}, result->font_id, result->position.x, result->position.y, result->text);
}
