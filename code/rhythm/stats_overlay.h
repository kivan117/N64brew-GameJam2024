#pragma once

#include "players.h"

typedef struct {
    const Players* players;
    uint8_t font_id;
} StatsOverlay;

void stats_overlay_init(StatsOverlay* overlay, const Players* players, rdpq_font_t* font, uint8_t font_id);
void stats_overlay_draw(StatsOverlay* overlay);