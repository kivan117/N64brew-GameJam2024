#include "stats_overlay.h"

#include <libdragon.h>

void stats_overlay_init(StatsOverlay* overlay, const Players* players, rdpq_font_t* font, uint8_t font_id) {
    overlay->players = players;
    overlay->font_id = font_id;

    // create a style for each player with their color
    rdpq_font_style(font, 0, &(rdpq_fontstyle_t){.color = color_from_packed32(0xFF0000FF) });
    rdpq_font_style(font, 1, &(rdpq_fontstyle_t){.color = color_from_packed32(0x00FF00FF) });
    rdpq_font_style(font, 2, &(rdpq_fontstyle_t){.color = color_from_packed32(0x0000FFFF) });
    rdpq_font_style(font, 3, &(rdpq_fontstyle_t){.color = color_from_packed32(0xFFFF00FF) });
    rdpq_font_style(font, 4, &(rdpq_fontstyle_t){.color = color_from_packed32(0xFFFFFFFF) });
}

static const char* format_str = 
    "Player %i\n"
    "%s\n"
    "Ma: %lu\n"
    "Pe: %lu\n"
    "Gr: %lu\n"
    "Go: %lu\n"
    "Bo: %lu\n"
    "Mi: %lu\n";

static const char* get_info_str(StatsOverlay* overlay, int index);

void stats_overlay_draw(StatsOverlay* overlay) {
    const int column_width = display_get_width() / 4;
    
    char buffer[64];
    for (int i  = 0; i < 4; i++) {
        const Player* player = &overlay->players->players[i].base;
        const char* info_str = get_info_str(overlay, i);

        int x_pos = (column_width * i) + 10;
        int y_pos = 100;
        sprintf(buffer, format_str, 
            i + 1,
            info_str,
            player->result_totals[INPUT_TRACKER_RESULT_MARVELOUS],
            player->result_totals[INPUT_TRACKER_RESULT_PERFECT],
            player->result_totals[INPUT_TRACKER_RESULT_GREAT],
            player->result_totals[INPUT_TRACKER_RESULT_GOOD],
            player->result_totals[INPUT_TRACKER_RESULT_BOO],
            player->result_totals[INPUT_TRACKER_RESULT_MISS]);
        rdpq_text_print(&(rdpq_textparms_t){ .style_id = i}, overlay->font_id, x_pos, y_pos, buffer);
    }
}

static const char* get_info_str(StatsOverlay* overlay, int index) {
    if (overlay->players->players[index].type == PLAYER_TYPE_CONTROLLER) {
        switch (overlay->players->players[index].controller.port)
        {
            case 0: return "Port 1";
            case 1: return "Port 2";
            case 2: return "Port 3";
            case 3: return "Port 4";
            default: return "unknown";
        }
    } else {
        switch (overlay->players->players[index].ai.difficulty)
        {
            case DIFF_EASY: return "AI (Easy)";
            case DIFF_MEDIUM: return "AI (Med)";
            case DIFF_HARD: return "AI (Hard)";
            default: return "unknown";
        }
    }

    return NULL;
}