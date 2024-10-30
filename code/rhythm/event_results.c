#include "event_results.h"

void event_results_init(EventResults* results, Players* players, rdpq_font_t* font, uint8_t font_id) {
    const float y_pos = 200.0f;
    const float column_width = display_get_width() / 4;

    for (int i = 0; i < 4; i++) {
        Vec2 position = {column_width * i + 10, y_pos};
        event_result_init(&results->r[i], &position, &players->players[i].base, (uint8_t)i, font, font_id);
    }
}

void event_results_update(EventResults* results, float time_delta) {
    for (int i = 0; i < 4; i++) {
        event_result_update(&results->r[i], time_delta);
    }
}

void event_results_draw(EventResults* results) {
    for (int i = 0; i < 4; i++) {
        event_result_draw(&results->r[i]);
    }
}

void event_results_reset(EventResults* results) {
    for (int i = 0; i < 4; i++) {
        event_result_reset(&results->r[i]);
    }
}