#include <libdragon.h>
#include "../../core.h"
#include "../../minigame.h"

#include "simfile/simfile.h"
#include "simfile/simfile_context.h"

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

static Indicator indicators[INDICATOR_COUNT];
static Note notes[NOTE_COUNT];
static Simfile simfile;
static SimfileContext context;
static size_t next_note = 0;

static void draw_notes();
static void draw_indicators();
static void show_next_indicator(const SimfileEvent* event, void* arg);

static void update_indicators(float deltatime);
static void draw_indicators();

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
    update_indicators(deltatime);
    simfile_context_update(&context, deltatime);

    // if the song is finished restart it
    if (simfile_context_finished(&context)) {
        memset(indicators, 0, sizeof(indicators));
        mixer_ch_stop(1);
        next_note = 0;
        simfile_context_reset(&context);
    }

    // temporary fix
    if (context.current_time > 0 && !mixer_ch_playing(1)) {
        wav64_play(&junkie_loop, 1);
    }
}

/*==============================
    minigame_loop
    Code that is called every loop.
    @param  The delta time for this tick
==============================*/
void minigame_loop(float deltatime)
{
    rdpq_attach(display_get(), NULL);
    rdpq_clear(color_from_packed32(GAME_BACKGROUND));

    rdpq_set_mode_standard();
    rdpq_mode_alphacompare(1);
    
    draw_indicators();
    draw_notes();

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