#include <ntk/font/pango/layout.h>

static NtkPangoLayoutFont* ntk_pango_layout_font_copy(NtkPangoLayoutFont* pfont) {
  return (NtkPangoLayoutFont*)ntk_user_font_copy((NtkUserFont*)pfont);
}

static void ntk_pango_layout_font_free(NtkPangoLayoutFont* pfont) {
  ntk_user_font_free((NtkUserFont*)pfont);
}

G_DEFINE_BOXED_TYPE(NtkPangoLayoutFont, ntk_pango_layout_font, ntk_pango_layout_font_copy, ntk_pango_layout_font_free);

NtkUserFont* ntk_pango_layout_font_impl_copy(NtkUserFont* font) {
  PangoLayout* layout = PANGO_LAYOUT(g_object_ref(G_OBJECT(font->parent_instance.userdata.ptr)));
  PangoFontDescription* font_desc = pango_font_description_copy(pango_layout_get_font_description(layout));
  return (NtkUserFont*)ntk_pango_layout_font_new(layout, font_desc);
}

void ntk_pango_layout_font_impl_free(NtkUserFont* font) {
  g_object_unref(font->parent_instance.userdata.ptr);
  g_free(font);
}

static float ntk_pango_layout_font_get_width(nk_handle userdata, float height, const char* text, int length) {
  PangoLayout* font_layout = PANGO_LAYOUT(userdata.ptr);

  const PangoFontDescription* font_desc = pango_layout_get_font_description(font_layout);

  PangoLayout* layout = pango_layout_copy(font_layout);
  pango_layout_set_font_description(layout, font_desc);
  pango_layout_set_height(layout, height);
  pango_layout_set_text(layout, text, length);
  int width = pango_layout_get_width(layout);
  g_object_unref(layout);
  return width / PANGO_SCALE;
}

NtkPangoLayoutFont* ntk_pango_layout_font_new(PangoLayout* layout, PangoFontDescription* font_desc) {
  NtkPangoLayoutFont* font = g_try_malloc0(sizeof(NtkPangoLayoutFont));
  if (font == NULL) return NULL;

  font->parent_instance.parent_instance.userdata.ptr = g_object_ref(layout);
  font->parent_instance.parent_instance.height = pango_font_description_get_size(font_desc) / PANGO_SCALE;
  font->parent_instance.parent_instance.width = ntk_pango_layout_font_get_width;
  font->parent_instance.impl_copy = ntk_pango_layout_font_impl_copy;
  font->parent_instance.impl_free = ntk_pango_layout_font_impl_free;

  g_debug(
    "Font %p created: layout: %p, height: %f", font, font->parent_instance.parent_instance.userdata.ptr,
    font->parent_instance.parent_instance.height
  );

  if (font_desc != NULL)
    pango_layout_set_font_description(PANGO_LAYOUT(font->parent_instance.parent_instance.userdata.ptr), font_desc);
  return font;
}
