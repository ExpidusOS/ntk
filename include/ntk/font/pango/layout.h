#ifndef __NTK_FONT_PANGO_LAYOUT_H_
#define __NTK_FONT_PANGO_LAYOUT_H_ 1

#include <pango/pango.h>
#include <nuklear.h>

#define NTK_TYPE_PANGO_LAYOUT_FONT ntk_pango_layout_font_get_type()

/**
 * NtkPangoLayoutFont:
 *
 * Since: 0.1.0
 */
typedef struct nk_user_font NtkPangoLayoutFont;

GType ntk_pango_layout_font_get_type();

/**
 * ntk_pango_layout_font_new: (constructor) (ref-func ntk_pango_layout_font_ref) (unref-func ntk_pango_layout_font_unref)
 * @layout: (transfer none): The #PangoLayout instance.
 * @font_desc: (transfer none): The font description.
 * Returns: (transfer full): A new Nuklear User Font instance.
 */
NtkPangoLayoutFont* ntk_pango_layout_font_new(PangoLayout* layout, PangoFontDescription* font_desc);

/**
 * ntk_pango_layout_font_ref:
 * @font: (transfer none): The base font
 * Returns: (transfer full): A new instance.
 */
NtkPangoLayoutFont* ntk_pango_layout_font_copy(NtkPangoLayoutFont* font);

/**
 * ntk_pango_layout_font_free:
 * @font: (transfer none): The font
 */
void ntk_pango_layout_font_free(NtkPangoLayoutFont* font);

#endif
