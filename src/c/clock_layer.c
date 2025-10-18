#include "clock_layer.h"

#include "resources_service.h"

#define CLOCK_H 40
#define CLOCK_SPACE 2
#define CLOCK_OFFSET 4

static Layer *s_clock_layer;
static char s_clock_time_text[6] = "00:00";
static char s_clock_seconds_text[3] = "00";

static void clock_layer_update_proc(Layer *layer, GContext *ctx) {
  // Fill a whole layer with the black color.
  GRect layer_bounds = layer_get_unobstructed_bounds(layer);
  layer_bounds.origin = GPoint(0, 0);
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, layer_bounds, 0, GCornerNone);

  // Get the current time.
  time_t tmp = time(NULL);
  struct tm *time = localtime(&tmp);

  // Drawing the time and seconds.
  GFont font_small = resources_service_get_custom_font(CustomFontKonekoToro);
  GFont font_big = resources_service_get_custom_font(CustomFontToro);

  graphics_context_set_text_color(ctx, GColorWhite);

  strftime(s_clock_time_text, sizeof(s_clock_time_text), clock_is_24h_style() ? "%H:%M" : "%I:%M",
           time);
  GSize time_text_size = graphics_text_layout_get_content_size(
      s_clock_time_text, font_big, layer_bounds, GTextOverflowModeFill, GTextAlignmentLeft);

  GSize seconds_text_size = graphics_text_layout_get_content_size(
      "00", font_small, layer_bounds, GTextOverflowModeFill, GTextAlignmentLeft);
  strftime(s_clock_seconds_text, sizeof(s_clock_seconds_text), "%S", time);

  GRect timestamp_rect =
      GRect(0, 0, time_text_size.w + seconds_text_size.w + CLOCK_SPACE, time_text_size.h);
  grect_align(&timestamp_rect, &layer_bounds, GAlignCenter, true);
  timestamp_rect.origin.y -= CLOCK_OFFSET;

  GRect time_rect =
      GRect(timestamp_rect.origin.x, timestamp_rect.origin.y, time_text_size.w, time_text_size.h);
  graphics_draw_text(ctx, s_clock_time_text, font_big, time_rect, GTextOverflowModeFill,
                     GTextAlignmentLeft, NULL);

  GRect seconds_rect = GRect(0, 0, seconds_text_size.w, seconds_text_size.h);
  grect_align(&seconds_rect, &timestamp_rect, GAlignBottomRight, true);
  graphics_draw_text(ctx, s_clock_seconds_text, font_small, seconds_rect, GTextOverflowModeFill,
                     GTextAlignmentLeft, NULL);
}

void clock_layer_init(Layer *layer, int y) {
  GRect rect = layer_get_unobstructed_bounds(layer);

  s_clock_layer = layer_create(GRect(0, y, rect.size.w, CLOCK_H));
  layer_set_update_proc(s_clock_layer, clock_layer_update_proc);
  layer_add_child(layer, s_clock_layer);
}

GRect clock_layer_get_bounds() {
  return layer_get_bounds(s_clock_layer);
}

void clock_layer_tick(void) {
  layer_mark_dirty(s_clock_layer);
}

void clock_layer_deinit(void) {
  layer_destroy(s_clock_layer);
}
