#ifndef INFORMATION_LAYER_H_
#define INFORMATION_LAYER_H_

#include <pebble.h>

void information_layer_init(Layer *layer, int y);
void information_layer_tick(void);
void information_layer_deinit(void);

#endif // INFORMATION_LAYER_H_
