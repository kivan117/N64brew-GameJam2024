#include "resources.h"

void rhythm_resources_init(RhythmResources* resources) {
    resources->sprites[RHYTHM_SPRITE_INDICATOR] = sprite_load("rom:/rhythm/indicator.sprite");
    resources->sprites[RHYTHM_SPRITE_BUTTON_A] = sprite_load("rom:/core/AButton.sprite");
    resources->sprites[RHYTHM_SPRITE_BUTTON_B] = sprite_load("rom:/core/BButton.sprite");
    resources->sprites[RHYTHM_SPRITE_BUTTON_L] = sprite_load("rom:/core/LTrigger.sprite");
    resources->sprites[RHYTHM_SPRITE_BUTTON_R]= sprite_load("rom:/core/RTrigger.sprite");
    resources->sprites[RHYTHM_SPRITE_BUTTON_Z] = sprite_load("rom:/core/ZTrigger.sprite");
    resources->sprites[RHYTHM_SPRITE_BUTTON_CR] = sprite_load("rom:/core/CRight.sprite");
    resources->sprites[RHYTHM_SPRITE_BUTTON_CL] = sprite_load("rom:/core/CLeft.sprite");

    resources->fonts[RHYTHM_FONT_EVENT_RESULT] = rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_VAR);

    for (int i = 0; i < RHYTHM_FONT_COUNT; i++) {
        rdpq_text_register_font(i + 1, resources->fonts[i] );
    }
}

void rhythm_resources_uninit(RhythmResources* resources) {
    for (int i = 0; i < RHYTHM_SPRITE_COUNT; i++) {
        sprite_free(resources->sprites[i]);
    }

    // TODO: delete font?
    for (int i = 0; i < RHYTHM_FONT_COUNT; i++) {
        rdpq_text_unregister_font(i + 1);
    }
}