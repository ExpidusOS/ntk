#include <ntk/color.h>
#include <ntk/styler.h>

gboolean ntk_styler_create_srollbar_style(NtkStyler* self, NtkStylerElement* elems, struct nk_style_scrollbar* style) {
  g_return_val_if_fail(NTK_IS_STYLER(self), FALSE);
  g_return_val_if_fail(style != NULL, FALSE);

  g_return_val_if_fail(ntk_styler_create_item_style(
        self, elems, (NtkStylerState[]){NTK_STYLER_STATE_NORMAL, NTK_STYLER_STATE_NONE}, &style->normal
      ), FALSE);

  g_return_val_if_fail(ntk_styler_create_item_style(
        self, elems, (NtkStylerState[]){NTK_STYLER_STATE_HOVER, NTK_STYLER_STATE_NONE}, &style->hover
      ), FALSE);

  g_return_val_if_fail(ntk_styler_create_item_style(
        self, elems, (NtkStylerState[]){NTK_STYLER_STATE_ACTIVE, NTK_STYLER_STATE_NONE}, &style->active
      ), FALSE);

  NtkStylerKey key = {};
  key.elem = elems;
  key.state = (NtkStylerState[]){NTK_STYLER_STATE_NORMAL, NTK_STYLER_STATE_NONE};
  key.prop = NTK_STYLER_PROPERTY_COLOR;

  if (ntk_styler_has_style_property(self, key)) {
    GValue value = G_VALUE_INIT;
    if (!ntk_styler_get_style_property(self, key, &value)) {
      g_free(elems);
      g_return_val_if_reached(FALSE);
    }
    g_assert_cmpint(G_VALUE_TYPE(&value), ==, NTK_TYPE_COLOR);

    NtkColor* color = g_value_get_boxed(&value);
    g_return_val_if_fail(color != NULL, FALSE);

    struct nk_color ncolor;
    ntk_color_nuke(color, &ncolor);
    style->cursor_normal = nk_style_item_color(ncolor);
  }

  key.state = (NtkStylerState[]){NTK_STYLER_STATE_HOVER, NTK_STYLER_STATE_NONE};
  if (ntk_styler_has_style_property(self, key)) {
    GValue value = G_VALUE_INIT;
    if (!ntk_styler_get_style_property(self, key, &value)) {
      g_free(elems);
      g_return_val_if_reached(FALSE);
    }
    g_assert_cmpint(G_VALUE_TYPE(&value), ==, NTK_TYPE_COLOR);

    NtkColor* color = g_value_get_boxed(&value);
    g_return_val_if_fail(color != NULL, FALSE);

    struct nk_color ncolor;
    ntk_color_nuke(color, &ncolor);
    style->cursor_hover = nk_style_item_color(ncolor);
  }

  key.state = (NtkStylerState[]){NTK_STYLER_STATE_ACTIVE, NTK_STYLER_STATE_NONE};
  if (ntk_styler_has_style_property(self, key)) {
    GValue value = G_VALUE_INIT;
    if (!ntk_styler_get_style_property(self, key, &value)) {
      g_free(elems);
      g_return_val_if_reached(FALSE);
    }
    g_assert_cmpint(G_VALUE_TYPE(&value), ==, NTK_TYPE_COLOR);

    NtkColor* color = g_value_get_boxed(&value);
    g_return_val_if_fail(color != NULL, FALSE);

    struct nk_color ncolor;
    ntk_color_nuke(color, &ncolor);
    style->cursor_active = nk_style_item_color(ncolor);
  }

  key.state = (NtkStylerState[]){NTK_STYLER_STATE_NORMAL, NTK_STYLER_STATE_NONE};
  key.prop = NTK_STYLER_PROPERTY_BORDER_COLOR;
  if (ntk_styler_has_style_property(self, key)) {
    GValue value = G_VALUE_INIT;
    if (!ntk_styler_get_style_property(self, key, &value)) {
      g_free(elems);
      g_return_val_if_reached(FALSE);
    }
    g_assert_cmpint(G_VALUE_TYPE(&value), ==, NTK_TYPE_COLOR);

    NtkColor* color = g_value_get_boxed(&value);
    g_return_val_if_fail(color != NULL, FALSE);
    ntk_color_nuke(color, &style->cursor_border_color);
  }

  key.prop = NTK_STYLER_PROPERTY_BORDER_RADIUS;
  if (ntk_styler_has_style_property(self, key)) {
    GValue value = G_VALUE_INIT;
    g_assert_cmpint(G_VALUE_TYPE(&value), ==, G_TYPE_FLOAT);

    style->rounding = g_value_get_float(&value);
  }

  key.prop = NTK_STYLER_PROPERTY_PADDING_TOP;
  if (ntk_styler_has_style_property(self, key)) {
    GValue value = G_VALUE_INIT;
    g_assert_cmpint(G_VALUE_TYPE(&value), ==, G_TYPE_FLOAT);

    style->padding.y = g_value_get_float(&value);
  }

  key.prop = NTK_STYLER_PROPERTY_PADDING_LEFT;
  if (ntk_styler_has_style_property(self, key)) {
    GValue value = G_VALUE_INIT;
    g_assert_cmpint(G_VALUE_TYPE(&value), ==, G_TYPE_FLOAT);

    style->padding.x = g_value_get_float(&value);
  }

  key.prop = NTK_STYLER_PROPERTY_BORDER_WIDTH;
  if (ntk_styler_has_style_property(self, key)) {
    GValue value = G_VALUE_INIT;
    g_return_val_if_fail(ntk_styler_get_style_property(self, key, &value), FALSE);
    g_assert_cmpint(G_VALUE_TYPE(&value), ==, G_TYPE_FLOAT);

    style->border = g_value_get_float(&value);
  }

  key.elem = ntk_styler_element_extend(elems, (NtkStylerElement[]){ NTK_STYLER_ELEMENT_BUTTON, NTK_STYLER_ELEMENT_NONE });
  g_return_val_if_fail(key.elem != NULL, FALSE);

  key.prop = NTK_STYLER_PROPERTY_VISIBILITY;
  if (ntk_styler_has_style_property(self, key)) {
    GValue value = G_VALUE_INIT;
    if (!ntk_styler_get_style_property(self, key, &value)) {
      g_free(key.elem);
      g_return_val_if_reached(FALSE);
    }
    g_assert_cmpint(G_VALUE_TYPE(&value), ==, G_TYPE_BOOLEAN);

    style->show_buttons = g_value_get_boolean(&value);
  }

  key.classes = (char*[]){ "inc", NULL };
  if (!ntk_styler_create_button_style_for_key(self, key, &style->inc_button)) {
    g_free(key.elem);
    g_return_val_if_reached(FALSE);
  }

  key.classes = (char*[]){ "dec", NULL };
  if (!ntk_styler_create_button_style_for_key(self, key, &style->dec_button)) {
    g_free(key.elem);
    g_return_val_if_reached(FALSE);
  }

  g_free(key.elem);
  return TRUE;
}
