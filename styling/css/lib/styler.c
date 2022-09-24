#include <ntk/styling/css/styler.h>
#include "styler-priv.h"

#define NTK_CSS_STYLER_PRIVATE(self)                                                                                 \
  ((self)->priv == NULL ? ntk_css_styler_get_instance_private(self) : (self)->priv)

G_DEFINE_TYPE_WITH_PRIVATE(NtkCSSStyler, ntk_css_styler, NTK_TYPE_STYLER);

static void ntk_css_styler_default_key_free(NtkStylerKey* key) {
  g_free(key->elem);
  g_free(key);
}

static void ntk_css_styler_default_value_free(GValue* value) {
  g_value_unset(value);
  g_free(value);
}

static gboolean ntk_css_styler_entry_create(CssStyleRule* rule, CssDeclaration* declaration, NtkStylerKey** key_out, GValue** value_out) {
  NtkStylerKey* key = g_try_malloc0(sizeof (NtkStylerKey));
  g_return_val_if_fail(key != NULL, FALSE);

  size_t n_elems = 0;
  size_t n_states = 0;
  for (size_t i = 0; i < rule->selectors->length; i++) {
    CssSelector* selector = rule->selectors->data[i];
    if (selector->match == CssSelMatchTag) n_elems++;
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

  key->elem = g_try_malloc0(sizeof (NtkStylerElement) * n_elems);
  if (key->elem == NULL) {
    g_free(key);
    g_return_val_if_reached(FALSE);
  }

  key->state = g_try_malloc0(sizeof (NtkStylerState) * n_states);
  if (key->state == NULL) {
    g_free(key->elem);
    g_free(key);
    g_return_val_if_reached(FALSE);
  }

  n_elems = 0;
  for (size_t i = 0; i < rule->selectors->length; i++) {
    CssSelector* selector = rule->selectors->data[i];
    if (selector->match != CssSelMatchTag) continue;

    if (g_strcmp0(selector->data->value, "p")) {
      key->elem[n_elems] = NTK_STYLER_ELEMENT_TEXT;
    } else if (g_strcmp0(selector->data->value, "button")) {
      key->elem[n_elems] = NTK_STYLER_ELEMENT_BUTTON;
    } else if (g_strcmp0(selector->data->value, "context-button")) {
      key->elem[n_elems] = NTK_STYLER_ELEMENT_CONTEXT_BUTTON;
    } else {
      continue;
    }

    n_elems++;
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

  if (g_strcmp0(declaration->property, "color")) {
    key->prop = NTK_STYLER_PROPERTY_COLOR;
  } else if (g_strcmp0(declaration->property, "background-color")) {
    key->prop = NTK_STYLER_PROPERTY_BACKGROUND_COLOR;
  } else if (g_strcmp0(declaration->property, "border-color")) {
    key->prop = NTK_STYLER_PROPERTY_BORDER_COLOR;
  } else if (g_strcmp0(declaration->property, "padding")) {
    key->prop = NTK_STYLER_PROPERTY_PADDING;
  } else if (g_strcmp0(declaration->property, "border-radius")) {
    key->prop = NTK_STYLER_PROPERTY_BORDER_COLOR;
  } else if (g_strcmp0(declaration->property, "visibility")) {
    key->prop = NTK_STYLER_PROPERTY_VISIBILITY;
  } else if (g_strcmp0(declaration->property, "border-width")) {
    key->prop = NTK_STYLER_PROPERTY_BORDER_WIDTH;
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

  GHashTable* tbl = g_hash_table_new_full((GHashFunc)ntk_styler_key_hash, (GEqualFunc)ntk_styler_key_equal, (GDestroyNotify)ntk_css_styler_default_key_free, (GDestroyNotify)ntk_css_styler_default_value_free);
  g_return_val_if_fail(tbl != NULL, NULL);

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

static gboolean ntk_css_styler_has_style_property(NtkStyler* styler, NtkStylerKey key) {
  return FALSE;
}

static gboolean ntk_css_styler_get_style_property(NtkStyler* styler, NtkStylerKey key, GValue* value) {
  return FALSE;
}

static gboolean ntk_css_styler_set_style_property(NtkStyler* styler, NtkStylerKey key, const GValue* value) {
  return FALSE;
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
  styler_class->has_style_property = ntk_css_styler_has_style_property;
  styler_class->get_style_property = ntk_css_styler_get_style_property;
  styler_class->set_style_property = ntk_css_styler_set_style_property;
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
  return TRUE;
}
