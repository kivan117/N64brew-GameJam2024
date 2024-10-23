#include "button_overlay.h"

#include <malloc.h>
#include <string.h>

void button_overlay_init(ButtonOverlay* overlay) {
    overlay->overlay_item_count = 0;
    overlay->overlay_items = NULL;
    overlay->a_button = sprite_load("rom:/core/AButton.sprite");
    overlay->b_button = sprite_load("rom:/core/BButton.sprite");
    overlay->l_button = sprite_load("rom:/core/LTrigger.sprite");
    overlay->r_button = sprite_load("rom:/core/RTrigger.sprite");
    overlay->z_button = sprite_load("rom:/core/ZTrigger.sprite");
    overlay->cr_button = sprite_load("rom:/core/CRight.sprite");
    overlay->cl_button = sprite_load("rom:/core/CLeft.sprite");
}

static void button_overlay_close(ButtonOverlay* overlay) {
    if (overlay->overlay_items) {
        free(overlay->overlay_items);
    }
}

void button_overlay_uninit(ButtonOverlay* overlay) {
    button_overlay_close(overlay);
    sprite_free(overlay->a_button);
    sprite_free(overlay->b_button);
}

// TODO: clean this up
static void fixup_overlay_item_textures(ButtonOverlay* overlay) {
    for (uint32_t i = 0; i < overlay->overlay_item_count; i++) {
        ButtonOverlayItem* item = &overlay->overlay_items[i];

        switch ((uintptr_t)item->button1)
        {
            case 'a': item->button1 = overlay->a_button; break;
            case 'b': item->button1 = overlay->b_button; break;
            case 'l': item->button1 = overlay->l_button; break;
            case 'r': item->button1 = overlay->r_button; break;
            case 'z': item->button1 = overlay->z_button; break;
            case '<': item->button1 = overlay->cl_button; break;
            case '>': item->button1 = overlay->cr_button; break;
            
            default:
                item->button1 = NULL;
        }

        switch ((uintptr_t)item->button2)
        {
            case 'a': item->button2 = overlay->a_button; break;
            case 'b': item->button2 = overlay->b_button; break;
            case 'l': item->button2 = overlay->l_button; break;
            case 'r': item->button2 = overlay->r_button; break;
            case 'z': item->button2 = overlay->z_button; break;
            case '<': item->button2 = overlay->cl_button; break;
            case '>': item->button2 = overlay->cr_button; break;
            
            default: 
                item->button2 = NULL;
        }
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
