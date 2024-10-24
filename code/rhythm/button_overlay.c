#include "button_overlay.h"

#include <malloc.h>
#include <string.h>

void button_overlay_init(ButtonOverlay* overlay, RhythmResources* resources) {
    overlay->overlay_item_count = 0;
    overlay->overlay_items = NULL;
    overlay->resources = resources;
}

static void button_overlay_close(ButtonOverlay* overlay) {
    if (overlay->overlay_items) {
        free(overlay->overlay_items);
    }
}

void button_overlay_uninit(ButtonOverlay* overlay) {
    button_overlay_close(overlay);
}

static sprite_t* get_sprite_for_val(RhythmResources* resources, uintptr_t value) {
    switch (value)
    {
        case 'a': return resources->sprites[RHYTHM_SPRITE_BUTTON_A];
        case 'b': return resources->sprites[RHYTHM_SPRITE_BUTTON_B];
        case 'l': return resources->sprites[RHYTHM_SPRITE_BUTTON_L];
        case 'r': return resources->sprites[RHYTHM_SPRITE_BUTTON_R];
        case 'z': return resources->sprites[RHYTHM_SPRITE_BUTTON_Z];
        case '<': return resources->sprites[RHYTHM_SPRITE_BUTTON_CL];
        case '>': return resources->sprites[RHYTHM_SPRITE_BUTTON_CR];
        
        default:
            return NULL;
    }
}

static void fixup_overlay_item_textures(ButtonOverlay* overlay) {
    for (uint32_t i = 0; i < overlay->overlay_item_count; i++) {
        ButtonOverlayItem* item = &overlay->overlay_items[i];
        item->button1 = get_sprite_for_val(overlay->resources, (uintptr_t)item->button1);
        item->button2 = get_sprite_for_val(overlay->resources, (uintptr_t)item->button2);
    }
}

int button_overlay_open_f(ButtonOverlay* overlay, const char* path) {
    button_overlay_close(overlay);

    FILE* f = fopen(path, "rb");
    if (!f) {
        return 0;
    }

    fread(&overlay->overlay_item_count, sizeof(uint32_t), 1, f);
    debugf("load overlay: %lu\n", overlay->overlay_item_count);
    overlay->overlay_items = malloc(sizeof(ButtonOverlayItem) * overlay->overlay_item_count);
    fread(overlay->overlay_items, sizeof(ButtonOverlayItem), overlay->overlay_item_count, f);
    fixup_overlay_item_textures(overlay);

    return 1;
}

void button_overlay_load_debug(ButtonOverlay* overlay, ButtonOverlayItem* overlay_items, uint32_t item_count) {
    button_overlay_close(overlay);

    overlay->overlay_item_count = item_count;
    overlay->overlay_items = malloc(sizeof(ButtonOverlayItem) * item_count);
    memcpy(overlay->overlay_items, overlay_items, sizeof(ButtonOverlayItem) * item_count);

    fixup_overlay_item_textures(overlay);
}

void button_overlay_draw(ButtonOverlay* overlay) {
    for (size_t i = 0; i < overlay->overlay_item_count; i++) {
        ButtonOverlayItem* overlay_item = &overlay->overlay_items[i];

        if (!overlay_item->button2) {
            rdpq_sprite_blit(overlay_item->button1, 
                            overlay_item->cx - overlay_item->button1->width / 2, 
                            overlay_item->cy - overlay_item->button1->height /2 , 
                            NULL);
        } else {
            rdpq_sprite_blit(overlay_item->button1, 
                overlay_item->cx - overlay_item->button1->width / 2, 
                overlay_item->cy - overlay_item->button1->height, 
                NULL);
            rdpq_sprite_blit(overlay_item->button2, 
                overlay_item->cx - overlay_item->button1->width / 2, 
                overlay_item->cy, 
                NULL);
        }

    }
}
