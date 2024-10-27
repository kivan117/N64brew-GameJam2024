#pragma once

#include "player.h"

#include "../../core.h"

typedef struct {
    Player base;
    AiDiff difficulty;
    const SimfileEvent* previous_event;
    float next_button_press_time;
} PlayerAi;

void player_ai_init(PlayerAi* player, AiDiff difficulty);
void player_ai_update(PlayerAi* player);
void player_ai_reset(PlayerAi* player, Track* track);