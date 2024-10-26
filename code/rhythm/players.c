#include "players.h"

#include "../../core.h"

void players_init(Players* p) {
    // initialize controller players
    uint32_t player_count = core_get_playercount();
    for (uint32_t i = 0; i < player_count; i++) {
        player_controller_init(&p->players[i].controller, core_get_playercontroller(i));
    }

    // initialize ai players players
    for (int i = player_count; i < 4; i++) {
        player_ai_init(&p->players[i].ai);
    }
}

void players_reset(Players* p, Track* track) {
    for (int i = 0; i < 4; i++) {
        if (p->players[i].type == PLAYER_TYPE_AI) {
            player_ai_reset(&p->players[i].ai, track);
        } else {
            player_reset(&p->players[i].base, track);
        }
    }
}

void players_update(Players* p) {
    for (int i = 0; i < 4; i++) {
        player_update(&p->players[i].base);
    }
}
