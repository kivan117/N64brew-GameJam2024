#include "resources.h"

static void create_player_font_styles(rdpq_font_t* font);

void rhythm_resources_init(RhythmResources* resources) {
    resources->sprites[RHYTHM_SPRITE_INDICATOR] = sprite_load("rom:/rhythm/indicator.sprite");
    resources->sprites[RHYTHM_SPRITE_BUTTON_A] = sprite_load("rom:/core/AButton.sprite");
    resources->sprites[RHYTHM_SPRITE_BUTTON_B] = sprite_load("rom:/core/BButton.sprite");
    resources->sprites[RHYTHM_SPRITE_BUTTON_L] = sprite_load("rom:/core/LTrigger.sprite");
    resources->sprites[RHYTHM_SPRITE_BUTTON_R]= sprite_load("rom:/core/RTrigger.sprite");
    resources->sprites[RHYTHM_SPRITE_BUTTON_Z] = sprite_load("rom:/core/ZTrigger.sprite");
    resources->sprites[RHYTHM_SPRITE_BUTTON_CR] = sprite_load("rom:/core/CRight.sprite");
    resources->sprites[RHYTHM_SPRITE_BUTTON_CL] = sprite_load("rom:/core/CLeft.sprite");
    resources->sprites[RHYTHM_SPRITE_BUTTON_DU] = sprite_load("rom:/core/DUp.sprite");
    resources->sprites[RHYTHM_SPRITE_BUTTON_DD] = sprite_load("rom:/core/DDown.sprite");
    resources->sprites[RHYTHM_SPRITE_BUTTON_DL] = sprite_load("rom:/core/DLeft.sprite");
    resources->sprites[RHYTHM_SPRITE_BUTTON_DR] = sprite_load("rom:/core/DRight.sprite");

    resources->fonts[RHYTHM_FONT_BUILTIN] = rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_VAR);
    resources->fonts[RHYTHM_FONT_TITLE] = rdpq_font_load("rom:/snake3d/m6x11plus.font64");

    for (int i = 0; i < RHYTHM_FONT_COUNT; i++) {
        rdpq_text_register_font(i + 1, resources->fonts[i] );
        create_player_font_styles(resources->fonts[i]);
    }
}

void rhythm_resources_uninit(RhythmResources* resources) {
    for (int i = 0; i < RHYTHM_SPRITE_COUNT; i++) {
        sprite_free(resources->sprites[i]);
    }

    // TODO: delete font?
    for (int i = 0; i < RHYTHM_FONT_COUNT; i++) {
        rdpq_text_unregister_font(i + 1);
        rdpq_font_free(resources->fonts[i]);
    }
}

sprite_t* rhythm_resources_get_sprite_for_button(RhythmResources* resouces, SimfileInputTrackerButton button) {
    switch(button) {
        case SIMFILE_INPUT_TRACKER_BUTTON_A: return resouces->sprites[RHYTHM_SPRITE_BUTTON_A];
        case SIMFILE_INPUT_TRACKER_BUTTON_B: return resouces->sprites[RHYTHM_SPRITE_BUTTON_B];
        case SIMFILE_INPUT_TRACKER_BUTTON_L: return resouces->sprites[RHYTHM_SPRITE_BUTTON_L];
        case SIMFILE_INPUT_TRACKER_BUTTON_R: return resouces->sprites[RHYTHM_SPRITE_BUTTON_R];
        case SIMFILE_INPUT_TRACKER_BUTTON_Z: return resouces->sprites[RHYTHM_SPRITE_BUTTON_Z];
        case SIMFILE_INPUT_TRACKER_BUTTON_C_LEFT: return resouces->sprites[RHYTHM_SPRITE_BUTTON_CL];
        case SIMFILE_INPUT_TRACKER_BUTTON_C_RIGHT: return resouces->sprites[RHYTHM_SPRITE_BUTTON_CR];
        case SIMFILE_INPUT_TRACKER_BUTTON_DPAD_UP: return resouces->sprites[RHYTHM_SPRITE_BUTTON_DU];
        case SIMFILE_INPUT_TRACKER_BUTTON_DPAD_DOWN: return resouces->sprites[RHYTHM_SPRITE_BUTTON_DD];
        case SIMFILE_INPUT_TRACKER_BUTTON_DPAD_LEFT: return resouces->sprites[RHYTHM_SPRITE_BUTTON_DL];
        case SIMFILE_INPUT_TRACKER_BUTTON_DPAD_RIGHT: return resouces->sprites[RHYTHM_SPRITE_BUTTON_DR];
        default: return NULL;
    }

    return NULL;
}

void create_player_font_styles(rdpq_font_t* font) {
    // create a style for each player with their color
    rdpq_font_style(font, 0, &(rdpq_fontstyle_t){.color = color_from_packed32(0xFF0000FF) });
    rdpq_font_style(font, 1, &(rdpq_fontstyle_t){.color = color_from_packed32(0x00FF00FF) });
    rdpq_font_style(font, 2, &(rdpq_fontstyle_t){.color = color_from_packed32(0x0000FFFF) });
    rdpq_font_style(font, 3, &(rdpq_fontstyle_t){.color = color_from_packed32(0xFFFF00FF) });
    rdpq_font_style(font, 4, &(rdpq_fontstyle_t){.color = color_from_packed32(0xFFFFFFFF) });
}