#include <libdragon.h>
#include "../../core.h"
#include "../../minigame.h"

#include "note_results.h"

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

wav64_t junkie_loop;
sprite_t* a_button;
sprite_t* b_button;
sprite_t* indicator_sprite;
rdpq_font_t *font;

typedef struct {
    float cx, cy;
    int indicator_active;
    sprite_t* button;
} Note;

typedef struct {
    Note* note;
    float time_remaining;
} Indicator;

#define INDICATOR_LIFETIME 0.3f

#define INDICATOR_COUNT 2
#define NOTE_COUNT 8

static void note_init(Note* note, float cx, float cy, sprite_t* button) {
    note->cx = cx;
    note->cy = cy;
    note->indicator_active = 0;
    note->button = button;
}

#define GAME_BACKGROUND     0x000000FF
static uint32_t background_color = GAME_BACKGROUND;

static Indicator indicators[INDICATOR_COUNT];
static Note notes[NOTE_COUNT];
static Simfile simfile;
static SimfileContext context;
static SimfileInputTracker tracker;
static NoteResults note_results;
static size_t next_note = 0;

static void draw_notes();
static void draw_indicators();
static void show_next_indicator(const SimfileEvent* event, void* arg);

static void update_indicators(float deltatime);
static void draw_indicators();

static int player_controller_get_button_pressed(int button, void* arg);
static void process_input();

/*==============================
    minigame_init
    The minigame initialization function
==============================*/
void minigame_init()
{
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, FILTERS_RESAMPLE);
    wav64_open(&junkie_loop, "rom:/rhythm/tabloid_junkie.wav64");
    a_button = sprite_load("rom:/core/AButton.sprite");
    b_button = sprite_load("rom:/core/BButton.sprite");
    indicator_sprite = sprite_load("rom:/rhythm/indicator.sprite");
    font = rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_VAR);
    rdpq_text_register_font(1, font);
    #define TEXT_COLOR          0x6CBB3CFF
    rdpq_font_style(font, 0, &(rdpq_fontstyle_t){.color = color_from_packed32(TEXT_COLOR) });

    memset(indicators, 0, sizeof(indicators));

    note_init(&notes[0], 40.0f, 40.0f, a_button);
    note_init(&notes[1], 80.0f, 40.0f, a_button);
    note_init(&notes[2], 120.0f, 40.0f, b_button);

    note_init(&notes[3], 60.0f, 80.0f, a_button);
    note_init(&notes[4], 100.0f, 80.0f, a_button);
    note_init(&notes[5], 140.0f, 80.0f, b_button);

    note_init(&notes[6], 80.0f, 120.0f, a_button);
    note_init(&notes[7], 120.0f, 120.0f, b_button);

    simfile_open(&simfile, "rom:/rhythm/tabloid_junkie.csm");
    simfile_context_init(&context, &simfile, INDICATOR_LIFETIME);
    simfile_context_push_callback(&context, show_next_indicator, NULL);

    SimfileInputTrackerInterface input_interface = {player_controller_get_button_pressed, 0};
    simfile_input_tracker_init(&tracker, &context, &input_interface);

    note_results_init(&note_results);
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

    update_indicators(deltatime);
    simfile_context_update(&context, deltatime);
    note_results_update(&note_results, deltatime);
    process_input(deltatime);

    joypad_buttons_t btn = joypad_get_buttons_pressed(0);
    // if the song is finished restart it
    if (simfile_context_finished(&context) && (btn.raw & SIMFILE_INPUT_TRACKER_BUTTON_START)) {
        memset(indicators, 0, sizeof(indicators));
        mixer_ch_stop(1);
        next_note = 0;
        simfile_context_reset(&context);
        simfile_input_tracker_reset(&tracker);
        note_results_init(&note_results);
    }

    // temporary fix
    if (!simfile_context_finished(&context) && context.current_time > 0 && !mixer_ch_playing(1)) {
        wav64_play(&junkie_loop, 1);
    }
    
    draw_indicators();
    draw_notes();
    note_results_draw(&note_results, font);

    rdpq_detach_show();
}

/*==============================
    minigame_cleanup
    Clean up any memory used by your game just before it ends.
==============================*/
void minigame_cleanup()
{
    wav64_close(&junkie_loop);
    sprite_free(a_button);
    sprite_free(b_button);
    sprite_free(indicator_sprite);
    rdpq_text_unregister_font(1);
}

static void show_next_indicator(const SimfileEvent* event, void* arg) {
    Note* note = &notes[next_note++];
    // get next indicator
    Indicator* indicator = NULL;
    for (size_t i = 0; i < INDICATOR_COUNT; i++) {
        if (indicators[i].time_remaining <= 0) {
            indicator = &indicators[i];
        }
    }

    if (!indicator) {
        return; // should not happen
    }

    indicator->note = note;
    indicator->time_remaining = event->time - context.current_time;

    simfile_input_tracker_enqueue(&tracker, event);
}

void update_indicators(float deltatime) {
    for (size_t i = 0; i < INDICATOR_COUNT; i++) {
        Indicator* indicator = indicators + i;

        if (indicator->time_remaining <= 0) {
            continue;
        }
        
        indicator->time_remaining = indicator->time_remaining - deltatime;
    }
}

void draw_indicators() {
    rdpq_blitparms_t params;
    memset(&params, 0, sizeof(rdpq_blitparms_t));

    for (size_t i = 0; i < INDICATOR_COUNT; i++) {
        Indicator* indicator = indicators + i;

        if (indicator->time_remaining <= 0) {
            continue;
        }

        float indicator_scale = 0.5f + ((indicator->time_remaining / INDICATOR_LIFETIME)* 0.5f) ;

        float indicator_x = indicator->note->cx - (indicator_sprite->width / 2) * indicator_scale;
        float indicator_y = indicator->note->cy - (indicator_sprite->height / 2) * indicator_scale;

        params.scale_x = indicator_scale;
        params.scale_y = indicator_scale;

        rdpq_sprite_blit(indicator_sprite, indicator_x, indicator_y, &params);
    }
}

static void draw_notes() {
    for (size_t i = 0; i < NOTE_COUNT; i++) {
        Note* note = &notes[i];

        rdpq_sprite_blit(note->button, note->cx - note->button->width / 2, note->cy - note->button->height /2 , NULL);
    }
}

int player_controller_get_button_pressed(int button, void* arg) {
    uint32_t port = (uint32_t)arg;
    joypad_buttons_t btn = joypad_get_buttons_pressed(port);

    return btn.raw & (uint32_t)button;
}

void process_input() {
    SimfileInputTrackerResult result = simfile_input_tracker_update(&tracker);

    if (result.type != INPUT_TRACKER_RESULT_NONE) {
        Note* note = &notes[result.event_index];
        note_results_push(&note_results, note->cx, note->cy, result.type);
    }
}