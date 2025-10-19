#ifndef INFO_LAYER_H_
#define INFO_LAYER_H_

#include <pebble.h>

void info_layer_init(Layer *layer, int y);
void info_layer_tick(void);
void info_layer_deinit(void);

#endif  // INFO_LAYER_H_
