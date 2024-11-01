#include "target_indicator.h"

void target_indicator_init(TargetIndicator* indicator, sprite_t* sprite, const Vec2* position) {
    indicator->sprite = sprite;
    indicator->position = *position;
}

void target_indicator_tick(TargetIndicator* indicator, Metronome* metronome) {
    #define MIN_SCALE 0.65f
    float scale = MIN_SCALE + ( (metronome->time_to_next_beat / metronome->beat_time) * (1.0f - MIN_SCALE));

    rdpq_sprite_blit(indicator->sprite, 
        indicator->position.x - scale * indicator->sprite->width / 2, 
        indicator->position.y - scale * indicator->sprite->height / 2 , 
        &(rdpq_blitparms_t){.scale_x = scale, .scale_y = scale});
}