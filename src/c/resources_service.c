#include "resources_service.h"

#define RESOURCE_ID_FONT_KONEKO_TORO RESOURCE_ID_FONT_KONEKO_TORO_24
#define RESOURCE_ID_FONT_TORO RESOURCE_ID_FONT_TORO_36

typedef struct ResourcesService {
  GFont font_koneko_toro;
  GFont font_toro;
  GBitmap *character_even;
  GBitmap *character_odd;
  GBitmap *icon_battery_empty;
  GBitmap *icon_battery_half;
  GBitmap *icon_battery_full;
  GBitmap *icon_paws;
} ResourcesService;

static ResourcesService *s_resources_service;

void resources_service_init(void) {
  s_resources_service = malloc(sizeof(ResourcesService));

  s_resources_service->font_koneko_toro =
      fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_KONEKO_TORO));
  s_resources_service->font_toro =
      fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TORO));

  s_resources_service->character_even = gbitmap_create_with_resource(RESOURCE_ID_CHARACTER_EVEN);
  s_resources_service->character_odd = gbitmap_create_with_resource(RESOURCE_ID_CHARACTER_ODD);

  s_resources_service->icon_battery_empty =
      gbitmap_create_with_resource(RESOURCE_ID_ICON_BATTERY_EMPTY);
  s_resources_service->icon_battery_full =
      gbitmap_create_with_resource(RESOURCE_ID_ICON_BATTERY_FULL);
  s_resources_service->icon_battery_half =
      gbitmap_create_with_resource(RESOURCE_ID_ICON_BATTERY_HALF);
  s_resources_service->icon_paws = gbitmap_create_with_resource(RESOURCE_ID_ICON_PAWS);
}

void resources_service_deinit(void) {
  fonts_unload_custom_font(s_resources_service->font_koneko_toro);
  fonts_unload_custom_font(s_resources_service->font_toro);

  gbitmap_destroy(s_resources_service->character_even);
  gbitmap_destroy(s_resources_service->character_odd);

  free(s_resources_service);
}

GBitmap *resources_service_get_character(int ticks) {
  if (ticks % 2 == 0) {
    return s_resources_service->character_even;
  }
  return s_resources_service->character_odd;
}

GBitmap *resources_service_get_custom_icon(CustomIcon icon) {
  switch (icon) {
    case CustomIconBatteryEmpty:
      return s_resources_service->icon_battery_empty;
    case CustomIconBatteryFull:
      return s_resources_service->icon_battery_full;
    case CustomIconBatteryHalf:
      return s_resources_service->icon_battery_half;
    case CustomIconPaws:
      return s_resources_service->icon_paws;
  }
  return NULL;
}

GFont resources_service_get_custom_font(CustomFont font) {
  switch (font) {
    case CustomFontKonekoToro:
      return s_resources_service->font_koneko_toro;
    case CustomFontToro:
      return s_resources_service->font_toro;
  }
  return fonts_get_system_font(FONT_KEY_FONT_FALLBACK);
}
