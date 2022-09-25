#include <ntk/styler-styles.h>
#include <ntk/color.h>

gboolean ntk_styler_create_item_style(NtkStyler* self, NtkStylerElement* elems, NtkStylerState* state, struct nk_style_item* style) {
  g_return_val_if_fail(NTK_IS_STYLER(self), FALSE);
  g_return_val_if_fail(style != NULL, FALSE);

  NtkStylerKey key = {};
  key.elem = elems;
  key.state = state;
  key.prop = NTK_STYLER_PROPERTY_BACKGROUND_IMAGE;

  if (ntk_styler_has_style_property(self, key)) {
    g_warning("Not implemented yet, refer to comment in "__FILE__":%d in the NTK source code", __LINE__ + 1);
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

gboolean ntk_styler_create_text_style(NtkStyler* self, NtkStylerElement* elems, struct nk_style_text* style) {
  g_return_val_if_fail(NTK_IS_STYLER(self), FALSE);
  g_return_val_if_fail(style != NULL, FALSE);

  NtkStylerKey key = {};
  key.elem = elems;
  key.state = (NtkStylerState[]){ NTK_STYLER_STATE_NORMAL, NTK_STYLER_STATE_NONE };
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

gboolean ntk_styler_create_button_style(NtkStyler* self, NtkStylerElement* elems, struct nk_style_button* style) {
  g_return_val_if_fail(NTK_IS_STYLER(self), FALSE);
  g_return_val_if_fail(style != NULL, FALSE);

  g_return_val_if_fail(ntk_styler_create_item_style(self, elems, (NtkStylerState[]){ NTK_STYLER_STATE_NORMAL, NTK_STYLER_STATE_NONE }, &style->normal), FALSE);
  g_return_val_if_fail(ntk_styler_create_item_style(self, elems, (NtkStylerState[]){ NTK_STYLER_STATE_HOVER, NTK_STYLER_STATE_NONE }, &style->hover), FALSE);
  g_return_val_if_fail(ntk_styler_create_item_style(self, elems, (NtkStylerState[]){ NTK_STYLER_STATE_ACTIVE, NTK_STYLER_STATE_NONE }, &style->active), FALSE);

  NtkStylerKey key = {};
  key.elem = elems;
  key.state = (NtkStylerState[]){ NTK_STYLER_STATE_NORMAL, NTK_STYLER_STATE_NONE };
  key.prop = NTK_STYLER_PROPERTY_BORDER_COLOR;

  if (ntk_styler_has_style_property(self, key)) {
    GValue value = G_VALUE_INIT;
    g_return_val_if_fail(ntk_styler_get_style_property(self, key, &value), FALSE);
    g_assert_cmpint(G_VALUE_TYPE(&value), ==, NTK_TYPE_COLOR);

    NtkColor* color = g_value_get_boxed(&value);
    g_return_val_if_fail(color != NULL, FALSE);
    ntk_color_nuke(color, &style->border_color);
  }

  key.prop = NTK_STYLER_PROPERTY_BACKGROUND_COLOR;
  if (ntk_styler_has_style_property(self, key)) {
    GValue value = G_VALUE_INIT;
    g_return_val_if_fail(ntk_styler_get_style_property(self, key, &value), FALSE);
    g_assert_cmpint(G_VALUE_TYPE(&value), ==, NTK_TYPE_COLOR);

    NtkColor* color = g_value_get_boxed(&value);
    g_return_val_if_fail(color != NULL, FALSE);
    ntk_color_nuke(color, &style->text_background);
  }

  key.prop = NTK_STYLER_PROPERTY_COLOR;
  if (ntk_styler_has_style_property(self, key)) {
    GValue value = G_VALUE_INIT;
    g_return_val_if_fail(ntk_styler_get_style_property(self, key, &value), FALSE);
    g_assert_cmpint(G_VALUE_TYPE(&value), ==, NTK_TYPE_COLOR);

    NtkColor* color = g_value_get_boxed(&value);
    g_return_val_if_fail(color != NULL, FALSE);
    ntk_color_nuke(color, &style->text_normal);
    ntk_color_nuke(color, &style->text_hover);
    ntk_color_nuke(color, &style->text_active);
  }

  key.state = (NtkStylerState[]){ NTK_STYLER_STATE_HOVER, NTK_STYLER_STATE_NONE };
  key.prop = NTK_STYLER_PROPERTY_COLOR;
  if (ntk_styler_has_style_property(self, key)) {
    GValue value = G_VALUE_INIT;
    g_return_val_if_fail(ntk_styler_get_style_property(self, key, &value), FALSE);
    g_assert_cmpint(G_VALUE_TYPE(&value), ==, NTK_TYPE_COLOR);

    NtkColor* color = g_value_get_boxed(&value);
    g_return_val_if_fail(color != NULL, FALSE);
    ntk_color_nuke(color, &style->text_hover);
  }

  key.state = (NtkStylerState[]){ NTK_STYLER_STATE_ACTIVE, NTK_STYLER_STATE_NONE };
  key.prop = NTK_STYLER_PROPERTY_COLOR;
  if (ntk_styler_has_style_property(self, key)) {
    GValue value = G_VALUE_INIT;
    g_return_val_if_fail(ntk_styler_get_style_property(self, key, &value), FALSE);
    g_assert_cmpint(G_VALUE_TYPE(&value), ==, NTK_TYPE_COLOR);

    NtkColor* color = g_value_get_boxed(&value);
    g_return_val_if_fail(color != NULL, FALSE);
    ntk_color_nuke(color, &style->text_active);
  }

  key.state = (NtkStylerState[]){ NTK_STYLER_STATE_NORMAL, NTK_STYLER_STATE_NONE };
  key.prop = NTK_STYLER_PROPERTY_BORDER_WIDTH;
  if (ntk_styler_has_style_property(self, key)) {
    GValue value = G_VALUE_INIT;
    g_return_val_if_fail(ntk_styler_get_style_property(self, key, &value), FALSE);
    g_assert_cmpint(G_VALUE_TYPE(&value), ==, G_TYPE_FLOAT);

    style->border = g_value_get_float(&value);
  }

  key.prop = NTK_STYLER_PROPERTY_BORDER_RADIUS;
  if (ntk_styler_has_style_property(self, key)) {
    GValue value = G_VALUE_INIT;
    g_return_val_if_fail(ntk_styler_get_style_property(self, key, &value), FALSE);
    g_assert_cmpint(G_VALUE_TYPE(&value), ==, G_TYPE_FLOAT);

    style->rounding = g_value_get_float(&value);
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
