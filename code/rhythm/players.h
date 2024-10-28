#pragma once

#include "player_controller.h"
#include "player_ai.h"

typedef union {
    PlayerType type;
    Player base;
    PlayerController controller;
    PlayerAi ai;
} PlayerU;

typedef struct {
    PlayerU players[4];
} Players;

void players_init(Players* p);
void players_load_track(Players* p, Track* track);
void players_reset(Players* p);
void players_update(Players* p);

