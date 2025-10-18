#include "resources_service.h"

typedef struct ResourcesService {
  GFont font_koneko_toro;
  GFont font_toro;
  GBitmap *character_even;
  GBitmap *character_odd;
} ResourcesService;

static ResourcesService *s_resources_service;

void resources_service_init(void) {
  s_resources_service = malloc(sizeof(ResourcesService));
  s_resources_service->font_koneko_toro =
      fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_KONEKO_TORO_24));
  s_resources_service->font_toro =
      fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TORO_36));
  s_resources_service->character_even = gbitmap_create_with_resource(RESOURCE_ID_CHARACTER_EVEN);
  s_resources_service->character_odd = gbitmap_create_with_resource(RESOURCE_ID_CHARACTER_ODD);
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

GFont resources_service_get_custom_font(CustomFont font) {
  switch (font) {
    case CustomFontKonekoToro:
      return s_resources_service->font_koneko_toro;
    case CustomFontToro:
      return s_resources_service->font_toro;
  }
  return NULL;
}
