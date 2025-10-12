#include "character_layer.h"

#define CHARACTER_H 80
#define CHARACTER_STEP 4

static Layer *s_character_layer;
static GBitmap *s_character_even;
static GBitmap *s_character_odd;
static int s_character_ticks;

static void character_layer_update_proc(Layer *layer, GContext *ctx) {
  // Get the character bitmap.
  GBitmap *bitmap;
  if (s_character_ticks % 2 == 0) {
    bitmap = s_character_even;
  } else {
    bitmap = s_character_odd;
  }

  // Get the position of character bitmap and align it to bottom.
  GRect frame = gbitmap_get_bounds(bitmap);
  GRect frame_inside = layer_get_unobstructed_bounds(layer);
  grect_align(&frame, &frame_inside, GAlignBottom, true);

  // Move the character.
  switch (s_character_ticks) {
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
  s_character_ticks = 0;

  s_character_even = gbitmap_create_with_resource(RESOURCE_ID_CHARACTER_EVEN);
  s_character_odd = gbitmap_create_with_resource(RESOURCE_ID_CHARACTER_ODD);

  Layer *window_root_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_unobstructed_bounds(window_root_layer);

  s_character_layer =
      layer_create(GRect(0, window_bounds.size.h - CHARACTER_H, window_bounds.size.w, CHARACTER_H));
  layer_set_update_proc(s_character_layer, character_layer_update_proc);

  layer_add_child(window_root_layer, s_character_layer);
}

void character_layer_tick(void) {
  if (++s_character_ticks == 4) {
    s_character_ticks = 0;
  }
  layer_mark_dirty(s_character_layer);
}

void character_layer_deinit(void) {
  layer_destroy(s_character_layer);
  gbitmap_destroy(s_character_even);
  gbitmap_destroy(s_character_odd);
}
