#ifndef DATE_LAYER_H_
#define DATE_LAYER_H_

#include <pebble.h>

void date_layer_init(Layer *layer, int y);
void date_layer_tick(void);
void date_layer_deinit(void);

#endif // DATE_LAYER_H_
