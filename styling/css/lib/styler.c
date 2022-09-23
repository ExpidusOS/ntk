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

static gboolean ntk_css_styler_entry_create(CssRule* rule, NtkStylerKey** key_out, GValue** value_out) {
  switch (rule->type) {
    case CssRuleUnkown: return FALSE;
    case CssRuleImport: return FALSE;
  }

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

    NtkStylerKey* key = NULL;
    GValue* value = NULL;
    if (!ntk_css_styler_entry_create(rule, &key, &value)) continue;
    g_hash_table_insert(tbl, key, value);
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

  styler->export = ntk_css_styler_export;
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
