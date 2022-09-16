#pragma once

#include <glib-object.h>
#include <ntk/font/object.h>
#include <pango/pango.h>

G_BEGIN_DECLS

#define NTK_PANGO_TYPE_FONT ntk_pango_font_get_type()
G_DECLARE_DERIVABLE_TYPE(NtkPangoFont, ntk_pango_font, NTK_PANGO, FONT, NtkFont);

/**
 * NtkPangoFont:
 *
 * Since: 0.1.0
 */
struct _NtkPangoFontClass {
  NtkFontClass parent_class;
};

NtkFont* ntk_pango_font_new(PangoFontDescription* desc);
NtkFont* ntk_pango_font_new_with_context(PangoContext* ctx, PangoFontDescription* desc);

/**
 * ntk_pango_font_set_description: (set-property description)
 */
void ntk_pango_font_set_description(NtkPangoFont* self, const PangoFontDescription* desc);

/**
 * ntk_pango_font_get_description: (get-property description)
 */
PangoFontDescription* ntk_pango_font_get_description(NtkPangoFont* self);

G_END_DECLS
