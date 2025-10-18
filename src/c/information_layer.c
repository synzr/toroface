#include "information_layer.h"

#include "resources_service.h"

#define INFO_LAYER_ICON_WIDTH 18
#define INFO_LAYER_PADDING 4
#define INFO_LAYER_HEIGHT 24

typedef struct InformationLayerData {
  int level;
  int steps;
} InformationLayerData;

static Layer *s_information_layer;
static char s_information_layer_level_text[3];
static char s_information_layer_count_text[6];

static void information_layer_update_proc(Layer *layer, GContext *ctx) {
  GRect rect = layer_get_unobstructed_bounds(layer);
  GFont font = resources_service_get_custom_font(CustomFontKonekoToro);
  InformationLayerData *data = layer_get_data(s_information_layer);

  int content_w = INFO_LAYER_ICON_WIDTH + INFO_LAYER_PADDING;

  snprintf(s_information_layer_level_text, sizeof(s_information_layer_level_text), "%d",
           data->level);
  GSize text_size = graphics_text_layout_get_content_size(
      s_information_layer_level_text, font, rect, GTextOverflowModeFill, GTextAlignmentLeft);
  content_w += text_size.w;

#ifdef PBL_HEALTH
  snprintf(s_information_layer_count_text, sizeof(s_information_layer_count_text), "%d",
           data->steps);
  text_size = graphics_text_layout_get_content_size(s_information_layer_count_text, font, rect,
                                                    GTextOverflowModeFill, GTextAlignmentLeft);
  content_w += INFO_LAYER_ICON_WIDTH + INFO_LAYER_PADDING + text_size.w;
#endif

  int content_x = (rect.size.w - content_w) / 2;

  /* Draw the battery icon based on level percentage. */
  graphics_context_set_compositing_mode(ctx, GCompOpSet);

  GBitmap *battery_icon;
  if (data->level < 20) {
    battery_icon = resources_service_get_custom_icon(CustomIconBatteryEmpty);
  } else if (data->level < 80) {
    battery_icon = resources_service_get_custom_icon(CustomIconBatteryHalf);
  } else {
    battery_icon = resources_service_get_custom_icon(CustomIconBatteryFull);
  }

  GRect battery_rect_inside = GRect(0, 0, INFO_LAYER_ICON_WIDTH, rect.size.h);
  GRect battery_rect = gbitmap_get_bounds(battery_icon);
  grect_align(&battery_rect, &battery_rect_inside, GAlignCenter, true);
  battery_rect.origin.x += content_x;
  graphics_draw_bitmap_in_rect(ctx, battery_icon, battery_rect);

  /* Draw the battery level in text. */
  GRect text_rect =
      GRect(content_x + INFO_LAYER_ICON_WIDTH + INFO_LAYER_PADDING, -4, text_size.w, text_size.h);

  graphics_context_set_text_color(ctx, GColorBlack);
  graphics_draw_text(ctx, s_information_layer_level_text, font, text_rect, GTextOverflowModeFill,
                     GTextAlignmentLeft, NULL);

#ifdef PBL_HEALTH
  /* Draw the paws icon. */
  GBitmap *paws_icon = resources_service_get_custom_icon(CustomIconPaws);
  GRect paws_rect = gbitmap_get_bounds(paws_icon);
  paws_rect.origin.x = content_x + text_rect.size.w;
  graphics_draw_bitmap_in_rect(ctx, paws_icon, paws_rect);

  /* Draw the step count in text. */
  text_rect = GRect(paws_rect.origin.x + INFO_LAYER_ICON_WIDTH + INFO_LAYER_PADDING, -4,
                    text_size.w, text_size.h);
  graphics_context_set_text_color(ctx, GColorBlack);
  graphics_draw_text(ctx, s_information_layer_count_text, font, text_rect, GTextOverflowModeFill,
                     GTextAlignmentLeft, NULL);
#endif  // PBL_HEALTH
}

static void battery_state_service_callback(BatteryChargeState charge) {
  InformationLayerData *data = layer_get_data(s_information_layer);
  data->level = charge.charge_percent;
  layer_mark_dirty(s_information_layer);
}

void information_layer_init(Layer *layer, int y) {
  GRect rect = layer_get_unobstructed_bounds(layer);

  s_information_layer = layer_create_with_data(GRect(0, y, rect.size.w, INFO_LAYER_HEIGHT),
                                               sizeof(InformationLayerData));

  InformationLayerData *data = layer_get_data(s_information_layer);
  data->level = battery_state_service_peek().charge_percent;
  data->steps = PBL_IF_HEALTH_ELSE(health_service_sum_today(HealthMetricStepCount), 0);

  layer_set_update_proc(s_information_layer, information_layer_update_proc);
  layer_add_child(layer, s_information_layer);

  battery_state_service_subscribe(battery_state_service_callback);
}

void information_layer_tick(void) {
#ifdef PBL_HEALTH
  InformationLayerData *data = layer_get_data(s_information_layer);
  data->steps = health_service_sum_today(HealthMetricStepCount);
  layer_mark_dirty(s_information_layer);
#endif  // PBL_HEALTH
}

void information_layer_deinit(void) {
  layer_destroy(s_information_layer);
}
