#include <pebble.h>
#include "main_window.h"

inline void init(void) {
  main_window_init();
}

inline void deinit(void) {
  main_window_deinit();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
