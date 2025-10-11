#ifndef CLOCK_LAYER_H_
#define CLOCK_LAYER_H_

#include <pebble.h>

void clock_layer_init(Window *window, GPoint position);
void clock_layer_update(void);
void clock_layer_deinit(void);

#endif
