#include "character_layer.h"

#define CHARACTER_H 80
#define CHARACTER_STEP 4

typedef struct CharacterLayerData {
  int ticks;
  GBitmap *even;
  GBitmap *odd;
} CharacterLayerData;

static Layer *s_character_layer;

static void character_layer_update_proc(Layer *layer, GContext *ctx) {
  CharacterLayerData *layer_data = layer_get_data(s_character_layer);

  // Get the character bitmap.
  GBitmap *bitmap;
  if (layer_data->ticks % 2 == 0) {
    bitmap = layer_data->even;
  } else {
    bitmap = layer_data->odd;
  }

  // Get the position of character bitmap and align it to bottom.
  GRect frame = gbitmap_get_bounds(bitmap);
  GRect frame_inside = layer_get_unobstructed_bounds(layer);
  grect_align(&frame, &frame_inside, GAlignBottom, true);

  // Move the character.
  switch (layer_data->ticks) {
    case 1:
      frame.origin.x += CHARACTER_STEP;  // to right
      break;
    case 3:
      frame.origin.x -= CHARACTER_STEP;  // to left
      break;
  }

  // Draw the character.
  graphics_context_set_compositing_mode(ctx, GCompOpSet);
  graphics_draw_bitmap_in_rect(ctx, bitmap, frame);
}

void character_layer_init(Window *window) {
  Layer *window_root_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_unobstructed_bounds(window_root_layer);

  s_character_layer =
      layer_create_with_data(GRect(0, window_bounds.size.h - CHARACTER_H, window_bounds.size.w, CHARACTER_H), sizeof(CharacterLayerData));

  CharacterLayerData *layer_data = layer_get_data(s_character_layer);
  layer_data->even = gbitmap_create_with_resource(RESOURCE_ID_CHARACTER_EVEN);
  layer_data->odd = gbitmap_create_with_resource(RESOURCE_ID_CHARACTER_ODD);
  layer_data->ticks = 0;

  layer_set_update_proc(s_character_layer, character_layer_update_proc);
  layer_add_child(window_root_layer, s_character_layer);
}

void character_layer_tick(void) {
  CharacterLayerData *layer_data = layer_get_data(s_character_layer);
  if (++layer_data->ticks == 4) {
    layer_data->ticks = 0;
  }
  layer_mark_dirty(s_character_layer);
}

void character_layer_deinit(void) {
  CharacterLayerData *layer_data = layer_get_data(s_character_layer);
  gbitmap_destroy(layer_data->even);
  gbitmap_destroy(layer_data->odd);
  layer_destroy(s_character_layer);
}
