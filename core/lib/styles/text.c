#include <ntk/color.h>
#include <ntk/styler.h>

gboolean ntk_styler_create_text_style(NtkStyler* self, NtkStylerElement* elems, struct nk_style_text* style) {
  g_return_val_if_fail(NTK_IS_STYLER(self), FALSE);
  g_return_val_if_fail(style != NULL, FALSE);

  NtkStylerKey key = {};
  key.elem = elems;
  key.state = (NtkStylerState[]){NTK_STYLER_STATE_NORMAL, NTK_STYLER_STATE_NONE};
  key.prop = NTK_STYLER_PROPERTY_COLOR;

  if (ntk_styler_has_style_property(self, key)) {
    GValue value = G_VALUE_INIT;
    g_return_val_if_fail(ntk_styler_get_style_property(self, key, &value), FALSE);
    g_assert_cmpint(G_VALUE_TYPE(&value), ==, NTK_TYPE_COLOR);

    NtkColor* color = g_value_get_boxed(&value);
    g_return_val_if_fail(color != NULL, FALSE);
    ntk_color_nuke(color, &style->color);
  }

  key.prop = NTK_STYLER_PROPERTY_PADDING_TOP;
  if (ntk_styler_has_style_property(self, key)) {
    GValue value = G_VALUE_INIT;
    g_return_val_if_fail(ntk_styler_get_style_property(self, key, &value), FALSE);
    g_assert_cmpint(G_VALUE_TYPE(&value), ==, G_TYPE_FLOAT);

    style->padding.y = g_value_get_float(&value);
  }

  key.prop = NTK_STYLER_PROPERTY_PADDING_LEFT;
  if (ntk_styler_has_style_property(self, key)) {
    GValue value = G_VALUE_INIT;
    g_return_val_if_fail(ntk_styler_get_style_property(self, key, &value), FALSE);
    g_assert_cmpint(G_VALUE_TYPE(&value), ==, G_TYPE_FLOAT);

    style->padding.x = g_value_get_float(&value);
  }
  return TRUE;
}
