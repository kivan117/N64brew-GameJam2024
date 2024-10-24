#include <libdragon.h>
#include "../../core.h"
#include "../../minigame.h"

#include "static_overlay.h"
#include "resources.h"

#include "simfile/simfile.h"
#include "simfile/simfile_context.h"
#include "simfile/simfile_input_tracker.h"

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
StaticOverlay static_overlay;

static int player_controller_get_button_pressed(int button, void* arg);
static void process_input();

#define LOOP_COUNT 3
static const LoopInfo loops[LOOP_COUNT] = {
    {
        "Tabloid Junkie",
        "rom:/rhythm/tabloid_junkie.wav64", 
        "rom:/rhythm/tabloid_junkie.csm", 
        "rom:/rhythm/tabloid_junkie_layout.layout",
        {SIMFILE_INPUT_TRACKER_BUTTON_A, SIMFILE_INPUT_TRACKER_BUTTON_B, SIMFILE_INPUT_TRACKER_BUTTON_L, SIMFILE_INPUT_TRACKER_BUTTON_R}
    },
    {
        "Privacy",
        "rom:/rhythm/privacy.wav64", 
        "rom:/rhythm/privacy.csm", 
        "rom:/rhythm/privacy_layout.layout",
        {SIMFILE_INPUT_TRACKER_BUTTON_L, SIMFILE_INPUT_TRACKER_BUTTON_R, SIMFILE_INPUT_TRACKER_BUTTON_Z, SIMFILE_INPUT_TRACKER_BUTTON_A}
    },
    {
        "Breaking News",
        "rom:/rhythm/breaking_news.wav64", 
        "rom:/rhythm/breaking_news.csm", 
        "rom:/rhythm/breaking_news_layout.layout",
        {SIMFILE_INPUT_TRACKER_BUTTON_C_LEFT, SIMFILE_INPUT_TRACKER_BUTTON_C_RIGHT, SIMFILE_INPUT_TRACKER_BUTTON_Z, SIMFILE_INPUT_TRACKER_BUTTON_A}
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
    static_overlay_init(&static_overlay, &resources);
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

    joypad_buttons_t btn = joypad_get_buttons_pressed(0);
    // if the song is finished restart it
    if (simfile_context_finished(&static_overlay.context)) {
        // temporary to restart loop
        if ((btn.raw & SIMFILE_INPUT_TRACKER_BUTTON_START)) {
            static_overlay_restart_loop(&static_overlay);
        }
        else if (btn.raw & SIMFILE_INPUT_TRACKER_BUTTON_R) {
            current_loop += 1;
            if (current_loop == LOOP_COUNT) {
                current_loop = 0;
            }
            load_loop(current_loop);
        }
    }

    static_overlay_tick(&static_overlay, deltatime);

    rdpq_detach_show();
}

/*==============================
    minigame_cleanup
    Clean up any memory used by your game just before it ends.
==============================*/
void minigame_cleanup()
{
    static_overlay_uninit(&static_overlay);
    rhythm_resources_uninit(&resources);
}

void load_loop(int index) {
    const LoopInfo* loop = &loops[index];
    static_overlay_load_loop(&static_overlay, loop);

    current_loop = index;
}