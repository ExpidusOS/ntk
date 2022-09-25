#include <ntk/color.h>
#include <ntk/styler.h>

gboolean
ntk_styler_create_item_style(NtkStyler* self, NtkStylerElement* elems, NtkStylerState* state, struct nk_style_item* style) {
  g_return_val_if_fail(NTK_IS_STYLER(self), FALSE);
  g_return_val_if_fail(style != NULL, FALSE);

  NtkStylerKey key = {};
  key.elem = elems;
  key.state = state;
  key.prop = NTK_STYLER_PROPERTY_BACKGROUND_IMAGE;

  if (ntk_styler_has_style_property(self, key)) {
    g_warning(
      "Not implemented yet, refer to comment in "__FILE__
      ":%d in the NTK source code",
      __LINE__ + 2
    );
    // TODO: possibly implement the image loading into NtkRenderer
    return FALSE;
  }

  key.prop = NTK_STYLER_PROPERTY_BACKGROUND_COLOR;
  if (ntk_styler_has_style_property(self, key)) {
    GValue value = G_VALUE_INIT;
    g_return_val_if_fail(ntk_styler_get_style_property(self, key, &value), FALSE);
    g_assert_cmpint(G_VALUE_TYPE(&value), ==, NTK_TYPE_COLOR);

    NtkColor* color = g_value_get_boxed(&value);
    g_return_val_if_fail(color != NULL, FALSE);

    struct nk_color ncolor;
    ntk_color_nuke(color, &ncolor);
    *style = nk_style_item_color(ncolor);
    return TRUE;
  }
  return TRUE;
}
