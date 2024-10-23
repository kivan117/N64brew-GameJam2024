#include "button_overlay.h"

#include <malloc.h>
#include <string.h>

void button_overlay_init(ButtonOverlay* overlay) {
    overlay->overlay_item_count = 0;
    overlay->overlay_items = NULL;
    overlay->a_button = sprite_load("rom:/core/AButton.sprite");
    overlay->b_button = sprite_load("rom:/core/BButton.sprite");
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

static void fixup_overlay_item_textures(ButtonOverlay* overlay) {
    for (uint32_t i = 0; i < overlay->overlay_item_count; i++) {
        ButtonOverlayItem* item = &overlay->overlay_items[i];

        debugf("%lu: x: %f y: %f (%u)\n", i, item->cx, item->cy, (uintptr_t)item->button);

        switch ((uintptr_t)item->button)
        {
            case 'a': item->button = overlay->a_button; break;
            case 'b': item->button = overlay->b_button; break;
            
            default: item->button = NULL;
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

        rdpq_sprite_blit(overlay_item->button, overlay_item->cx - overlay_item->button->width / 2, overlay_item->cy - overlay_item->button->height /2 , NULL);
    }
}
