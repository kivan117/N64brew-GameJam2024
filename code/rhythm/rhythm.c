#include <libdragon.h>
#include "../../core.h"
#include "../../minigame.h"

#include "simfile/simfile.h"
#include "simfile/simfile_playback.h"
#include "simfile/simfile_input_tracker.h"

#include "event_stream.h"
#include "event_results.h"
#include "loop_info.h"
#include "players.h"
#include "pulse_sprite.h"
#include "stats_overlay.h"
#include "resources.h"
#include "target_indicator.h"
#include "track.h"

#include <string.h>

const MinigameDef minigame_def = {
    .gamename = "Rhythm Game",
    .developername = "Your Name",
    .description = "This is an example game.",
    .instructions = "Press A to win."
};

#define GAME_BACKGROUND     0x000000FF
static uint32_t background_color = GAME_BACKGROUND;

RhythmResources resources;
Track track;
Players players;
StatsOverlay stats_overlay;
EventStream event_stream;
EventResults event_results;
TargetIndicator target_indicator;
PulseSprite pulse_sprite;

#define LOOP_COUNT 4
static const LoopInfo loops[LOOP_COUNT] = {
    {
        "Tabloid Junkie",
        "rom:/rhythm/tabloid_junkie.wav64", 
        "rom:/rhythm/tabloid_junkie.csm", 
        {SIMFILE_INPUT_TRACKER_BUTTON_A, SIMFILE_INPUT_TRACKER_BUTTON_B, SIMFILE_INPUT_TRACKER_BUTTON_L, SIMFILE_INPUT_TRACKER_BUTTON_R}
    },
    {
        "Privacy",
        "rom:/rhythm/privacy.wav64", 
        "rom:/rhythm/privacy.csm", 
        {SIMFILE_INPUT_TRACKER_BUTTON_L, SIMFILE_INPUT_TRACKER_BUTTON_R, SIMFILE_INPUT_TRACKER_BUTTON_Z, SIMFILE_INPUT_TRACKER_BUTTON_A}
    },
    {
        "Breaking News",
        "rom:/rhythm/breaking_news.wav64", 
        "rom:/rhythm/breaking_news.csm", 
        {SIMFILE_INPUT_TRACKER_BUTTON_C_LEFT, SIMFILE_INPUT_TRACKER_BUTTON_C_RIGHT, SIMFILE_INPUT_TRACKER_BUTTON_Z, SIMFILE_INPUT_TRACKER_BUTTON_A}
    },
    {
        "Screenbreaker",
        "rom:/rhythm/screenbreaker.wav64", 
        "rom:/rhythm/screenbreaker.csm", 
        {SIMFILE_INPUT_TRACKER_BUTTON_DPAD_UP, SIMFILE_INPUT_TRACKER_BUTTON_DPAD_DOWN, SIMFILE_INPUT_TRACKER_BUTTON_DPAD_LEFT, SIMFILE_INPUT_TRACKER_BUTTON_DPAD_RIGHT}
    }
};

static int current_loop = -1;
static void load_loop(int index);

/*==============================
    minigame_init
    The minigame initialization function
==============================*/
void minigame_init()
{
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, FILTERS_RESAMPLE);
    rhythm_resources_init(&resources);
    players_init(&players);
    event_results_init(&event_results, &players, resources.fonts[RHYTHM_FONT_BUILTIN], rhythm_resources_get_font_id(&resources, RHYTHM_FONT_BUILTIN));
    stats_overlay_init(&stats_overlay, &players, resources.fonts[RHYTHM_FONT_BUILTIN], rhythm_resources_get_font_id(resources, RHYTHM_FONT_BUILTIN));

    const Vec2 pulse_sprite_pos = {20.0f, 20.0f};
    pulse_sprite_init(&pulse_sprite, resources.sprites[RHYTHM_SPRITE_PULSE_TEST], &pulse_sprite_pos);
    load_loop(0);
}

/*==============================
    minigame_fixedloop
    Code that is called every loop, at a fixed delta time.
    Use this function for stuff where a fixed delta time is 
    important, like physics.
    @param  The fixed delta time for this tick
==============================*/
void minigame_fixedloop(float deltatime)
{

}

/*==============================
    minigame_loop
    Code that is called every loop.
    @param  The delta time for this tick
==============================*/
void minigame_loop(float deltatime)
{
    rdpq_attach(display_get(), NULL);
    rdpq_clear(color_from_packed32(background_color));

    rdpq_set_mode_standard();
    rdpq_mode_alphacompare(1);

    joypad_buttons_t btn_down = joypad_get_buttons_held(0);
    joypad_buttons_t btn_pressed = joypad_get_buttons_pressed(0);

    if ((btn_pressed.raw & SIMFILE_INPUT_TRACKER_BUTTON_START)) {
        if (!track.started) {
            //debugf("start track\n");
            track_start(&track);
        }

        // if the song is finished restart it
        if (simfile_playback_finished(&track.playback)) {
            track_reset(&track);
            players_reset(&players);
            event_stream_reset(&event_stream);
            event_results_reset(&event_results);
        }
    }

    // temp loop navigation
    if (btn_down.raw & SIMFILE_INPUT_TRACKER_BUTTON_Z) {
        if (btn_pressed.raw & SIMFILE_INPUT_TRACKER_BUTTON_DPAD_RIGHT) {
            current_loop += 1;
            if (current_loop == LOOP_COUNT) {
                current_loop = 0;
            }
            load_loop(current_loop);
        }

        if (btn_pressed.raw & SIMFILE_INPUT_TRACKER_BUTTON_DPAD_LEFT) {
            current_loop -= 1;
            if (current_loop < 0) {
                current_loop = LOOP_COUNT - 1;
            }
            load_loop(current_loop);
        }
    }

    track_update(&track, deltatime);
    event_stream_update(&event_stream, deltatime);
    event_results_update(&event_results, deltatime);
    players_update(&players);
    
    rdpq_sprite_blit(target_indicator.sprite, target_indicator.position.x, target_indicator.position.y, NULL);
    pulse_sprite_tick(&pulse_sprite, &track.playback.metronome);

    event_stream_draw(&event_stream);
    event_results_draw(&event_results);
    stats_overlay_draw(&stats_overlay);

    rdpq_text_printf(&(rdpq_textparms_t){
        .width = display_get_width(), .align = ALIGN_CENTER, .style_id = 4
    }, rhythm_resources_get_font_id(&resources, RHYTHM_FONT_TITLE), 0 ,30, loops[current_loop].name);
    

    rdpq_detach_show();
}

/*==============================
    minigame_cleanup
    Clean up any memory used by your game just before it ends.
==============================*/
void minigame_cleanup()
{
    track_unload(&track);
    rhythm_resources_uninit(&resources);
}

void load_loop(int index) {
    if (current_loop >= 0) {
        track_unload(&track);
    }

    const LoopInfo* loop = &loops[index];

    track_init(&track, 1, loop->wav_file, loop->simfile, 0.75);
    players_load_track(&players, &track);
    players_set_input_buttons(&players, loop->column_to_button_map, SIMFILE_DEFAULT_COLUMN_COUNT);
    event_results_reset(&event_results);
    
    // create a track for each column...setting the appropriate sprite
    // in this case each track has the same positions
    const Vec2 start_pos = {350.0f, 60.0f};
    const Vec2 target_pos = {160.0f, 60.0f};
    event_stream_init(&event_stream, &track.playback);
    for (int i = 0; i < SIMFILE_DEFAULT_COLUMN_COUNT; i++) {
        sprite_t* button_sprite = rhythm_resources_get_sprite_for_button(&resources, loop->column_to_button_map[i]);
        event_stream_create_track(&event_stream, &start_pos, &target_pos, button_sprite);
    }

    target_indicator.sprite = resources.sprites[RHYTHM_SPRITE_INDICATOR];
    target_indicator.position = target_pos;

    current_loop = index;
}
