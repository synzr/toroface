#include "clock_layer.h"

#define CLOCK_H 40
#define CLOCK_SPACE 2
#define CLOCK_OFFSET 4

typedef struct ClockLayerData {
  GFont font_big;
  GFont font_small;
} ClockLayerData;

static Layer *s_clock_layer;

static void clock_layer_update_proc(Layer *layer, GContext *ctx) {
  ClockLayerData *layer_data = layer_get_data(s_clock_layer);

  // Fill a whole layer with the black color.
  GRect layer_bounds = layer_get_unobstructed_bounds(layer);
  layer_bounds.origin = GPoint(0, 0);
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, layer_bounds, 0, GCornerNone);

  // Get the current time.
  time_t tmp = time(NULL);
  struct tm *time = localtime(&tmp);

  // Drawing the time and seconds.
  static char time_text[6], seconds_text[3] = "00\0";
  graphics_context_set_text_color(ctx, GColorWhite);

  strftime(time_text, sizeof(time_text), clock_is_24h_style() ? "%H:%M" : "%I:%M", time);
  GSize time_text_size = graphics_text_layout_get_content_size(
      time_text, layer_data->font_big, layer_bounds, GTextOverflowModeFill, GTextAlignmentLeft);

  seconds_text[0] = seconds_text[1] = '0';
  GSize seconds_text_size = graphics_text_layout_get_content_size(
      seconds_text, layer_data->font_small, layer_bounds, GTextOverflowModeFill, GTextAlignmentLeft);
  strftime(seconds_text, sizeof(seconds_text), "%S", time);

  GRect timestamp_rect =
      GRect(0, 0, time_text_size.w + seconds_text_size.w + CLOCK_SPACE, time_text_size.h);
  grect_align(&timestamp_rect, &layer_bounds, GAlignCenter, true);
  timestamp_rect.origin.y -= CLOCK_OFFSET;

  GRect time_rect =
      GRect(timestamp_rect.origin.x, timestamp_rect.origin.y, time_text_size.w, time_text_size.h);
  graphics_draw_text(ctx, time_text, layer_data->font_big, time_rect, GTextOverflowModeFill,
                     GTextAlignmentLeft, NULL);

  GRect seconds_rect = GRect(0, 0, seconds_text_size.w, seconds_text_size.h);
  grect_align(&seconds_rect, &timestamp_rect, GAlignBottomRight, true);
  graphics_draw_text(ctx, seconds_text, layer_data->font_small, seconds_rect, GTextOverflowModeFill,
                     GTextAlignmentLeft, NULL);
}

void clock_layer_init(Window *window, GPoint position) {
  Layer *window_root_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_unobstructed_bounds(window_root_layer);

  s_clock_layer =
      layer_create_with_data(GRect(position.x, position.y, window_bounds.size.w - position.x, CLOCK_H), sizeof(ClockLayerData));

  ClockLayerData *layer_data = layer_get_data(s_clock_layer);
  layer_data->font_big = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TORO_38));
  layer_data->font_small = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_KONEKO_TORO_28));

  layer_set_update_proc(s_clock_layer, clock_layer_update_proc);
  layer_add_child(window_root_layer, s_clock_layer);
}

void clock_layer_tick(void) {
  layer_mark_dirty(s_clock_layer);
}

void clock_layer_deinit(void) {
  ClockLayerData *layer_data = layer_get_data(s_clock_layer);
  fonts_unload_custom_font(layer_data->font_big);
  fonts_unload_custom_font(layer_data->font_small);
  layer_destroy(s_clock_layer);
}
