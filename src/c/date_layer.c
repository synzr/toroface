#include "date_layer.h"
#include "resources_service.h"

#define PAD_W 8

static Layer *s_date_layer;

static void date_layer_update_proc(Layer *layer, GContext *ctx) {
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

void date_layer_init(Layer *layer, int y) {
  GRect rect = layer_get_unobstructed_bounds(layer);

  s_date_layer = layer_create(GRect(PAD_W, y, rect.size.w - (PAD_W * 2), 28));
  layer_set_update_proc(s_date_layer, date_layer_update_proc);
  layer_add_child(layer, s_date_layer);
}

void date_layer_tick(void) {
  layer_mark_dirty(s_date_layer);
}

void date_layer_deinit(void) {
  layer_destroy(s_date_layer);
}
