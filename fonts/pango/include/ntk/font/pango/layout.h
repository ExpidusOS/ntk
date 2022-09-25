#pragma once

#include <ntk/font/user.h>
#include <nuklear.h>
#include <pango/pango.h>

#define NTK_PANGO_TYPE_LAYOUT_FONT ntk_pango_layout_font_get_type()

/**
 * NtkPangoLayoutFont:
 *
 * Since: 0.1.0
 */
typedef struct _NtkPangoLayoutFont {
  NtkUserFont parent_instance;
} NtkPangoLayoutFont;

GType ntk_pango_layout_font_get_type();

/**
 * ntk_pango_layout_font_new: (constructor) (ref-func ntk_pango_layout_font_ref) (unref-func ntk_pango_layout_font_unref)
 * @layout: (transfer none): The #PangoLayout instance.
 * @font_desc: (transfer none): The font description.
 * Returns: (transfer full): A new Nuklear User Font instance.
 */
NtkPangoLayoutFont* ntk_pango_layout_font_new(PangoLayout* layout, PangoFontDescription* font_desc);
