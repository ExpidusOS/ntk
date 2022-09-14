#ifndef __NTK_FONT_PANGO_OBJECT_H_
#define __NTK_FONT_PANGO_OBJECT_H_ 1

#include <glib-object.h>
#include <pango/pango.h>

G_BEGIN_DECLS

#define NTK_TYPE_PANGO_FONT ntk_pango_font_get_type()
G_DECLARE_DERIVABLE_TYPE(NtkPangoFont, ntk_pango_font, NTK, PANGO_FONT, GObject);

/**
 * NtkPangoFont:
 *
 * Since: 0.1.0
 */
struct _NtkPangoFontClass {
  GObjectClass parent_class;
};

/**
 * ntk_pango_font_set_description: (set-property description)
 */
void ntk_pango_font_set_description(NtkPangoFont* self, const PangoFontDescription* desc);

/**
 * ntk_pango_font_get_description: (get-property description)
 */
PangoFontDescription* ntk_pango_font_get_description(NtkPangoFont* self);

G_END_DECLS

#endif
