#ifndef BAR_LAYER_H_
#define BAR_LAYER_H_

#include <pebble.h>

void bar_layer_init(Layer *layer, int y);
void bar_layer_tick(void);
void bar_layer_deinit(void);

#endif
