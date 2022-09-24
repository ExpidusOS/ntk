#include "styler-priv.h"
#include <ntk/styling/css/styler.h>

#define NTK_CSS_STYLER_PRIVATE(self) ((self)->priv == NULL ? ntk_css_styler_get_instance_private(self) : (self)->priv)

G_DEFINE_TYPE_WITH_PRIVATE(NtkCSSStyler, ntk_css_styler, NTK_TYPE_STYLER);

static void ntk_css_styler_default_key_free(NtkStylerKey* key) {
  g_free(key->elem);
  g_free(key);
}

static void ntk_css_styler_default_value_free(GValue* value) {
  g_value_unset(value);
  g_free(value);
}

static gboolean
ntk_css_styler_entry_create(CssStyleRule* rule, CssDeclaration* declaration, NtkStylerKey** key_out, GValue** value_out) {
  NtkStylerKey* key = g_try_malloc0(sizeof(NtkStylerKey));
  g_return_val_if_fail(key != NULL, FALSE);

  size_t n_elems = 0;
  size_t n_states = 0;
  gboolean has_input = FALSE;
  gboolean has_input_type = FALSE;
  gboolean has_scrollbar = FALSE;
  gboolean has_scrollbar_type = FALSE;
  for (size_t i = 0; i < rule->selectors->length; i++) {
    CssSelector* selector = rule->selectors->data[i];
    switch (selector->match) {
      case CssSelMatchTag:
        if (g_str_equal(selector->data->value, "input")) has_input = TRUE;
        else if (g_str_equal(selector->data->value, "scrollbar")) has_scrollbar = TRUE;
        n_elems++;
        break;
      case CssSelMatchAttrExact:
        if (has_input && g_str_equal(selector->data->argument, "type")) {
          if (g_str_equal(selector->data->value, "button")) {
            has_input_type = TRUE;
            n_elems++;
          } else if (g_str_equal(selector->data->value, "toggle")) {
            has_input_type = TRUE;
            n_elems++;
          } else if (g_str_equal(selector->data->value, "text")) {
            has_input_type = TRUE;
            n_elems++;
          } else if (g_str_equal(selector->data->value, "range")) {
            has_input_type = TRUE;
            n_elems++;
          } else if (g_str_equal(selector->data->value, "checkbox")) {
            has_input_type = TRUE;
            n_elems++;
          } else {
            continue;
          }
        } else if (has_scrollbar && g_str_equal(selector->data->argument, "type")) {
          if (g_str_equal(selector->data->value, "vertical")) {
            has_scrollbar_type = TRUE;
            n_elems++;
          } else if (g_str_equal(selector->data->value, "horizontal")) {
            has_scrollbar_type = TRUE;
            n_elems++;
          } else {
            continue;
          }
        }
        break;
      default:
        break;
    }
    switch (selector->pseudo) {
      case CssPseudoHover:
      case CssPseudoActive:
      case CssPseudoSelection:
      case CssPseudoDefault:
        n_states++;
        break;
      default:
        break;
    }
  }

  if (!has_input_type && has_input) n_elems++;
  if (!has_scrollbar_type && has_scrollbar) n_elems += 2;

  key->elem = g_try_malloc0(sizeof(NtkStylerElement) * n_elems);
  if (key->elem == NULL) {
    g_free(key);
    g_return_val_if_reached(FALSE);
  }

  key->state = g_try_malloc0(sizeof(NtkStylerState) * n_states);
  if (key->state == NULL) {
    g_free(key->elem);
    g_free(key);
    g_return_val_if_reached(FALSE);
  }

  n_elems = 0;
  for (size_t i = 0; i < rule->selectors->length; i++) {
    CssSelector* selector = rule->selectors->data[i];
    if (selector->match == CssSelMatchTag) {
      if (g_str_equal(selector->data->value, "p")) {
        key->elem[n_elems] = NTK_STYLER_ELEMENT_TEXT;
      } else if (g_str_equal(selector->data->value, "button")) {
        key->elem[n_elems] = NTK_STYLER_ELEMENT_BUTTON;
      } else if (g_str_equal(selector->data->value, "input")) {
        has_input = TRUE;
      } else if (g_str_equal(selector->data->value, "scrollbar")) {
        has_scrollbar = TRUE;
      } else if (g_str_equal(selector->data->value, "progress")) {
        key->elem[n_elems] = NTK_STYLER_ELEMENT_PROGRESS;
      } else if (g_str_equal(selector->data->value, "textarea")) {
        key->elem[n_elems] = NTK_STYLER_ELEMENT_TEXTAREA;
      } else if (g_str_equal(selector->data->value, "tab")) {
        key->elem[n_elems] = NTK_STYLER_ELEMENT_TAB;
      } else if (g_str_equal(selector->data->value, "option")) {
        key->elem[n_elems] = NTK_STYLER_ELEMENT_OPTION;
      } else if (g_str_equal(selector->data->value, "window")) {
        key->elem[n_elems] = NTK_STYLER_ELEMENT_WINDOW;
      } else if (g_str_equal(selector->data->value, "window-header")) {
        key->elem[n_elems] = NTK_STYLER_ELEMENT_WINDOW_HEADER;
      } else {
        continue;
      }

      n_elems++;
    } else if (selector->match == CssSelMatchAttrExact) {
      if (has_input && g_str_equal(selector->data->argument, "type")) {
        if (g_str_equal(selector->data->value, "button")) {
          key->elem[n_elems] = NTK_STYLER_ELEMENT_BUTTON;
          has_input_type = TRUE;
        } else if (g_str_equal(selector->data->value, "toggle")) {
          key->elem[n_elems] = NTK_STYLER_ELEMENT_INPUT_TOGGLE;
          has_input_type = TRUE;
        } else if (g_str_equal(selector->data->value, "text")) {
          key->elem[n_elems] = NTK_STYLER_ELEMENT_INPUT_TEXT;
          has_input_type = TRUE;
        } else if (g_str_equal(selector->data->value, "range")) {
          key->elem[n_elems] = NTK_STYLER_ELEMENT_INPUT_SLIDER;
          has_input_type = TRUE;
        } else if (g_str_equal(selector->data->value, "checkbox")) {
          key->elem[n_elems] = NTK_STYLER_ELEMENT_INPUT_CHECKBOX;
          has_input_type = TRUE;
        } else {
          continue;
        }
        n_elems++;
      } else if (has_scrollbar && g_str_equal(selector->data->argument, "type")) {
        if (g_str_equal(selector->data->value, "vertical")) {
          key->elem[n_elems] = NTK_STYLER_ELEMENT_SCROLLBAR_VERTICAL;
          has_scrollbar_type = TRUE;
        } else if (g_str_equal(selector->data->value, "horizontal")) {
          key->elem[n_elems] = NTK_STYLER_ELEMENT_SCROLLBAR_HORIZONTAL;
          has_scrollbar_type = TRUE;
        } else {
          continue;
        }
        n_elems++;
      }
    }
  }

  if (!has_input_type && has_input) key->elem[n_elems++] = NTK_STYLER_ELEMENT_INPUT_TEXT;
  if (!has_scrollbar_type && has_scrollbar) {
    key->elem[n_elems++] = NTK_STYLER_ELEMENT_SCROLLBAR_VERTICAL;
    key->elem[n_elems++] = NTK_STYLER_ELEMENT_SCROLLBAR_HORIZONTAL;
  }

  n_states = 0;
  for (size_t i = 0; i < rule->selectors->length; i++) {
    CssSelector* selector = rule->selectors->data[i];
    if (selector->match == CssSelMatchTag) n_elems++;
    switch (selector->pseudo) {
      case CssPseudoHover:
        key->state[n_states++] = NTK_STYLER_STATE_HOVER;
        break;
      case CssPseudoActive:
        key->state[n_states++] = NTK_STYLER_STATE_ACTIVE;
        break;
      case CssPseudoSelection:
        key->state[n_states++] = NTK_STYLER_STATE_SELECTION;
        break;
      case CssPseudoDefault:
        key->state[n_states++] = NTK_STYLER_STATE_NORMAL;
        break;
      default:
        break;
    }
  }

  if (g_str_equal(declaration->property, "color")) {
    key->prop = NTK_STYLER_PROPERTY_COLOR;
  } else if (g_str_equal(declaration->property, "background-color")) {
    key->prop = NTK_STYLER_PROPERTY_BACKGROUND_COLOR;
  } else if (g_str_equal(declaration->property, "background-image")) {
    key->prop = NTK_STYLER_PROPERTY_BACKGROUND_IMAGE;
  } else if (g_str_equal(declaration->property, "border-color")) {
    key->prop = NTK_STYLER_PROPERTY_BORDER_COLOR;
  } else if (g_str_equal(declaration->property, "spacing-top")) {
    key->prop = NTK_STYLER_PROPERTY_PADDING_TOP;
  } else if (g_str_equal(declaration->property, "spacing-left")) {
    key->prop = NTK_STYLER_PROPERTY_PADDING_LEFT;
  } else if (g_str_equal(declaration->property, "padding-top")) {
    key->prop = NTK_STYLER_PROPERTY_PADDING_TOP;
  } else if (g_str_equal(declaration->property, "padding-left")) {
    key->prop = NTK_STYLER_PROPERTY_PADDING_LEFT;
  } else if (g_str_equal(declaration->property, "border-radius")) {
    key->prop = NTK_STYLER_PROPERTY_BORDER_COLOR;
  } else if (g_str_equal(declaration->property, "visibility")) {
    key->prop = NTK_STYLER_PROPERTY_VISIBILITY;
  } else if (g_str_equal(declaration->property, "border-width")) {
    key->prop = NTK_STYLER_PROPERTY_BORDER_WIDTH;
  } else if (g_str_equal(declaration->property, "text-align")) {
    key->prop = NTK_STYLER_PROPERTY_TEXT_ALIGN;
  } else {
    g_free(key->state);
    g_free(key->elem);
    g_free(key);
    g_return_val_if_reached(FALSE);
  }

  *key_out = key;
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

      NtkStylerKey* key = NULL;
      GValue* value = NULL;
      if (!ntk_css_styler_entry_create(style_rule, declaration, &key, &value)) continue;
      g_hash_table_insert(tbl, key, value);
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
  g_return_val_if_fail(length < 0, FALSE);

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

      NtkStylerKey* key = NULL;
      GValue* value = NULL;
      if (!ntk_css_styler_entry_create(style_rule, declaration, &key, &value)) {
        return FALSE;
      }

      ntk_css_styler_default_key_free(key);
      ntk_css_styler_default_value_free(value);
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
