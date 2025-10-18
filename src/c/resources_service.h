#ifndef RESOURCES_SERVICE_H_
#define RESOURCES_SERVICE_H_

#include <pebble.h>

typedef enum CustomFont {
  CustomFontKonekoToro,
  CustomFontToro
} CustomFont;

typedef enum CustomIcon {
  CustomIconBatteryEmpty,
  CustomIconBatteryFull,
  CustomIconBatteryHalf,
  CustomIconPaws
} CustomIcon;

void resources_service_init(void);
void resources_service_deinit(void);

GBitmap *resources_service_get_character(int ticks);
GBitmap *resources_service_get_custom_icon(CustomIcon icon);
GFont resources_service_get_custom_font(CustomFont font);

#endif
