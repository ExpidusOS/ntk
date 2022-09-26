#include <ntk/color.h>
#include <ntk/styler.h>

gboolean ntk_styler_create_button_style(NtkStyler* self, NtkStylerElement* elems, struct nk_style_button* style) {
  NtkStylerKey key = {};
  key.elem = elems;
  key.state = (NtkStylerState[]){NTK_STYLER_STATE_NORMAL, NTK_STYLER_STATE_NONE};
  return ntk_styler_create_button_style_for_key(self, key, style);
}

gboolean ntk_styler_create_button_style_for_key(NtkStyler* self, NtkStylerKey key, struct nk_style_button* style) {
  g_return_val_if_fail(NTK_IS_STYLER(self), FALSE);
  g_return_val_if_fail(style != NULL, FALSE);

  key.state = (NtkStylerState[]){NTK_STYLER_STATE_NORMAL, NTK_STYLER_STATE_NONE};
  g_return_val_if_fail(
    ntk_styler_create_item_style_for_key(
      self, key, &style->normal
    ),
    FALSE
  );

  key.state = (NtkStylerState[]){NTK_STYLER_STATE_HOVER, NTK_STYLER_STATE_NONE};
  g_return_val_if_fail(
    ntk_styler_create_item_style_for_key(
      self, key, &style->hover
    ),
    FALSE
  );

  key.state = (NtkStylerState[]){NTK_STYLER_STATE_ACTIVE, NTK_STYLER_STATE_NONE};
  g_return_val_if_fail(
    ntk_styler_create_item_style_for_key(
      self, key, &style->active
    ),
    FALSE
  );

  key.state = (NtkStylerState[]){NTK_STYLER_STATE_NORMAL, NTK_STYLER_STATE_NONE};
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

  key.state = (NtkStylerState[]){NTK_STYLER_STATE_HOVER, NTK_STYLER_STATE_NONE};
  key.prop = NTK_STYLER_PROPERTY_COLOR;
  if (ntk_styler_has_style_property(self, key)) {
    GValue value = G_VALUE_INIT;
    g_return_val_if_fail(ntk_styler_get_style_property(self, key, &value), FALSE);
    g_assert_cmpint(G_VALUE_TYPE(&value), ==, NTK_TYPE_COLOR);

    NtkColor* color = g_value_get_boxed(&value);
    g_return_val_if_fail(color != NULL, FALSE);
    ntk_color_nuke(color, &style->text_hover);
  }

  key.state = (NtkStylerState[]){NTK_STYLER_STATE_ACTIVE, NTK_STYLER_STATE_NONE};
  key.prop = NTK_STYLER_PROPERTY_COLOR;
  if (ntk_styler_has_style_property(self, key)) {
    GValue value = G_VALUE_INIT;
    g_return_val_if_fail(ntk_styler_get_style_property(self, key, &value), FALSE);
    g_assert_cmpint(G_VALUE_TYPE(&value), ==, NTK_TYPE_COLOR);

    NtkColor* color = g_value_get_boxed(&value);
    g_return_val_if_fail(color != NULL, FALSE);
    ntk_color_nuke(color, &style->text_active);
  }

  key.state = (NtkStylerState[]){NTK_STYLER_STATE_NORMAL, NTK_STYLER_STATE_NONE};
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

  key.prop = NTK_STYLER_PROPERTY_TEXT_ALIGN;
  if (ntk_styler_has_style_property(self, key)) {
    GValue value = G_VALUE_INIT;
    g_return_val_if_fail(ntk_styler_get_style_property(self, key, &value), FALSE);
    g_assert_cmpint(G_VALUE_TYPE(&value), ==, G_TYPE_STRING);

    const gchar* str = g_value_get_string(&value);
    g_assert(str != NULL);

    if (g_str_equal(str, "center"))
      style->text_alignment = NK_TEXT_CENTERED;
    else if (g_str_equal(str, "left"))
      style->text_alignment = NK_TEXT_LEFT;
    else if (g_str_equal(str, "right"))
      style->text_alignment = NK_TEXT_RIGHT;
  }
  return TRUE;
}
