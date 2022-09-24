#include "context-priv.h"
#include "styler-priv.h"
#include <math.h>
#include <ntk/styler.h>

#define NTK_STYLER_PRIVATE(self) ntk_styler_get_instance_private(self)

G_DEFINE_TYPE_WITH_PRIVATE(NtkStyler, ntk_styler, G_TYPE_OBJECT);

size_t ntk_styler_element_get_depth(NtkStylerElement* elem) {
  g_return_val_if_fail(elem != NULL, -1);

  size_t i = 0;
  while (elem[i] > 0) {
    g_assert(elem[i] < NTK_STYLER_N_ELEMENTS);
    i++;
  }
  return i;
}

size_t ntk_styler_state_get_depth(NtkStylerState* state) {
  g_return_val_if_fail(state != NULL, -1);

  size_t i = 0;
  while (state[i] > 0) {
    g_assert(state[i] < NTK_STYLER_N_STATES);
    i++;
  }
  return i;
}

guint ntk_styler_key_hash(NtkStylerKey* key) {
  g_return_val_if_fail(key != NULL, -1);
  g_return_val_if_fail(key->elem != NULL, -1);
  g_return_val_if_fail(key->state != NULL, -1);

  size_t elem_count = ntk_styler_element_get_depth(key->elem);
  size_t n_elem_digits = floor(log10(NTK_STYLER_N_ELEMENTS)) + 1;
  size_t elem_digits = floor(log10((10 * n_elem_digits) + elem_count)) + 1;
  gint elem = 0;

  for (size_t i = 0; i < elem_count; i++)
    elem += (10 * elem_digits) + key->elem[i];

  size_t state_count = ntk_styler_state_get_depth(key->state);
  size_t n_state_digits = floor(log10(NTK_STYLER_N_STATES)) + 1;
  size_t state_digits = floor(log10((10 * n_state_digits) + state_count)) + 1;
  gint state = 0;

  for (size_t i = 0; i < state_count; i++)
    state += (10 * state_digits) + key->state[i];

  size_t n_prop_digits = floor(log10(NTK_STYLER_N_PROPERTIES)) + 1;
  return (elem << elem_digits) + (state << state_digits) + (key->prop << n_prop_digits);
}

gboolean ntk_styler_key_equal(NtkStylerKey* a, NtkStylerKey* b) {
  size_t a_elem_count = ntk_styler_element_get_depth(a->elem);
  size_t b_elem_count = ntk_styler_element_get_depth(b->elem);
  if (a_elem_count != b_elem_count) return FALSE;

  size_t a_state_count = ntk_styler_state_get_depth(a->state);
  size_t b_state_count = ntk_styler_state_get_depth(b->state);
  if (a_state_count != b_state_count) return FALSE;

  for (size_t i = 0; i < a_elem_count; i++) {
    if (a->elem[i] != b->elem[i]) return FALSE;
  }

  for (size_t i = 0; i < a_state_count; i++) {
    if (a->state[i] != b->state[i]) return FALSE;
  }

  return a->prop == b->prop;
}

static gboolean ntk_styler_apply_internal(NtkStyler* self, NtkContext* ctx, struct nk_style style) {
  NtkStylerPrivate* priv = NTK_STYLER_PRIVATE(self);

  priv->saved_style = ctx->priv->nk.style;
  ctx->priv->nk.style = style;
  ctx->priv->nk.style.font = priv->saved_style.font;
  return TRUE;
}

static void ntk_styler_default_key_free(NtkStylerKey* key) {
  g_free(key->state);
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

  priv->styles = g_hash_table_new_full(
    (GHashFunc)ntk_styler_key_hash, (GEqualFunc)ntk_styler_key_equal, (GDestroyNotify)ntk_styler_default_key_free,
    (GDestroyNotify)ntk_styler_default_value_free
  );
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
  GHashTable* tbl = ntk_styler_export(self);
  g_return_val_if_fail(tbl != NULL, FALSE);

  NtkStylerKey* impl_key = g_try_malloc0(sizeof(NtkStylerKey));
  if (impl_key == NULL) {
    g_hash_table_unref(tbl);
    g_return_val_if_reached(FALSE);
  }

  size_t n_elems = ntk_styler_element_get_depth(key.elem);

  impl_key->elem = g_try_malloc(sizeof(NtkStylerElement) * n_elems);
  if (impl_key->elem == NULL) {
    g_free(impl_key);
    g_hash_table_unref(tbl);
    g_return_val_if_reached(FALSE);
  }

  for (size_t i = 0; i < n_elems; i++)
    impl_key->elem[i] = key.elem[i];

  size_t n_states = ntk_styler_state_get_depth(key.state);

  impl_key->state = g_try_malloc(sizeof(NtkStylerState) * n_states);
  if (impl_key->state == NULL) {
    g_free(impl_key->elem);
    g_free(impl_key);
    g_hash_table_unref(tbl);
    g_return_val_if_reached(FALSE);
  }

  for (size_t i = 0; i < n_states; i++)
    impl_key->state[i] = key.state[i];

  impl_key->prop = key.prop;

  gboolean result = g_hash_table_contains(tbl, impl_key);

  g_free(impl_key->state);
  g_free(impl_key->elem);
  g_free(impl_key);
  g_hash_table_unref(tbl);
  return result;
}

static gboolean ntk_styler_default_get_style_property(NtkStyler* self, NtkStylerKey key, GValue* value) {
  NtkStylerPrivate* priv = NTK_STYLER_PRIVATE(self);
  GHashTable* tbl = ntk_styler_export(self);
  g_return_val_if_fail(tbl != NULL, FALSE);

  NtkStylerKey* impl_key = g_try_malloc0(sizeof(NtkStylerKey));
  if (impl_key == NULL) {
    g_hash_table_unref(tbl);
    g_return_val_if_reached(FALSE);
  }

  size_t n_elems = ntk_styler_element_get_depth(key.elem);

  impl_key->elem = g_try_malloc(sizeof(NtkStylerElement) * n_elems);
  if (impl_key->elem == NULL) {
    g_free(impl_key);
    g_hash_table_unref(tbl);
    g_return_val_if_reached(FALSE);
  }

  for (size_t i = 0; i < n_elems; i++)
    impl_key->elem[i] = key.elem[i];

  size_t n_states = ntk_styler_state_get_depth(key.state);

  impl_key->state = g_try_malloc(sizeof(NtkStylerState) * n_states);
  if (impl_key->state == NULL) {
    g_free(impl_key->elem);
    g_free(impl_key);
    g_hash_table_unref(tbl);
    g_return_val_if_reached(FALSE);
  }

  for (size_t i = 0; i < n_states; i++)
    impl_key->state[i] = key.state[i];

  impl_key->prop = key.prop;

  const GValue* srcval = g_hash_table_lookup(tbl, impl_key);
  if (srcval != NULL) g_value_copy(srcval, value);

  g_free(impl_key->state);
  g_free(impl_key->elem);
  g_free(impl_key);
  g_hash_table_unref(tbl);
  return srcval != NULL;
}

static gboolean ntk_styler_default_set_style_property(NtkStyler* self, NtkStylerKey key, const GValue* value) {
  NtkStylerPrivate* priv = NTK_STYLER_PRIVATE(self);
  GHashTable* tbl = ntk_styler_export(self);
  g_return_val_if_fail(tbl != NULL, FALSE);

  NtkStylerKey* impl_key = g_try_malloc0(sizeof(NtkStylerKey));
  if (impl_key == NULL) {
    g_hash_table_unref(tbl);
    g_return_val_if_reached(FALSE);
  }

  size_t n_elems = ntk_styler_element_get_depth(key.elem);

  impl_key->elem = g_try_malloc(sizeof(NtkStylerElement) * n_elems);
  if (impl_key->elem == NULL) {
    g_free(impl_key);
    g_hash_table_unref(tbl);
    g_return_val_if_reached(FALSE);
  }

  for (size_t i = 0; i < n_elems; i++)
    impl_key->elem[i] = key.elem[i];

  size_t n_states = ntk_styler_state_get_depth(key.state);

  impl_key->state = g_try_malloc(sizeof(NtkStylerState) * n_states);
  if (impl_key->state == NULL) {
    g_free(impl_key->elem);
    g_free(impl_key);
    g_hash_table_unref(tbl);
    g_return_val_if_reached(FALSE);
  }

  for (size_t i = 0; i < n_states; i++)
    impl_key->state[i] = key.state[i];

  impl_key->prop = key.prop;

  GValue* impl_value = g_try_malloc0(sizeof(GValue));
  if (value == NULL) {
    g_free(impl_key->state);
    g_free(impl_key->elem);
    g_free(impl_key);
    g_hash_table_unref(tbl);
    g_return_val_if_reached(FALSE);
  }

  g_value_copy(value, impl_value);
  g_hash_table_insert(tbl, impl_key, impl_value);
  g_hash_table_unref(tbl);
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
    g_return_val_if_fail(klass->set_style_property(self, entry.key, &entry.value), FALSE);
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

  return klass->set_style_property(self, key, value);
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
