#include <ntk/color.h>
#include <ntk/styler.h>

gboolean ntk_styler_create_window_header_style_for_key(NtkStyler* self, NtkStylerKey key, struct nk_style_window_header* style) {
  g_return_val_if_fail(NTK_IS_STYLER(self), FALSE);
  g_return_val_if_fail(style != NULL, FALSE);

  key.state = (NtkStylerState[]){NTK_STYLER_STATE_NORMAL, NTK_STYLER_STATE_NONE};
  g_return_val_if_fail(ntk_styler_create_item_style_for_key(self, key, &style->normal), FALSE);

  key.state = (NtkStylerState[]){NTK_STYLER_STATE_HOVER, NTK_STYLER_STATE_NONE};
  g_return_val_if_fail(ntk_styler_create_item_style_for_key(self, key, &style->hover), FALSE);

  key.state = (NtkStylerState[]){NTK_STYLER_STATE_ACTIVE, NTK_STYLER_STATE_NONE};
  g_return_val_if_fail(ntk_styler_create_item_style_for_key(self, key, &style->active), FALSE);

  NtkStylerKey button_key = key;
  button_key.elem = ntk_styler_element_extend(button_key.elem, (NtkStylerElement[]){NTK_STYLER_ELEMENT_BUTTON, NTK_STYLER_ELEMENT_NONE});
  g_return_val_if_fail(button_key.elem != NULL, FALSE);

  button_key.classes = (char*[]){ "close", NULL };
  if (!ntk_styler_create_button_style_for_key(self, button_key, &style->close_button)) {
    g_free(button_key.elem);
    g_return_val_if_reached(FALSE);
  }

  button_key.classes = (char*[]){ "minimize", NULL };
  if (!ntk_styler_create_button_style_for_key(self, button_key, &style->minimize_button)) {
    g_free(button_key.elem);
    g_return_val_if_reached(FALSE);
  }

  g_free(button_key.elem);

  key.state = (NtkStylerState[]){NTK_STYLER_STATE_NORMAL, NTK_STYLER_STATE_NONE};

  NtkStylerKey label_key = key;
  label_key.prop = NTK_STYLER_PROPERTY_COLOR;
  label_key.elem = ntk_styler_element_extend(button_key.elem, (NtkStylerElement[]){NTK_STYLER_ELEMENT_TEXT, NTK_STYLER_ELEMENT_NONE});
  g_return_val_if_fail(label_key.elem != NULL, FALSE);

  if (ntk_styler_has_style_property(self, label_key)) {
    GValue value = G_VALUE_INIT;
    if (!ntk_styler_get_style_property(self, label_key, &value)) {
      g_free(label_key.elem);
      g_return_val_if_reached(FALSE);
    }
    g_assert_cmpint(G_VALUE_TYPE(&value), ==, NTK_TYPE_COLOR);

    NtkColor* color = g_value_get_boxed(&value);
    g_return_val_if_fail(color != NULL, FALSE);
    ntk_color_nuke(color, &style->label_normal);
  }

  label_key.state = (NtkStylerState[]){NTK_STYLER_STATE_HOVER, NTK_STYLER_STATE_NONE};
  if (ntk_styler_has_style_property(self, label_key)) {
    GValue value = G_VALUE_INIT;
    if (!ntk_styler_get_style_property(self, label_key, &value)) {
      g_free(label_key.elem);
      g_return_val_if_reached(FALSE);
    }
    g_assert_cmpint(G_VALUE_TYPE(&value), ==, NTK_TYPE_COLOR);

    NtkColor* color = g_value_get_boxed(&value);
    g_return_val_if_fail(color != NULL, FALSE);
    ntk_color_nuke(color, &style->label_hover);
  }

  label_key.state = (NtkStylerState[]){NTK_STYLER_STATE_ACTIVE, NTK_STYLER_STATE_NONE};
  if (ntk_styler_has_style_property(self, label_key)) {
    GValue value = G_VALUE_INIT;
    if (!ntk_styler_get_style_property(self, label_key, &value)) {
      g_free(label_key.elem);
      g_return_val_if_reached(FALSE);
    }
    g_assert_cmpint(G_VALUE_TYPE(&value), ==, NTK_TYPE_COLOR);

    NtkColor* color = g_value_get_boxed(&value);
    g_return_val_if_fail(color != NULL, FALSE);
    ntk_color_nuke(color, &style->label_active);
  }

  label_key.state = (NtkStylerState[]){NTK_STYLER_STATE_NORMAL, NTK_STYLER_STATE_NONE};
  label_key.prop = NTK_STYLER_PROPERTY_PADDING_TOP;
  if (ntk_styler_has_style_property(self, label_key)) {
    GValue value = G_VALUE_INIT;
    if (!ntk_styler_get_style_property(self, label_key, &value)) {
      g_free(label_key.elem);
      g_return_val_if_reached(FALSE);
    }
    g_assert_cmpint(G_VALUE_TYPE(&value), ==, G_TYPE_FLOAT);

    style->label_padding.y = g_value_get_float(&value);
  }

  label_key.prop = NTK_STYLER_PROPERTY_PADDING_LEFT;
  if (ntk_styler_has_style_property(self, label_key)) {
    GValue value = G_VALUE_INIT;
    if (!ntk_styler_get_style_property(self, label_key, &value)) {
      g_free(label_key.elem);
      g_return_val_if_reached(FALSE);
    }
    g_assert_cmpint(G_VALUE_TYPE(&value), ==, G_TYPE_FLOAT);

    style->label_padding.x = g_value_get_float(&value);
  }

  g_free(label_key.elem);

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

  key.prop = NTK_STYLER_PROPERTY_SPACING_TOP;
  if (ntk_styler_has_style_property(self, key)) {
    GValue value = G_VALUE_INIT;
    g_return_val_if_fail(ntk_styler_get_style_property(self, key, &value), FALSE); 
    g_assert_cmpint(G_VALUE_TYPE(&value), ==, G_TYPE_FLOAT);

    style->spacing.y = g_value_get_float(&value);
  }

  key.prop = NTK_STYLER_PROPERTY_SPACING_LEFT;
  if (ntk_styler_has_style_property(self, key)) {
    GValue value = G_VALUE_INIT;
    g_return_val_if_fail(ntk_styler_get_style_property(self, key, &value), FALSE); 
    g_assert_cmpint(G_VALUE_TYPE(&value), ==, G_TYPE_FLOAT);

    style->spacing.x = g_value_get_float(&value);
  }
  return TRUE;
}
