#include <ntk/color.h>
#include <ntk/styler.h>

gboolean ntk_styler_create_chart_style(NtkStyler* self, NtkStylerElement* elems, struct nk_style_chart* style) {
  g_return_val_if_fail(NTK_IS_STYLER(self), FALSE);
  g_return_val_if_fail(style != NULL, FALSE);

  NtkStylerElement* full_elems =
    ntk_styler_element_extend(elems, (NtkStylerElement[]){NTK_STYLER_ELEMENT_CHART, NTK_STYLER_ELEMENT_NONE});
  g_return_val_if_fail(full_elems != NULL, FALSE);

  if (!ntk_styler_create_item_style(self, full_elems, (NtkStylerState[]){NTK_STYLER_STATE_NORMAL, NTK_STYLER_STATE_NONE}, &style->background)) {
    g_free(full_elems);
    g_return_val_if_reached(FALSE);
  }

  NtkStylerKey key = {};
  key.elem = full_elems;
  key.state = (NtkStylerState[]){NTK_STYLER_STATE_NORMAL, NTK_STYLER_STATE_NONE};
  key.prop = NTK_STYLER_PROPERTY_BORDER_COLOR;

  if (ntk_styler_has_style_property(self, key)) {
    GValue value = G_VALUE_INIT;
    if (!ntk_styler_get_style_property(self, key, &value)) {
      g_free(full_elems);
      g_return_val_if_reached(FALSE);
    }
    g_assert_cmpint(G_VALUE_TYPE(&value), ==, NTK_TYPE_COLOR);

    NtkColor* color = g_value_get_boxed(&value);
    g_return_val_if_fail(color != NULL, FALSE);
    ntk_color_nuke(color, &style->border_color);
  }

  key.prop = NTK_STYLER_PROPERTY_COLOR;
  key.state = (NtkStylerState[]){NTK_STYLER_STATE_SELECTION, NTK_STYLER_STATE_NONE};
  if (ntk_styler_has_style_property(self, key)) {
    GValue value = G_VALUE_INIT;
    if (!ntk_styler_get_style_property(self, key, &value)) {
      g_free(full_elems);
      g_return_val_if_reached(FALSE);
    }
    g_assert_cmpint(G_VALUE_TYPE(&value), ==, NTK_TYPE_COLOR);

    NtkColor* color = g_value_get_boxed(&value);
    g_return_val_if_fail(color != NULL, FALSE);
    ntk_color_nuke(color, &style->selected_color);
  }

  key.prop = NTK_STYLER_PROPERTY_COLOR;
  key.state = (NtkStylerState[]){NTK_STYLER_STATE_NORMAL, NTK_STYLER_STATE_NONE};
  if (ntk_styler_has_style_property(self, key)) {
    GValue value = G_VALUE_INIT;
    if (!ntk_styler_get_style_property(self, key, &value)) {
      g_free(full_elems);
      g_return_val_if_reached(FALSE);
    }
    g_assert_cmpint(G_VALUE_TYPE(&value), ==, NTK_TYPE_COLOR);

    NtkColor* color = g_value_get_boxed(&value);
    g_return_val_if_fail(color != NULL, FALSE);
    ntk_color_nuke(color, &style->color);
  }

  key.prop = NTK_STYLER_PROPERTY_BORDER_WIDTH;
  if (ntk_styler_has_style_property(self, key)) {
    GValue value = G_VALUE_INIT;
    if (!ntk_styler_get_style_property(self, key, &value)) {
      g_free(full_elems);
      g_return_val_if_reached(FALSE);
    }
    g_assert_cmpint(G_VALUE_TYPE(&value), ==, G_TYPE_FLOAT);

    style->border = g_value_get_float(&value);
  }

  key.prop = NTK_STYLER_PROPERTY_BORDER_RADIUS;
  if (ntk_styler_has_style_property(self, key)) {
    GValue value = G_VALUE_INIT;
    if (!ntk_styler_get_style_property(self, key, &value)) {
      g_free(full_elems);
      g_return_val_if_reached(FALSE);
    }
    g_assert_cmpint(G_VALUE_TYPE(&value), ==, G_TYPE_FLOAT);

    style->rounding = g_value_get_float(&value);
  }

  key.prop = NTK_STYLER_PROPERTY_PADDING_TOP;
  if (ntk_styler_has_style_property(self, key)) {
    GValue value = G_VALUE_INIT;
    if (!ntk_styler_get_style_property(self, key, &value)) {
      g_free(full_elems);
      g_return_val_if_reached(FALSE);
    }
    g_assert_cmpint(G_VALUE_TYPE(&value), ==, G_TYPE_FLOAT);

    style->padding.y = g_value_get_float(&value);
  }

  key.prop = NTK_STYLER_PROPERTY_PADDING_LEFT;
  if (ntk_styler_has_style_property(self, key)) {
    GValue value = G_VALUE_INIT;
    if (!ntk_styler_get_style_property(self, key, &value)) {
      g_free(full_elems);
      g_return_val_if_reached(FALSE);
    }
    g_assert_cmpint(G_VALUE_TYPE(&value), ==, G_TYPE_FLOAT);

    style->padding.x = g_value_get_float(&value);
  }

  g_free(full_elems);
  return TRUE;
}
