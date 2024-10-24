#include <libdragon.h>
#include "../../core.h"
#include "../../minigame.h"

#include "note_results.h"
#include "button_overlay.h"
#include "indicators.h"

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

sprite_t* indicator_sprite;
rdpq_font_t *font;



#define GAME_BACKGROUND     0x000000FF
static uint32_t background_color = GAME_BACKGROUND;


wav64_t audio_file;

static Simfile simfile;
static SimfileContext context;
static SimfileInputTracker tracker;

// static overlay
static NoteResults note_results;
static ButtonOverlay button_overlay;
static Indicators indicators;

static void show_next_indicator(const SimfileEvent* event, void* arg);

static int player_controller_get_button_pressed(int button, void* arg);
static void process_input();


typedef struct {
    const char* name;
    const char* wav_file;
    const char* simfile;
    const char* layout;
    SimfileInputTrackerButton column_to_button_map[SIMFILE_TRACKER_DEFAULT_COLUMN_COUNT];
} Loop;

#define LOOP_COUNT 3
static const Loop loops[LOOP_COUNT] = {
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
    simfile_init(&simfile);
    button_overlay_init(&button_overlay);

    indicator_sprite = sprite_load("rom:/rhythm/indicator.sprite");
    font = rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_VAR);
    rdpq_text_register_font(1, font);
    #define TEXT_COLOR          0x6CBB3CFF
    rdpq_font_style(font, 0, &(rdpq_fontstyle_t){.color = color_from_packed32(TEXT_COLOR) });

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

    simfile_context_update(&context, deltatime);
    note_results_update(&note_results, deltatime);
    process_input(deltatime);

    joypad_buttons_t btn = joypad_get_buttons_pressed(0);
    // if the song is finished restart it
    if (simfile_context_finished(&context)) {
        // restart loop
        if ((btn.raw & SIMFILE_INPUT_TRACKER_BUTTON_START)) {
            
            mixer_ch_stop(1);
            indicators_reset(&indicators);
            simfile_context_reset(&context);
            simfile_input_tracker_reset(&tracker);
            note_results_init(&note_results);
        }
        else if (btn.raw & SIMFILE_INPUT_TRACKER_BUTTON_R) {
            current_loop += 1;
            if (current_loop == LOOP_COUNT) {
                current_loop = 0;
            }
            load_loop(current_loop);
        }
    }

    // temporary fix
    if (!simfile_context_finished(&context) && context.current_time > 0 && !mixer_ch_playing(1)) {
        wav64_play(&audio_file, 1);
    }
    
    indicators_tick(&indicators, deltatime);
    button_overlay_draw(&button_overlay);
    note_results_draw(&note_results, font);

    rdpq_detach_show();
}

/*==============================
    minigame_cleanup
    Clean up any memory used by your game just before it ends.
==============================*/
void minigame_cleanup()
{
    button_overlay_uninit(&button_overlay);
    simfile_uninit(&simfile);

    wav64_close(&audio_file);
    sprite_free(indicator_sprite);
    rdpq_text_unregister_font(1);
}

void load_loop(int index) {
    if (current_loop >= 0) {
        mixer_ch_stop(1);
        wav64_close(&audio_file);
    }

    const Loop* loop = &loops[index];

    wav64_open(&audio_file, loop->wav_file);
    simfile_open(&simfile, loop->simfile);
    simfile_context_init(&context, &simfile, DEFAULT_INDICATOR_LIFETIME);
    simfile_context_push_callback(&context, show_next_indicator, NULL);
    indicators_init(&indicators, DEFAULT_INDICATOR_LIFETIME, &context, &button_overlay, indicator_sprite);

    SimfileInputTrackerInterface input_interface = {player_controller_get_button_pressed, 0};
    simfile_input_tracker_init(&tracker, &context, &input_interface);
    simfile_input_tracker_set_button_to_column_map(&tracker, loop->column_to_button_map, SIMFILE_TRACKER_DEFAULT_COLUMN_COUNT);
    button_overlay_open_f(&button_overlay, loop->layout);
    note_results_init(&note_results);

    current_loop = index;
}

static void show_next_indicator(const SimfileEvent* event, void* arg) {
    indicators_push(&indicators, event);
    simfile_input_tracker_enqueue(&tracker, event);
}

int player_controller_get_button_pressed(int button, void* arg) {
    uint32_t port = (uint32_t)arg;
    joypad_buttons_t btn = joypad_get_buttons_pressed(port);

    return btn.raw & (uint32_t)button;
}

void process_input() {
    SimfileInputTrackerResult result = simfile_input_tracker_update(&tracker);

    if (result.type <= INPUT_TRACKER_RESULT_BOO) {
        ButtonOverlayItem* overlay_item = &button_overlay.overlay_items[result.event_index];
        note_results_push(&note_results, overlay_item->cx, overlay_item->cy, result.type);
    }
}