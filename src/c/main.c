#include <pebble.h>
#include "main_window.h"
#include "resources_service.h"

inline void init(void) {
  resources_service_init();
  main_window_init();
}

inline void deinit(void) {
  main_window_deinit();
  resources_service_deinit();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
