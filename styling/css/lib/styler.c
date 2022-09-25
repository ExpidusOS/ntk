#include "styler-priv.h"
#include <ntk/color.h>
#include <ntk/styling/css/styler.h>

#define NTK_CSS_STYLER_PRIVATE(self) ((self)->priv == NULL ? ntk_css_styler_get_instance_private(self) : (self)->priv)

G_DEFINE_TYPE_WITH_PRIVATE(NtkCSSStyler, ntk_css_styler, NTK_TYPE_STYLER);

static void ntk_css_styler_default_key_free(NtkStylerKey* key) {
  g_clear_pointer(&key->class_name, g_free);
  g_clear_pointer(&key->state, g_free);
  g_free(key->elem);
  g_free(key);
}

static void ntk_css_styler_default_value_free(GValue* value) {
  g_value_unset(value);
  g_free(value);
}

static NtkColor* ntk_css_styler_new_color(CssValue* value) {
  switch (value->unit) {
    case CSS_VALUE_PARSER_HEXCOLOR:
      if (strlen(value->string) == 6) {
        return ntk_color_new(NTK_COLOR_FORMAT_RGB_HEX, value->string);
      } else if (strlen(value->string) == 8) {
        return ntk_color_new(NTK_COLOR_FORMAT_RGBA_HEX, value->string);
      }
      break;
    case CSS_VALUE_IDENT:
      return ntk_color_new(NTK_COLOR_FORMAT_NAMED, value->string);
    default:
      g_warning("Unsupported unit %d for color (%s)", value->unit, value->string);
      return NULL;
  }
  return NULL;
}

static gboolean ntk_css_styler_entry_count(const gchar* parent_tag, CssSelector* th, size_t n_elems, size_t n_states, size_t* n_elems_out, size_t* n_states_out) {
  g_assert(parent_tag == NULL ? n_elems == 0 : TRUE);

  do {
    switch (th->match) {
      case CssSelMatchTag:
        g_debug("%p Tag #%ld: %s (%s)", th, n_elems, th->tag->local, parent_tag);
        if (g_str_equal(th->tag->local, "button")) {
          if (th->tagHistory == NULL) {
            n_elems += 3;
            break;
          }
        } else if (g_str_equal(th->tag->local, "input")) {
          if (th->tagHistory == NULL) {
            n_elems++;
            break;
          }
        } else if (g_str_equal(th->tag->local, "scrollbar")) {
          if (th->tagHistory == NULL) {
            n_elems += 2;
            break;
          }
        } else {
          n_elems++;
        }

        if (th->tagHistory != NULL) {
          size_t n_child_elems = 0;
          size_t n_child_states = 0;

          g_return_val_if_fail(ntk_css_styler_entry_count(th->tag->local, th->tagHistory, n_elems, n_states, &n_child_elems, &n_child_states), FALSE);
          
          size_t old_elems = n_elems;
          size_t old_states = n_states;

          n_elems += n_child_elems;
          n_states += n_child_states;

          g_assert_cmpint(n_elems, !=, old_elems);
          if (n_states > 0) g_assert_cmpint(n_states, !=, old_states);
        }
        break;
      case CssSelMatchAttrExact:
        if (parent_tag != NULL) {
          if (g_str_equal(parent_tag, "button")) {
            if (g_str_equal(th->data->attribute->local, "type")) {
              if (g_str_equal(th->data->value, "default")) {
                n_elems++;
                break;
              } else if (g_str_equal(th->data->value, "menu")) {
                n_elems++;
                break;
              } else if (g_str_equal(th->data->value, "context")) {
                n_elems++;
                break;
              }
            }
          } else if (g_str_equal(parent_tag, "input")) {
            if (g_str_equal(th->data->attribute->local, "type")) {
              if (g_str_equal(th->data->value, "button")) {
                n_elems++;
                break;
              } else if (g_str_equal(th->data->value, "toggle")) {
                n_elems++;
                break;
              } else if (g_str_equal(th->data->value, "text")) {
                n_elems++;
                break;
              } else if (g_str_equal(th->data->value, "range")) {
                n_elems++;
                break;
              } else if (g_str_equal(th->data->value, "checkbox")) {
                n_elems++;
                break;
              }
            }
          } else if (g_str_equal(parent_tag, "scrollbar")) {
            if (g_str_equal(th->data->attribute->local, "type")) {
              if (g_str_equal(th->data->value, "vertical")) {
                n_elems++;
                break;
              } else if (g_str_equal(th->data->value, "horizontal")) {
                n_elems++;
                break;
              }
            }
          }
        }
        break;
      default:
        break;
    }

    switch (th->pseudo) {
      case CssPseudoHover:
      case CssPseudoActive:
      case CssPseudoSelection:
      case CssPseudoDefault:
      case CssPseudoSingleButton:
        n_states++;
        break;
      case CssPseudoNotParsed:
        if (th->match == CssSelMatchTag && parent_tag == NULL) n_states++;
        break;
      default:
        break;
    }
  } while ((th = th->tagHistory) != NULL);

  *n_elems_out = n_elems;
  *n_states_out = n_states;
  return TRUE;
}

static gboolean ntk_css_styler_entry_select(NtkStylerKey* key, const gchar* parent_tag, CssSelector* th, size_t n_elems, size_t n_states, size_t* n_elems_out, size_t* n_states_out) {
  g_assert(parent_tag == NULL ? n_elems == 0 : TRUE);

  do {
    switch (th->match) {
      case CssSelMatchTag:
        g_debug("%p Tag #%ld: %s (%s)", th, n_elems, th->tag->local, parent_tag);
        if (g_str_equal(th->tag->local, "p")) {
          key->elem[n_elems++] = NTK_STYLER_ELEMENT_TEXT;
        } else if (g_str_equal(th->tag->local, "button")) {
          if (th->tagHistory == NULL) {
            key->elem[n_elems++] = NTK_STYLER_ELEMENT_BUTTON;
            key->elem[n_elems++] = NTK_STYLER_ELEMENT_BUTTON_CONTEXT;
            key->elem[n_elems++] = NTK_STYLER_ELEMENT_BUTTON_MENU;
            break;
          }
        } else if (g_str_equal(th->tag->local, "input")) {
          if (th->tagHistory == NULL) {
            key->elem[n_elems++] = NTK_STYLER_ELEMENT_INPUT_TEXT;
            break;
          }
        } else if (g_str_equal(th->tag->local, "scrollbar")) {
          if (th->tagHistory == NULL) {
            key->elem[n_elems++] = NTK_STYLER_ELEMENT_SCROLLBAR_VERTICAL;
            key->elem[n_elems++] = NTK_STYLER_ELEMENT_SCROLLBAR_HORIZONTAL;
            break;
          }
        } else if (g_str_equal(th->tag->local, "progress")) {
          key->elem[n_elems++] = NTK_STYLER_ELEMENT_PROGRESS;
        } else if (g_str_equal(th->tag->local, "texarea")) {
          key->elem[n_elems++] = NTK_STYLER_ELEMENT_TEXTAREA;
        } else if (g_str_equal(th->tag->local, "tab")) {
          key->elem[n_elems++] = NTK_STYLER_ELEMENT_TAB;
        } else if (g_str_equal(th->tag->local, "option")) {
          key->elem[n_elems++] = NTK_STYLER_ELEMENT_OPTION;
        } else if (g_str_equal(th->tag->local, "window")) {
          key->elem[n_elems++] = NTK_STYLER_ELEMENT_WINDOW;
        } else if (g_str_equal(th->tag->local, "window-header")) {
          key->elem[n_elems++] = NTK_STYLER_ELEMENT_WINDOW_HEADER;
        } else {
          g_warning("Unsupported tag %s", th->tag->local);
          break;
        }

        if (th->tagHistory != NULL) {
          size_t n_child_elems = 0;
          size_t n_child_states = 0;

          g_return_val_if_fail(ntk_css_styler_entry_select(key, th->tag->local, th->tagHistory, n_elems, n_states, &n_child_elems, &n_child_states), FALSE);
          
          size_t old_elems = n_elems;
          size_t old_states = n_states;

          n_elems += n_child_elems;
          n_states += n_child_states;

          g_assert_cmpint(n_elems, !=, old_elems);
          if (n_states > 0) g_assert_cmpint(n_states, !=, old_states);
        }
        break;
      case CssSelMatchAttrExact:
        if (parent_tag != NULL) {
          if (g_str_equal(parent_tag, "button")) {
            if (g_str_equal(th->data->attribute->local, "type")) {
              if (g_str_equal(th->data->value, "default")) {
                key->elem[n_elems++] = NTK_STYLER_ELEMENT_BUTTON;
              } else if (g_str_equal(th->data->value, "menu")) {
                key->elem[n_elems++] = NTK_STYLER_ELEMENT_BUTTON_MENU;
              } else if (g_str_equal(th->data->value, "context")) {
                key->elem[n_elems++] = NTK_STYLER_ELEMENT_BUTTON_CONTEXT;
              }
            }
          } else if (g_str_equal(parent_tag, "input")) {
            if (g_str_equal(th->data->attribute->local, "type")) {
              if (g_str_equal(th->data->value, "button")) {
                key->elem[n_elems++] = NTK_STYLER_ELEMENT_BUTTON;
              } else if (g_str_equal(th->data->value, "toggle")) {
                key->elem[n_elems++] = NTK_STYLER_ELEMENT_INPUT_TOGGLE;
              } else if (g_str_equal(th->data->value, "text")) {
                key->elem[n_elems++] = NTK_STYLER_ELEMENT_INPUT_TEXT;
              } else if (g_str_equal(th->data->value, "range")) {
                key->elem[n_elems++] = NTK_STYLER_ELEMENT_INPUT_SLIDER;
              } else if (g_str_equal(th->data->value, "checkbox")) {
                key->elem[n_elems++] = NTK_STYLER_ELEMENT_INPUT_CHECKBOX;
              }
            }
          } else if (g_str_equal(parent_tag, "scrollbar")) {
            if (g_str_equal(th->data->attribute->local, "type")) {
              if (g_str_equal(th->data->value, "vertical")) {
                key->elem[n_elems++] = NTK_STYLER_ELEMENT_SCROLLBAR_VERTICAL;
              } else if (g_str_equal(th->data->value, "horizontal")) {
                key->elem[n_elems++] = NTK_STYLER_ELEMENT_SCROLLBAR_HORIZONTAL;
              }
            }
          }
        }
        break;
      case CssSelMatchClass:
        if (key->class_name == NULL) key->class_name = g_strdup(th->data->value);
        break;
      default:
        break;
    }

    switch (th->pseudo) {
      case CssPseudoHover:
        key->state[n_states++] = NTK_STYLER_STATE_HOVER;
        break;
      case CssPseudoActive:
        key->state[n_states++] = NTK_STYLER_STATE_ACTIVE;
        break;
      case CssPseudoSelection:
        key->state[n_states++] = NTK_STYLER_STATE_SELECTION;
        break;
      case CssPseudoSingleButton:
        key->state[n_states++] = NTK_STYLER_STATE_PRESSED;
        break;
      case CssPseudoDefault:
        key->state[n_states++] = NTK_STYLER_STATE_NORMAL;
        break;
      case CssPseudoNotParsed:
        if (th->match == CssSelMatchTag && parent_tag == NULL) key->state[n_states++] = NTK_STYLER_STATE_NORMAL;
        break;
      default:
        break;
    }
  } while ((th = th->tagHistory) != NULL);

  *n_elems_out = n_elems;
  *n_states_out = n_states;
  return TRUE;
}

static gboolean ntk_css_styler_entry_create(
  CssStyleRule* rule, CssDeclaration* declaration, CssSelector* selector, NtkStylerKey** key_out, GValue** value_out
) {
  NtkStylerKey* key = g_try_malloc0(sizeof(NtkStylerKey));
  g_return_val_if_fail(key != NULL, FALSE);

  size_t n_elems = 0;
  size_t n_states = 0;
  g_debug("Creating entry for (rule: %p, declaration: %p, selector: %p)", rule, declaration, selector);

  if (!ntk_css_styler_entry_count(NULL, selector, 0, 0, &n_elems, &n_states)) {
    g_free(key);
    g_return_val_if_reached(FALSE);
  }

  g_debug(
    "Entry for (rule: %p, declaration: %p, selector: %p) will have %ld elements and %ld states", rule, declaration, selector,
    n_elems, n_states
  );

  key->elem = g_try_malloc0(sizeof(NtkStylerElement) * n_elems);
  if (key->elem == NULL && n_elems > 0) {
    g_free(key);
    g_return_val_if_reached(FALSE);
  }

  key->state = g_try_malloc0(sizeof(NtkStylerState) * n_states);
  if (key->state == NULL && n_states > 0) {
    g_free(key->elem);
    g_free(key);
    g_return_val_if_reached(FALSE);
  }

  size_t counted_n_elems = n_elems;
  size_t counted_n_states = n_states;
  n_elems = 0;
  n_states = 0;

  if (!ntk_css_styler_entry_select(key, NULL, selector, 0, 0, &n_elems, &n_states)) {
    g_clear_pointer(&key->state, g_free);
    g_free(key->elem);
    g_free(key);
    g_return_val_if_reached(FALSE);
  }

  GValue* value = g_try_malloc0(sizeof(GValue));
  if (value == NULL) {
    g_free(key->state);
    g_free(key->elem);
    g_free(key);
    g_return_val_if_reached(FALSE);
  }

  g_debug("Creating property %s", declaration->property);
  if (g_str_equal(declaration->property, "color")) {
    key->prop = NTK_STYLER_PROPERTY_COLOR;

    if (declaration->values->length != 1) {
      g_free(key->state);
      g_free(key->elem);
      g_free(key);
      g_free(value);
      g_return_val_if_reached(FALSE);
    }

    NtkColor* color = ntk_css_styler_new_color(declaration->values->data[0]);
    if (color == NULL) {
      g_free(key->state);
      g_free(key->elem);
      g_free(key);
      g_free(value);
      g_return_val_if_reached(FALSE);
    }

    g_value_init(value, NTK_TYPE_COLOR);
    g_value_set_boxed(value, color);
  } else if (g_str_equal(declaration->property, "background-color")) {
    key->prop = NTK_STYLER_PROPERTY_BACKGROUND_COLOR;

    if (declaration->values->length != 1) {
      g_free(key->state);
      g_free(key->elem);
      g_free(key);
      g_free(value);
      g_return_val_if_reached(FALSE);
    }

    NtkColor* color = ntk_css_styler_new_color(declaration->values->data[0]);
    if (color == NULL) {
      g_free(key->state);
      g_free(key->elem);
      g_free(key);
      g_free(value);
      g_return_val_if_reached(FALSE);
    }

    g_value_init(value, NTK_TYPE_COLOR);
    g_value_set_boxed(value, color);
  } else if (g_str_equal(declaration->property, "background-image")) {
    key->prop = NTK_STYLER_PROPERTY_BACKGROUND_IMAGE;

    CssValue* css_value = declaration->values->data[0];

    if (css_value->unit != CSS_VALUE_URI) {
      g_free(key->state);
      g_free(key->elem);
      g_free(key);
      g_free(value);
      g_return_val_if_reached(FALSE);
    }

    g_value_init(value, G_TYPE_STRING);
    g_value_set_string(value, g_strdup(css_value->string));
  } else if (g_str_equal(declaration->property, "border-color")) {
    key->prop = NTK_STYLER_PROPERTY_BORDER_COLOR;

    if (declaration->values->length != 1) {
      g_free(key->state);
      g_free(key->elem);
      g_free(key);
      g_free(value);
      g_return_val_if_reached(FALSE);
    }

    NtkColor* color = ntk_css_styler_new_color(declaration->values->data[0]);
    if (color == NULL) {
      g_free(key->state);
      g_free(key->elem);
      g_free(key);
      g_free(value);
      g_return_val_if_reached(FALSE);
    }

    g_value_init(value, NTK_TYPE_COLOR);
    g_value_set_boxed(value, color);
  } else if (g_str_equal(declaration->property, "spacing-top")) {
    key->prop = NTK_STYLER_PROPERTY_PADDING_TOP;

    if (declaration->values->length != 1) {
      g_free(key->state);
      g_free(key->elem);
      g_free(key);
      g_free(value);
      g_return_val_if_reached(FALSE);
    }

    CssValue* css_value = declaration->values->data[0];

    if (css_value->unit != CSS_VALUE_PX) {
      g_free(key->state);
      g_free(key->elem);
      g_free(key);
      g_free(value);
      g_return_val_if_reached(FALSE);
    }

    g_value_init(value, G_TYPE_FLOAT);
    g_value_set_float(value, css_value->fValue);
  } else if (g_str_equal(declaration->property, "spacing-left")) {
    key->prop = NTK_STYLER_PROPERTY_PADDING_LEFT;

    if (declaration->values->length != 1) {
      g_free(key->state);
      g_free(key->elem);
      g_free(key);
      g_free(value);
      g_return_val_if_reached(FALSE);
    }

    CssValue* css_value = declaration->values->data[0];

    if (css_value->unit != CSS_VALUE_PX) {
      g_free(key->state);
      g_free(key->elem);
      g_free(key);
      g_free(value);
      g_return_val_if_reached(FALSE);
    }

    g_value_init(value, G_TYPE_FLOAT);
    g_value_set_float(value, css_value->fValue);
  } else if (g_str_equal(declaration->property, "padding-top")) {
    key->prop = NTK_STYLER_PROPERTY_PADDING_TOP;

    if (declaration->values->length != 1) {
      g_free(key->state);
      g_free(key->elem);
      g_free(key);
      g_free(value);
      g_return_val_if_reached(FALSE);
    }

    CssValue* css_value = declaration->values->data[0];

    if (css_value->unit != CSS_VALUE_PX) {
      g_free(key->state);
      g_free(key->elem);
      g_free(key);
      g_free(value);
      g_return_val_if_reached(FALSE);
    }

    g_value_init(value, G_TYPE_FLOAT);
    g_value_set_float(value, css_value->fValue);
  } else if (g_str_equal(declaration->property, "padding-left")) {
    key->prop = NTK_STYLER_PROPERTY_PADDING_LEFT;

    if (declaration->values->length != 1) {
      g_free(key->state);
      g_free(key->elem);
      g_free(key);
      g_free(value);
      g_return_val_if_reached(FALSE);
    }

    CssValue* css_value = declaration->values->data[0];

    if (css_value->unit != CSS_VALUE_PX) {
      g_free(key->state);
      g_free(key->elem);
      g_free(key);
      g_free(value);
      g_return_val_if_reached(FALSE);
    }

    g_value_init(value, G_TYPE_FLOAT);
    g_value_set_float(value, css_value->fValue);
  } else if (g_str_equal(declaration->property, "border-radius")) {
    key->prop = NTK_STYLER_PROPERTY_BORDER_COLOR;

    if (declaration->values->length != 1) {
      g_free(key->state);
      g_free(key->elem);
      g_free(key);
      g_free(value);
      g_return_val_if_reached(FALSE);
    }

    CssValue* css_value = declaration->values->data[0];

    if (css_value->unit != CSS_VALUE_PX) {
      g_free(key->state);
      g_free(key->elem);
      g_free(key);
      g_free(value);
      g_return_val_if_reached(FALSE);
    }

    g_value_init(value, G_TYPE_FLOAT);
    g_value_set_float(value, css_value->fValue);
  } else if (g_str_equal(declaration->property, "visibility")) {
    key->prop = NTK_STYLER_PROPERTY_VISIBILITY;

    if (declaration->values->length != 1) {
      g_free(key->state);
      g_free(key->elem);
      g_free(key);
      g_free(value);
      g_return_val_if_reached(FALSE);
    }

    CssValue* css_value = declaration->values->data[0];

    if (css_value->unit != CSS_VALUE_IDENT) {
      g_free(key->state);
      g_free(key->elem);
      g_free(key);
      g_free(value);
      g_return_val_if_reached(FALSE);
    }

    g_value_init(value, G_TYPE_BOOLEAN);

    if (g_str_equal(css_value->string, "visible")) {
      g_value_set_boolean(value, TRUE);
    } else if (g_str_equal(css_value->string, "hidden")) {
      g_value_set_boolean(value, FALSE);
    } else {
      g_free(key->state);
      g_free(key->elem);
      g_free(key);
      g_value_unset(value);
      g_free(value);
      g_return_val_if_reached(FALSE);
    }
  } else if (g_str_equal(declaration->property, "border-width")) {
    key->prop = NTK_STYLER_PROPERTY_BORDER_WIDTH;

    if (declaration->values->length != 1) {
      g_free(key->state);
      g_free(key->elem);
      g_free(key);
      g_free(value);
      g_return_val_if_reached(FALSE);
    }

    CssValue* css_value = declaration->values->data[0];

    if (css_value->unit != CSS_VALUE_PX) {
      g_free(key->state);
      g_free(key->elem);
      g_free(key);
      g_free(value);
      g_return_val_if_reached(FALSE);
    }

    g_value_init(value, G_TYPE_FLOAT);
    g_value_set_float(value, css_value->fValue);
  } else if (g_str_equal(declaration->property, "text-align")) {
    key->prop = NTK_STYLER_PROPERTY_TEXT_ALIGN;

    CssValue* css_value = declaration->values->data[0];

    if (css_value->unit != CSS_VALUE_IDENT) {
      g_free(key->state);
      g_free(key->elem);
      g_free(key);
      g_free(value);
      g_return_val_if_reached(FALSE);
    }

    g_value_init(value, G_TYPE_STRING);
    g_value_set_string(value, g_strdup(css_value->string));
  } else {
    g_free(key->state);
    g_free(key->elem);
    g_free(key);
    g_free(value);
    g_return_val_if_reached(FALSE);
  }

  if (G_VALUE_TYPE(value) == 0) {
    g_free(key->state);
    g_free(key->elem);
    g_free(key);
    g_return_val_if_reached(FALSE);
  }

  const gchar* key_str = ntk_styler_key_to_string(key);
  g_debug("Created key: %s", key_str);
  if (key_str != NULL) g_free(key_str);

  g_assert_cmpint(counted_n_elems, ==, n_elems);
  g_assert_cmpint(counted_n_states, ==, n_states);

  g_assert_cmpint(ntk_styler_element_get_depth(key->elem), ==, counted_n_elems);
  g_assert_cmpint(ntk_styler_state_get_depth(key->state), ==, counted_n_states);

  g_assert_cmpint(ntk_styler_element_get_depth(key->elem), ==, n_elems);
  g_assert_cmpint(ntk_styler_state_get_depth(key->state), ==, n_states);

  g_assert_cmpint(ntk_styler_element_get_depth(key->elem), >, 0);

  *key_out = key;
  *value_out = value;
  return TRUE;
}

static GHashTable* ntk_css_styler_export(NtkStyler* styler) {
  NtkCSSStyler* self = NTK_CSS_STYLER(styler);
  NtkCSSStylerPrivate* priv = NTK_CSS_STYLER_PRIVATE(self);

  GHashTable* tbl = g_hash_table_new_full(
    (GHashFunc)ntk_styler_key_hash, (GEqualFunc)ntk_styler_key_equal, (GDestroyNotify)ntk_css_styler_default_key_free,
    (GDestroyNotify)ntk_css_styler_default_value_free
  );
  g_return_val_if_fail(tbl != NULL, NULL);
  g_return_val_if_fail(priv->output != NULL, NULL);

  for (size_t i = 0; i < priv->output->stylesheet->rules.length; i++) {
    CssRule* rule = priv->output->stylesheet->rules.data[i];
    if (rule == NULL) continue;
    if (rule->type != CssRuleStyle) continue;

    CssStyleRule* style_rule = (CssStyleRule*)rule;
    for (size_t c = 0; c < style_rule->declarations->length; c++) {
      CssDeclaration* declaration = style_rule->declarations->data[c];
      if (declaration == NULL) continue;

      for (size_t i = 0; i < style_rule->selectors->length; i++) {
        CssSelector* selector = style_rule->selectors->data[i];
        if (selector == NULL) continue;
        if (selector->match != CssSelMatchTag) continue;

        NtkStylerKey* key = NULL;
        GValue* value = NULL;
        if (!ntk_css_styler_entry_create(style_rule, declaration, selector, &key, &value)) continue;

        g_return_val_if_fail(key != NULL, FALSE);
        g_return_val_if_fail(value != NULL, FALSE);

        g_hash_table_insert(tbl, key, value);
      }
    }
  }
  return tbl;
}

static void ntk_css_styler_finalize(GObject* obj) {
  NtkCSSStyler* self = NTK_CSS_STYLER(obj);
  NtkCSSStylerPrivate* priv = NTK_CSS_STYLER_PRIVATE(self);

  g_clear_pointer(&priv->output, css_destroy_output);

  G_OBJECT_CLASS(ntk_css_styler_parent_class)->finalize(obj);
}

static void ntk_css_styler_class_init(NtkCSSStylerClass* klass) {
  GObjectClass* object_class = G_OBJECT_CLASS(klass);
  NtkStylerClass* styler_class = NTK_STYLER_CLASS(klass);

  object_class->finalize = ntk_css_styler_finalize;

  styler_class->export = ntk_css_styler_export;
}

static void ntk_css_styler_init(NtkCSSStyler* self) {
  self->priv = ntk_css_styler_get_instance_private(self);
}

NtkStyler* ntk_css_styler_new() {
  return NTK_STYLER(g_object_new(NTK_CSS_TYPE_STYLER, NULL));
}

gboolean ntk_css_styler_load(NtkCSSStyler* self, const gchar* str, size_t length, GError** error) {
  g_return_val_if_fail(NTK_CSS_IS_STYLER(self), FALSE);
  g_return_val_if_fail(str != NULL, FALSE);
  g_return_val_if_fail(length > 0, FALSE);

  g_return_val_if_fail(error == NULL || *error == NULL, FALSE);
  NtkCSSStylerPrivate* priv = NTK_CSS_STYLER_PRIVATE(self);

  g_clear_pointer(&priv->output, css_destroy_output);

  priv->output = css_parse_string(str, length, CssParserModeStylesheet);
  g_return_val_if_fail(priv->output != NULL, FALSE);

  if (priv->output->errors.length > 0) {
    ntk_css_error_set(error, priv->output->errors.data[0]);
    g_clear_pointer(&priv->output, css_destroy_output);
    return FALSE;
  }

  for (size_t i = 0; i < priv->output->stylesheet->rules.length; i++) {
    CssRule* rule = priv->output->stylesheet->rules.data[i];
    if (rule == NULL) continue;
    if (rule->type != CssRuleStyle) continue;

    CssStyleRule* style_rule = (CssStyleRule*)rule;
    for (size_t c = 0; c < style_rule->declarations->length; c++) {
      CssDeclaration* declaration = style_rule->declarations->data[c];
      if (declaration == NULL) continue;

      for (size_t i = 0; i < style_rule->selectors->length; i++) {
        CssSelector* selector = style_rule->selectors->data[i];
        if (selector != NULL) continue;
        if (selector->match != CssSelMatchTag) continue;

        NtkStylerKey* key = NULL;
        GValue* value = NULL;
        if (!ntk_css_styler_entry_create(style_rule, declaration, selector, &key, &value)) {
          return FALSE;
        }

        g_return_val_if_fail(key != NULL, FALSE);
        g_return_val_if_fail(value != NULL, FALSE);

        ntk_css_styler_default_key_free(key);
        ntk_css_styler_default_value_free(value);
      }
    }
  }
  return TRUE;
}

gboolean ntk_css_styler_load_path(NtkCSSStyler* self, const gchar* path, GError** error) {
  g_return_val_if_fail(NTK_CSS_IS_STYLER(self), FALSE);
  g_return_val_if_fail(path != NULL, FALSE);

  gchar* contents = NULL;
  size_t contents_length = 0;
  g_return_val_if_fail(g_file_get_contents(path, &contents, &contents_length, error), FALSE);

  gboolean result = ntk_css_styler_load(self, contents, contents_length, error);
  g_free(contents);
  return result;
}

gboolean ntk_css_styler_load_file(NtkCSSStyler* self, GFile* file, GError** error) {
  g_return_val_if_fail(NTK_CSS_IS_STYLER(self), FALSE);
  g_return_val_if_fail(file != NULL, FALSE);

  gchar* contents = NULL;
  size_t contents_length = 0;
  g_return_val_if_fail(g_file_load_contents(file, NULL, &contents, &contents_length, NULL, error), FALSE);

  gboolean result = ntk_css_styler_load(self, contents, contents_length, error);
  g_free(contents);
  return result;
}

gboolean ntk_css_styler_load_resource(NtkCSSStyler* self, const gchar* path, GError** error) {
  g_return_val_if_fail(NTK_CSS_IS_STYLER(self), FALSE);
  g_return_val_if_fail(path != NULL, FALSE);

  GBytes* bytes = g_resources_lookup_data(path, G_RESOURCE_LOOKUP_FLAGS_NONE, error);
  g_return_val_if_fail(bytes != NULL, FALSE);

  size_t contents_length = 0;
  gchar* contents = g_bytes_unref_to_data(bytes, &contents_length);
  g_return_val_if_fail(contents != NULL, FALSE);

  gboolean result = ntk_css_styler_load(self, contents, contents_length, error);
  g_free(contents);
  return result;
}
