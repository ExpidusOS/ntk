#pragma once

#include <glib-object.h>
#include <ntk/font/pango/object.h>
#include <pango/pango.h>

G_BEGIN_DECLS

#define NTK_PANGO_TYPE_FONT_LAYOUT ntk_pango_font_layout_get_type()
G_DECLARE_FINAL_TYPE(NtkPangoFontLayout, ntk_pango_font_layout, NTK_PANGO, FONT_LAYOUT, NtkPangoFont);

/**
 * NtkPangoFontLayout:
 *
 * Since: 0.1.0
 */
struct _NtkPangoFontLayout {
  NtkPangoFont parent_instance;

  /*< private >*/
  struct _NtkPangoFontLayoutPrivate* priv;
};

NtkFont* ntk_pango_font_layout_new(PangoFontDescription* desc);
NtkFont* ntk_pango_font_layout_new_with_layout(PangoLayout* layout, PangoFontDescription* desc);

PangoLayout* ntk_pango_font_layout_get_layout(NtkPangoFontLayout* self);

G_END_DECLS
