#pragma once

#include "player.h"

typedef struct {
    Player player;
} PlayerAi;

void player_ai_init(PlayerAi* player);
void player_ai_reset(PlayerAi* player, Track* track);