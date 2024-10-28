#pragma once

#include "player.h"
#include "track.h"

/** This struct represnets a player that is controlled by a controller */
typedef struct {
    Player base;
    uint32_t port;
} PlayerController;

void player_controller_init(PlayerController* controller, uint32_t port);