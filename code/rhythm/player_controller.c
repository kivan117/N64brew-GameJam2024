#include "player_controller.h"

static int player_controller_get_button_pressed(int button, void* arg);

void player_controller_init(PlayerController* player, uint32_t port) {
    player->port = port;
    SimfileInputTrackerInterface input_interface = {player_controller_get_button_pressed, player};
    player_init(&player->base, PLAYER_TYPE_CONTROLLER, &input_interface);
    player->base.input_tracker.debug_handle = PLAYER_TYPE_CONTROLLER;
}

int player_controller_get_button_pressed(int button, void* arg) {
    PlayerController* player = (PlayerController*)arg;
    joypad_buttons_t btn = joypad_get_buttons_pressed(player->port);

    return btn.raw & (uint32_t)button;
}