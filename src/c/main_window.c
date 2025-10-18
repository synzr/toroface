#include "main_window.h"
#include "date_layer.h"
#include "character_layer.h"
#include "clock_layer.h"

static Window *s_main_window;

static void main_window_tick(struct tm *time, TimeUnits units) {
  date_layer_tick();
  clock_layer_tick();
  character_layer_tick();
}

static void main_window_load(Window *window) {
  Layer *layer = window_get_root_layer(window);

  date_layer_init(layer, 0);
  clock_layer_init(layer, 40);
  character_layer_init(layer);

  tick_timer_service_subscribe(SECOND_UNIT, main_window_tick);
}

static void main_window_unload(Window *window) {
  tick_timer_service_unsubscribe();

  clock_layer_deinit();
  date_layer_deinit();
  character_layer_deinit();
}

void main_window_init(void) {
  s_main_window = window_create();
  window_set_window_handlers(
      s_main_window, (WindowHandlers){.load = main_window_load, .unload = main_window_unload});
  window_stack_push(s_main_window, true);
}

void main_window_deinit(void) {
  window_stack_remove(s_main_window, true);
  window_destroy(s_main_window);
}
