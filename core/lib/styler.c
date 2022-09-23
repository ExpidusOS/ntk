#include <ntk/styler.h>
#include <math.h>
#include "context-priv.h"
#include "styler-priv.h"

#define NTK_STYLER_PRIVATE(self) ntk_styler_get_instance_private(self)

G_DEFINE_TYPE_WITH_PRIVATE(NtkStyler, ntk_styler, G_TYPE_OBJECT);

size_t ntk_styler_element_get_depth(NtkStylerElement* elem) {
  g_return_val_if_fail(elem != NULL, -1);

  size_t i = 0;
  while (elem[i] < NTK_STYLER_N_ELEMENTS && elem[i] > 0) i++;
  return i;
}

guint ntk_styler_key_hash(NtkStylerKey* key) {
  g_return_val_if_fail(key != NULL, -1);
  g_return_val_if_fail(key->elem != NULL, -1);

  size_t elem_count = ntk_styler_element_get_depth(key->elem);
  size_t n_elem_digits = floor(log10(NTK_STYLER_N_ELEMENTS)) + 1;
  size_t elem_digits = floor(log10((10 * n_elem_digits) + elem_count)) + 1;
  gint elem = 0;

  for (size_t i = 0; i < elem_count; i++) elem += (10 * elem_digits) + key->elem[i];

  size_t n_prop_digits = floor(log10(NTK_STYLER_N_PROPERTIES)) + 1;
  size_t n_state_digits = floor(log10(NTK_STYLER_N_STATES)) + 1;
  return (elem << n_elem_digits) + (key->prop << n_prop_digits) + (key->state << n_state_digits);
}

gboolean ntk_styler_key_equal(NtkStylerKey* a, NtkStylerKey* b) {
  return (a->elem == b->elem) && (a->prop == b->prop) && (a->state == b->state);
}

static gboolean ntk_styler_apply_internal(NtkStyler* self, NtkContext* ctx, struct nk_style style) {
  NtkStylerPrivate* priv = NTK_STYLER_PRIVATE(self);

  priv->saved_style = ctx->priv->nk.style;
  ctx->priv->nk.style = style;
  ctx->priv->nk.style.font = priv->saved_style.font;
  return TRUE;
}

static void ntk_styler_default_key_free(NtkStylerKey* key) {
  g_free(key->elem);
  g_free(key);
}

static void ntk_styler_default_value_free(GValue* value) {
  g_value_unset(value);
  g_free(value);
}

static void ntk_styler_constructed(GObject* obj) {
  G_OBJECT_CLASS(ntk_styler_parent_class)->constructed(obj);

  NtkStyler* self = NTK_STYLER(obj);
  NtkStylerPrivate* priv = NTK_STYLER_PRIVATE(self);

  priv->styles = g_hash_table_new_full((GHashFunc)ntk_styler_key_hash, (GEqualFunc)ntk_styler_key_equal, (GDestroyNotify)ntk_styler_default_key_free, (GDestroyNotify)ntk_styler_default_value_free);
}

static void ntk_styler_finalize(GObject* obj) {
  NtkStyler* self = NTK_STYLER(obj);
  NtkStylerPrivate* priv = NTK_STYLER_PRIVATE(self);

  g_clear_pointer(&priv->styles, g_hash_table_unref);

  G_OBJECT_CLASS(ntk_styler_parent_class)->finalize(obj);
}

static GHashTable* ntk_styler_default_export(NtkStyler* self) {
  NtkStylerPrivate* priv = NTK_STYLER_PRIVATE(self);
  return g_hash_table_ref(priv->styles);
}

static gboolean ntk_styler_default_has_style_property(NtkStyler* self, NtkStylerKey key) {
  NtkStylerPrivate* priv = NTK_STYLER_PRIVATE(self);

  NtkStylerKey* impl_key = g_try_malloc0(sizeof (NtkStylerKey));
  g_return_val_if_fail(impl_key != NULL, FALSE);

  size_t n_elems = ntk_styler_element_get_depth(key.elem);

  impl_key->elem = g_try_malloc(sizeof (NtkStylerElement) * n_elems);
  if (impl_key->elem == NULL) {
    g_free(impl_key);
    g_return_val_if_reached(FALSE);
  }

  for (size_t i = 0; i < n_elems; i++) impl_key->elem[i] = key.elem[i];

  impl_key->state = key.state;
  impl_key->prop = key.prop;

  gboolean result = g_hash_table_contains(priv->styles, impl_key);
  g_free(impl_key->elem);
  g_free(impl_key);
  return result;
}

static gboolean ntk_styler_default_get_style_property(NtkStyler* self, NtkStylerKey key, GValue* value) {
  NtkStylerPrivate* priv = NTK_STYLER_PRIVATE(self);

  NtkStylerKey* impl_key = g_try_malloc0(sizeof (NtkStylerKey));
  g_return_val_if_fail(impl_key != NULL, FALSE);

  size_t n_elems = ntk_styler_element_get_depth(key.elem);

  impl_key->elem = g_try_malloc(sizeof (NtkStylerElement) * n_elems);
  if (impl_key->elem == NULL) {
    g_free(impl_key);
    g_return_val_if_reached(FALSE);
  }

  for (size_t i = 0; i < n_elems; i++) impl_key->elem[i] = key.elem[i];

  impl_key->state = key.state;
  impl_key->prop = key.prop;

  const GValue* srcval = g_hash_table_lookup(priv->styles, impl_key);
  if (srcval != NULL) g_value_copy(srcval, value);

  g_free(impl_key->elem);
  g_free(impl_key);
  return srcval != NULL;
}

static gboolean ntk_styler_default_set_style_property(NtkStyler* self, NtkStylerEntry entry) {
  NtkStylerPrivate* priv = NTK_STYLER_PRIVATE(self);

  NtkStylerKey* key = g_try_malloc0(sizeof (NtkStylerKey));
  g_return_val_if_fail(key != NULL, FALSE);

  size_t n_elems = ntk_styler_element_get_depth(entry.key.elem);

  key->elem = g_try_malloc(sizeof (NtkStylerElement) * n_elems);
  if (key->elem == NULL) {
    g_free(key);
    g_return_val_if_reached(FALSE);
  }

  for (size_t i = 0; i < n_elems; i++) key->elem[i] = entry.key.elem[i];

  key->state = entry.key.state;
  key->prop = entry.key.prop;

  GValue* value = g_try_malloc0(sizeof (GValue));
  if (value == NULL) {
    g_free(key->elem);
    g_free(key);
    g_return_val_if_reached(FALSE);
  }

  g_value_copy(&entry.value, value);
  g_hash_table_insert(priv->styles, key, value);
  return TRUE;
}

static void ntk_styler_class_init(NtkStylerClass* klass) {
  GObjectClass* object_class = G_OBJECT_CLASS(klass);

  object_class->constructed = ntk_styler_constructed;
  object_class->finalize = ntk_styler_finalize;

  klass->export = ntk_styler_default_export;
  klass->has_style_property = ntk_styler_default_has_style_property;
  klass->get_style_property = ntk_styler_default_get_style_property;
  klass->set_style_property = ntk_styler_default_set_style_property;
}

static void ntk_styler_init(NtkStyler* self) {}

NtkStyler* ntk_styler_new() {
  return NTK_STYLER(g_object_new(NTK_TYPE_STYLER, NULL));
}

gboolean ntk_styler_import(NtkStyler* self, NtkStylerEntry* styles, size_t n_styles) {
  g_return_val_if_fail(NTK_IS_STYLER(self), FALSE);
  g_return_val_if_fail(styles != NULL, FALSE);
  NtkStylerClass* klass = NTK_STYLER_GET_CLASS(self);
  g_return_val_if_fail(klass != NULL, FALSE);
  g_return_val_if_fail(klass->set_style_property != NULL, FALSE);

  for (size_t i = 0; i < n_styles; i++) {
    NtkStylerEntry entry = styles[i];
    g_return_val_if_fail(klass->set_style_property(self, entry), FALSE);
  }
  return TRUE;
}

GHashTable* ntk_styler_export(NtkStyler* self) {
  g_return_val_if_fail(NTK_IS_STYLER(self), NULL);
  NtkStylerClass* klass = NTK_STYLER_GET_CLASS(self);
  g_return_val_if_fail(klass != NULL, NULL);
  g_return_val_if_fail(klass->export != NULL, NULL);
  return klass->export(self);
}

gboolean ntk_styler_has_style_property(NtkStyler* self, NtkStylerKey key) {
  g_return_val_if_fail(NTK_IS_STYLER(self), FALSE);
  NtkStylerClass* klass = NTK_STYLER_GET_CLASS(self);
  g_return_val_if_fail(klass != NULL, FALSE);
  g_return_val_if_fail(klass->has_style_property != NULL, FALSE);
  return klass->has_style_property(self, key);
}

gboolean ntk_styler_get_style_property(NtkStyler* self, NtkStylerKey key, GValue* value) {
  g_return_val_if_fail(NTK_IS_STYLER(self), FALSE);
  NtkStylerClass* klass = NTK_STYLER_GET_CLASS(self);
  g_return_val_if_fail(klass != NULL, FALSE);
  g_return_val_if_fail(klass->get_style_property != NULL, FALSE);
  return klass->get_style_property(self, key, value);
}

gboolean ntk_styler_set_style_property(NtkStyler* self, NtkStylerKey key, const GValue* value) {
  g_return_val_if_fail(NTK_IS_STYLER(self), FALSE);
  NtkStylerClass* klass = NTK_STYLER_GET_CLASS(self);
  g_return_val_if_fail(klass != NULL, FALSE);
  g_return_val_if_fail(klass->set_style_property != NULL, FALSE);

  NtkStylerEntry entry = { .key = key };
  g_value_copy(value, &entry.value);

  gboolean result = klass->set_style_property(self, entry);
  g_value_unset(&entry.value);
  return result;
}

gboolean ntk_styler_apply(NtkStyler* self, NtkContext* ctx) {
  g_return_val_if_fail(NTK_IS_STYLER(self), FALSE);
  g_return_val_if_fail(NTK_IS_CONTEXT(self), FALSE);

  struct nk_style styles = {};
  styles.font = ctx->priv->nk.style.font;
  return ntk_styler_apply_internal(self, ctx, styles);
}

void ntk_styler_restore(NtkStyler* self, NtkContext* ctx) {
  g_return_if_fail(NTK_IS_STYLER(self));
  g_return_if_fail(NTK_IS_CONTEXT(self));
  NtkStylerPrivate* priv = NTK_STYLER_PRIVATE(self);

  g_return_if_fail(ntk_styler_apply_internal(self, ctx, priv->saved_style));
}
