#include "clock_layer.h"

#define CLOCK_COMMON_SPACE 2
#define CLOCK_BETWEEN_HM_SPACE 8

static GFont s_clock_font_big;
static GFont s_clock_font_small;
static Layer *s_clock_layer;
static char s_clock_measure_buffer[4];

static void clock_layer_update_proc(Layer *layer, GContext *ctx) {
  // Fill a whole layer with the black color.
  GRect layer_bounds = layer_get_unobstructed_bounds(layer);
  layer_bounds.origin = GPoint(0, 0);
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, layer_bounds, 0, GCornerNone);

  // Get the current time.
  time_t tmp = time(NULL);
  struct tm *time = localtime(&tmp);

  // Drawing the measures.
  graphics_context_set_text_color(ctx, GColorWhite);

  GSize big_text_size = graphics_text_layout_get_content_size(
      "00", s_clock_font_big, layer_bounds, GTextOverflowModeFill, GTextAlignmentLeft);
  GSize small_text_size = graphics_text_layout_get_content_size(
      "00", s_clock_font_small, layer_bounds, GTextOverflowModeFill, GTextAlignmentLeft);

  int clock_w =
      (big_text_size.w * 2) + small_text_size.w + CLOCK_COMMON_SPACE + CLOCK_BETWEEN_HM_SPACE;
  int clock_w_pad = (layer_bounds.size.w - clock_w) / 2;

  strftime(s_clock_measure_buffer, sizeof(s_clock_measure_buffer),
           clock_is_24h_style() ? "%H" : "%I", time);
  graphics_draw_text(ctx, s_clock_measure_buffer, s_clock_font_big,
                     GRect(clock_w_pad, 1, big_text_size.w, big_text_size.h),
                     GTextOverflowModeFill, GTextAlignmentLeft, NULL);

  strftime(s_clock_measure_buffer, sizeof(s_clock_measure_buffer), "%M", time);
  graphics_draw_text(ctx, s_clock_measure_buffer, s_clock_font_big,
                     GRect(clock_w_pad + big_text_size.w + CLOCK_BETWEEN_HM_SPACE, 1,
                           big_text_size.w, big_text_size.h),
                     GTextOverflowModeFill, GTextAlignmentLeft, NULL);

  strftime(s_clock_measure_buffer, sizeof(s_clock_measure_buffer), "%S", time);
  graphics_draw_text(
      ctx, s_clock_measure_buffer, s_clock_font_small,
      GRect(clock_w_pad + (big_text_size.w * 2) + CLOCK_BETWEEN_HM_SPACE + CLOCK_COMMON_SPACE,
            small_text_size.h - 12, small_text_size.w, small_text_size.h),
      GTextOverflowModeFill, GTextAlignmentLeft, NULL);

  // Draw the colon between hours and minutes;
  graphics_context_set_fill_color(ctx, GColorWhite);

  int colon_x = clock_w_pad + big_text_size.w - 2;
  graphics_fill_rect(ctx, GRect(colon_x, (big_text_size.h / 4) + 3, 6, 6), 0, GCornerNone);
  graphics_fill_rect(ctx, GRect(colon_x, (big_text_size.h / 4) + (big_text_size.h / 2) - 3, 6, 6),
                     0, GCornerNone);
}

void clock_layer_init(Window *window, GPoint position) {
  s_clock_font_big = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_CLOCK_FONT_38));
  s_clock_font_small = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_CLOCK_FONT_26));

  Layer *window_root_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_unobstructed_bounds(window_root_layer);

  s_clock_layer =
      layer_create(GRect(position.x, position.y, window_bounds.size.w - position.x, 40));
  layer_set_update_proc(s_clock_layer, clock_layer_update_proc);
  layer_add_child(window_root_layer, s_clock_layer);
}

void clock_layer_tick(void) {
  layer_mark_dirty(s_clock_layer);
}

void clock_layer_deinit(void) {
  layer_destroy(s_clock_layer);
  fonts_unload_custom_font(s_clock_font_big);
  fonts_unload_custom_font(s_clock_font_small);
}
