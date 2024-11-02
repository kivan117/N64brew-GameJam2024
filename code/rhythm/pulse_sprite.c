#include "pulse_sprite.h"

void pulse_sprite_init(PulseSprite* pulse_sprite, sprite_t* sprite, const Vec2* position) {
    pulse_sprite->sprite = sprite;
    pulse_sprite->position = *position;
}

void pulse_sprite_tick(PulseSprite* pulse_sprite, SimfilePlayback* playback) {
    #define MIN_SCALE 0.65f
    #define PULSE_RANGE (1.0f - MIN_SCALE)
    float scale = 1.0f;

    // shrink the  pulse_sprite towards its minimum scale
    if (playback->current_beat_progress >= 0.5f) {
        // map 0.5 -> 1.0 to 1.0 -> MIN_SCALE
        float progress = (playback->current_beat_progress - 0.5f) * 2.0f;
        scale = 1.0f - (PULSE_RANGE * progress);
    } else { // grow the sprite back to its original size
        // map 0.0 -> 0.5 to MIN_SCALE -> 1.0
        float progress = playback->current_beat_progress * 2.0f;
        scale = MIN_SCALE + (PULSE_RANGE * progress);
    }

    rdpq_sprite_blit(pulse_sprite->sprite, 
        pulse_sprite->position.x - scale * pulse_sprite->sprite->width / 2, 
        pulse_sprite->position.y - scale * pulse_sprite->sprite->height / 2 , 
        &(rdpq_blitparms_t){.scale_x = scale, .scale_y = scale});
}