#include "player_ai.h"

static int player_ai_controller_get_button_pressed(int button, void* arg);


void player_ai_init(PlayerAi* player) {
    SimfileInputTrackerInterface input_interface = {player_ai_controller_get_button_pressed, player};
    player_init(&player->player, PLAYER_TYPE_AI, &input_interface);
}

void player_ai_reset(PlayerAi* player, Track* track) {
    // TODO: bind event to track in order to determine
    player_reset(&player->player, track);
}

int player_ai_controller_get_button_pressed(int button, void* arg) {
    return 0;
}
