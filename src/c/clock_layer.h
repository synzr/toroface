#ifndef CLOCK_LAYER_H_
#define CLOCK_LAYER_H_

#include <pebble.h>

void clock_layer_init(Layer *layer, int y);
GRect clock_layer_get_bounds();
void clock_layer_tick(void);
void clock_layer_deinit(void);

#endif // CLOCK_LAYER_H_
