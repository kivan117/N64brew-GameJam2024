#include "players.h"

#include "../../core.h"

void players_init(Players* p) {
    // initialize controller players
    uint32_t player_count = core_get_playercount();
    for (uint32_t i = 0; i < player_count; i++) {
        player_controller_init(&p->players[i].controller, core_get_playercontroller(i));
    }

    AiDiff diff = DIFF_EASY;
    // initialize ai players players
    for (int i = player_count; i < 4; i++) {
        player_ai_init(&p->players[i].ai, diff++);
    }
}

void players_load_track(Players* p, Track* track) {
    for (int i = 0; i < 4; i++) {
        if (p->players[i].type == PLAYER_TYPE_AI) {
            player_ai_load_track(&p->players[i].ai, track);
        } else {
            player_load_track(&p->players[i].base, track);
        }
    }
}

void players_reset(Players* p) {
    for (int i = 0; i < 4; i++) {
        if (p->players[i].type == PLAYER_TYPE_AI) {
            player_ai_reset(&p->players[i].ai);
        } else {
            player_reset(&p->players[i].base);
        }
    }
}

void players_update(Players* p) {
    for (int i = 0; i < 4; i++) {
        if (p->players[i].type == PLAYER_TYPE_AI) {
            player_ai_update(&p->players[i].ai);
        } else {
            player_update(&p->players[i].base);
        }
    }
}

void players_set_input_buttons(Players* p, const SimfileInputTrackerButton* button_to_column_map, uint32_t count) {
    for (int i = 0; i < 4; i++) {
        simfile_input_tracker_set_button_to_column_map(&p->players[i].base.input_tracker, button_to_column_map, count);
    }
}
