#include "bar_layer.h"

#include "resources_service.h"

#define BAR_PAD_W 8

static Layer *s_bar_layer;

static void bar_layer_update_proc(Layer *layer, GContext *ctx) {
  GRect window_rect = layer_get_unobstructed_bounds(layer);

  time_t tmp = time(NULL);
  struct tm *time = localtime(&tmp);

  static char date_text[6], weekday_text[4];
  GFont font = resources_service_get_custom_font(CustomFontKonekoToro);

  strftime(date_text, sizeof(date_text), "%m/%d", time);
  GSize date_text_size = graphics_text_layout_get_content_size(
      date_text, font, window_rect, GTextOverflowModeFill, GTextAlignmentLeft);
  GRect date_text_rect = GRect(0, 0, date_text_size.w, date_text_size.h);
  grect_align(&date_text_rect, &window_rect, GAlignLeft, true);

  strftime(weekday_text, sizeof(weekday_text), "%a", time);
  GSize weekday_text_size = graphics_text_layout_get_content_size(
      weekday_text, font, window_rect, GTextOverflowModeFill, GTextAlignmentRight);
  GRect weekday_text_rect = GRect(0, 0, weekday_text_size.w, weekday_text_size.h);
  grect_align(&weekday_text_rect, &window_rect, GAlignRight, true);

  graphics_context_set_text_color(ctx, GColorBlack);
  graphics_draw_text(ctx, date_text, font, date_text_rect, GTextOverflowModeFill,
                     GTextAlignmentLeft, NULL);
  graphics_draw_text(ctx, weekday_text, font, weekday_text_rect, GTextOverflowModeFill,
                     GTextAlignmentRight, NULL);
}

void bar_layer_init(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_rect = layer_get_unobstructed_bounds(window_layer);

  s_bar_layer = layer_create(GRect(BAR_PAD_W, 0, window_rect.size.w - (BAR_PAD_W * 2), 28));
  layer_set_update_proc(s_bar_layer, bar_layer_update_proc);

  layer_add_child(window_layer, s_bar_layer);
}

void bar_layer_tick(void) {
  layer_mark_dirty(s_bar_layer);
}

void bar_layer_deinit(void) {
  layer_destroy(s_bar_layer);
}
