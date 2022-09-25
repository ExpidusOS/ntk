#include "context-priv.h"
#include "styler-priv.h"
#include <math.h>
#include <ntk/color.h>
#include <ntk/styler.h>

#define NTK_STYLER_PRIVATE(self) ntk_styler_get_instance_private(self)

G_DEFINE_TYPE_WITH_PRIVATE(NtkStyler, ntk_styler, G_TYPE_OBJECT);

NtkStylerElement* ntk_styler_element_extend(NtkStylerElement* base, NtkStylerElement* extend) {
  size_t base_size = ntk_styler_element_get_depth(base);
  size_t extend_size = ntk_styler_element_get_depth(extend);

  NtkStylerElement* value = g_try_malloc0(sizeof(NtkStylerElement) * (base_size + extend_size + 1));
  if (value == NULL) return NULL;

  for (size_t i = 0; i < base_size; i++)
    value[i] = base[i];
  for (size_t i = 0; i < extend_size; i++)
    value[i + base_size] = extend[i];
  value[base_size + extend_size] = NTK_STYLER_ELEMENT_NONE;

  g_assert_cmpint(ntk_styler_element_get_depth(value), ==, base_size + extend_size);
  return value;
}

gboolean ntk_styler_element_has(NtkStylerElement* elem, NtkStylerElement wants) {
  size_t n_elems = ntk_styler_element_get_depth(elem);
  for (size_t i = 0; i < n_elems; i++) {
    if (elem[i] == wants) return TRUE;
  }
  return FALSE;
}

size_t ntk_styler_element_get_depth(NtkStylerElement* elem) {
  if (elem == NULL) return 0;

  size_t i = 0;
  while (elem[i] > 0) {
    g_assert(elem[i] < NTK_STYLER_N_ELEMENTS);
    i++;
  }
  return i;
}

const char* ntk_styler_element_to_string(NtkStylerElement elem) {
  switch (elem) {
    case NTK_STYLER_ELEMENT_NONE:
      return "None";
    case NTK_STYLER_ELEMENT_TEXT:
      return "Text";
    case NTK_STYLER_ELEMENT_BUTTON:
      return "Button";
    case NTK_STYLER_ELEMENT_BUTTON_MENU:
      return "Menu Button";
    case NTK_STYLER_ELEMENT_BUTTON_CONTEXT:
      return "Context Menu Button";
    case NTK_STYLER_ELEMENT_INPUT_TEXT:
      return "Input Text";
    case NTK_STYLER_ELEMENT_INPUT_TOGGLE:
      return "Input Toggle";
    case NTK_STYLER_ELEMENT_INPUT_SLIDER:
      return "Input Slider";
    case NTK_STYLER_ELEMENT_INPUT_CHECKBOX:
      return "Input Checkbox";
    case NTK_STYLER_ELEMENT_PROGRESS:
      return "Progress";
    case NTK_STYLER_ELEMENT_SCROLLBAR_VERTICAL:
      return "Vertical Scrollbar";
    case NTK_STYLER_ELEMENT_SCROLLBAR_HORIZONTAL:
      return "Horizontal Scrollbar";
    case NTK_STYLER_ELEMENT_TEXTAREA:
      return "Text Area";
    case NTK_STYLER_ELEMENT_TAB:
      return "Tab";
    case NTK_STYLER_ELEMENT_OPTION:
      return "Option";
    case NTK_STYLER_ELEMENT_WINDOW:
      return "Window";
    case NTK_STYLER_ELEMENT_WINDOW_HEADER:
      return "Window Header";
    case NTK_STYLER_N_ELEMENTS:
    default:
      return NULL;
  }
}

size_t ntk_styler_state_get_depth(NtkStylerState* state) {
  if (state == NULL) return 0;

  size_t i = 0;
  while (state[i] > 0) {
    g_assert(state[i] < NTK_STYLER_N_STATES);
    i++;
  }
  return i;
}

const char* ntk_styler_state_to_string(NtkStylerState state) {
  switch (state) {
    case NTK_STYLER_STATE_NONE:
      return "None";
    case NTK_STYLER_STATE_NORMAL:
      return "Normal";
    case NTK_STYLER_STATE_HOVER:
      return "Hover";
    case NTK_STYLER_STATE_ACTIVE:
      return "Active";
    case NTK_STYLER_STATE_PRESSED:
      return "Pressed";
    case NTK_STYLER_STATE_SELECTION:
      return "Selection";
    case NTK_STYLER_N_STATES:
    default:
      return NULL;
  }
}

const char* ntk_styler_property_to_string(NtkStylerProperty prop) {
  switch (prop) {
    case NTK_STYLER_PROPERTY_UNKNOWN:
      return "Unknown";
    case NTK_STYLER_PROPERTY_COLOR:
      return "Color";
    case NTK_STYLER_PROPERTY_BACKGROUND_COLOR:
      return "Background Color";
    case NTK_STYLER_PROPERTY_BACKGROUND_IMAGE:
      return "Background Image";
    case NTK_STYLER_PROPERTY_BORDER_COLOR:
      return "Border Color";
    case NTK_STYLER_PROPERTY_SPACING_TOP:
      return "Spacing Top";
    case NTK_STYLER_PROPERTY_SPACING_LEFT:
      return "Spacing Left";
    case NTK_STYLER_PROPERTY_PADDING_TOP:
      return "Padding Top";
    case NTK_STYLER_PROPERTY_PADDING_LEFT:
      return "Padding Left";
    case NTK_STYLER_PROPERTY_BORDER_RADIUS:
      return "Border Radius";
    case NTK_STYLER_PROPERTY_VISIBILITY:
      return "Visibility";
    case NTK_STYLER_PROPERTY_BORDER_WIDTH:
      return "Border Width";
    case NTK_STYLER_PROPERTY_TEXT_ALIGN:
      return "Text Align";
    case NTK_STYLER_N_PROPERTIES:
    default:
      return NULL;
  }
}

void ntk_styler_key_build_element(NtkStylerKey* key, NtkStylerElement elem, size_t* n_elems) {
  if (key != NULL && key->elem != NULL) key->elem[*n_elems] = elem;
  *n_elems = (*n_elems) + 1;
}

void ntk_styler_key_build_state(NtkStylerKey* key, NtkStylerState state, size_t* n_states) {
  if (key != NULL && key->state != NULL) key->state[*n_states] = state;
  *n_states = (*n_states) + 1;
}

const char* ntk_styler_key_to_string(NtkStylerKey* key) {
  GString* str = g_string_new(NULL);
  g_return_val_if_fail(str != NULL, NULL);

  size_t n_elems = ntk_styler_element_get_depth(key->elem);
  g_string_append_printf(str, "Elements: %ld", n_elems);

  if (n_elems > 0) g_string_append(str, " (");
  for (size_t i = 0; i < n_elems; i++) {
    g_string_append_printf(str, "%s:%d", ntk_styler_element_to_string(key->elem[i]), key->elem[i]);
    if ((i + 1) < n_elems) g_string_append(str, ", ");
  }
  if (n_elems > 0) g_string_append_c(str, ')');

  size_t n_states = ntk_styler_state_get_depth(key->state);
  g_string_append_printf(str, ", State: %ld", n_states);

  if (n_states > 0) g_string_append(str, " (");
  for (size_t i = 0; i < n_states; i++) {
    g_string_append_printf(str, "%s:%d", ntk_styler_state_to_string(key->state[i]), key->state[i]);
    if ((i + 1) < n_states) g_string_append(str, ", ");
  }
  if (n_states > 0) g_string_append_c(str, ')');

  size_t n_classes = key->classes == NULL ? 0 : g_strv_length(key->classes);
  g_string_append_printf(
    str, ", Property: %s:%d, Class Names: %ld", ntk_styler_property_to_string(key->prop), key->prop,
    n_classes
  );

  if (n_classes > 0) {
    g_string_append(str, " (");
    for (size_t i = 0; i < n_classes; i++) {
      g_string_append_printf(str, "%s", key->classes[i]);
      if ((i + 1) < n_classes) g_string_append(str, ", ");
    }
    g_string_append_c(str, ')');
  }
  return g_string_free(str, FALSE);
}

guint ntk_styler_key_hash(NtkStylerKey* key) {
  g_return_val_if_fail(key != NULL, -1);

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

  size_t n_classes = key->classes == NULL ? 0 : g_strv_length(key->classes);
  guint classes = 0;
  for (size_t i = 0; i < n_classes; i++) classes += g_str_hash(key->classes[i]);
  return (elem << elem_digits) + (state << state_digits) + (key->prop << n_prop_digits) + classes;
}

gboolean ntk_styler_key_equal(NtkStylerKey* a, NtkStylerKey* b) {
  size_t a_elem_count = ntk_styler_element_get_depth(a->elem);
  size_t b_elem_count = ntk_styler_element_get_depth(b->elem);
  if (a_elem_count != b_elem_count) return FALSE;

  size_t a_state_count = ntk_styler_state_get_depth(a->state);
  size_t b_state_count = ntk_styler_state_get_depth(b->state);
  if (a_state_count != b_state_count) return FALSE;

  size_t a_class_count = a->classes == NULL ? 0 : g_strv_length(a->classes);
  size_t b_class_count = b->classes == NULL ? 0 : g_strv_length(b->classes);
  if (a_class_count != b_class_count) return FALSE;

  for (size_t i = 0; i < a_elem_count; i++) {
    if (a->elem[i] != b->elem[i]) return FALSE;
  }

  for (size_t i = 0; i < a_state_count; i++) {
    if (a->state[i] != b->state[i]) return FALSE;
  }

  for (size_t i = 0; i < a_class_count; i++) {
    if (!g_str_equal(a->classes[i], b->classes[i])) return FALSE;
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
  if (key->classes != NULL) {
    size_t i = 0;
    while (key->classes[i] != NULL) g_clear_pointer(&key->classes[i++], g_free);
  }

  g_clear_pointer(&key->classes, g_free);
  g_clear_pointer(&key->state, g_free);
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
  impl_key->classes = key.classes == NULL ? NULL : g_try_malloc0(sizeof (char*) * g_strv_length(key.classes));
  if (impl_key->classes != NULL) {
    for (size_t i = 0; i < g_strv_length(key.classes); i++) impl_key->classes[i] = g_strdup(key.classes[i]);
  }

  gboolean result = g_hash_table_contains(tbl, impl_key);

  if (impl_key->classes != NULL) {
    for (size_t i = 0; i < g_strv_length(impl_key->classes); i++) g_clear_pointer(&impl_key->classes[i], g_free);
  }

  g_clear_pointer(&impl_key->classes, g_free);
  g_clear_pointer(&impl_key->state, g_free);
  g_clear_pointer(&impl_key->elem, g_free);
  g_free(impl_key);
  g_hash_table_unref(tbl);
  return result;
}

static gboolean ntk_styler_default_get_style_property(NtkStyler* self, NtkStylerKey key, GValue* value) {
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
  impl_key->classes = key.classes == NULL ? NULL : g_try_malloc0(sizeof (char*) * g_strv_length(key.classes));
  if (impl_key->classes != NULL) {
    for (size_t i = 0; i < g_strv_length(key.classes); i++) impl_key->classes[i] = g_strdup(key.classes[i]);
  }

  const GValue* srcval = g_hash_table_lookup(tbl, impl_key);
  if (srcval != NULL) g_value_copy(srcval, value);

  if (impl_key->classes != NULL) {
    for (size_t i = 0; i < g_strv_length(impl_key->classes); i++) g_clear_pointer(&impl_key->classes[i], g_free);
  }

  g_clear_pointer(&impl_key->classes, g_free);
  g_clear_pointer(&impl_key->state, g_free);
  g_clear_pointer(&impl_key->elem, g_free);
  g_free(impl_key);
  g_hash_table_unref(tbl);
  return srcval != NULL;
}

static gboolean ntk_styler_default_set_style_property(NtkStyler* self, NtkStylerKey key, const GValue* value) {
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
  impl_key->classes = key.classes == NULL ? NULL : g_try_malloc0(sizeof (char*) * g_strv_length(key.classes));
  if (impl_key->classes != NULL) {
    for (size_t i = 0; i < g_strv_length(key.classes); i++) impl_key->classes[i] = g_strdup(key.classes[i]);
  }

  GValue* impl_value = g_try_malloc0(sizeof(GValue));
  if (value == NULL) {
    if (impl_key->classes != NULL) {
      for (size_t i = 0; i < g_strv_length(impl_key->classes); i++) g_clear_pointer(&impl_key->classes[i], g_free);
    }

    g_clear_pointer(&impl_key->classes, g_free);
    g_clear_pointer(&impl_key->state, g_free);
    g_clear_pointer(&impl_key->elem, g_free);
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

  g_return_val_if_fail(ntk_styler_create_text_style(self, NULL, &styles.text), FALSE);
  g_return_val_if_fail(
    ntk_styler_create_button_style(
      self, (NtkStylerElement[]){NTK_STYLER_ELEMENT_BUTTON, NTK_STYLER_ELEMENT_NONE}, &styles.button
    ),
    FALSE
  );
  g_return_val_if_fail(
    ntk_styler_create_button_style(
      self, (NtkStylerElement[]){NTK_STYLER_ELEMENT_BUTTON_CONTEXT, NTK_STYLER_ELEMENT_NONE}, &styles.contextual_button
    ),
    FALSE
  );
  g_return_val_if_fail(
    ntk_styler_create_button_style(
      self, (NtkStylerElement[]){NTK_STYLER_ELEMENT_BUTTON_MENU, NTK_STYLER_ELEMENT_NONE}, &styles.menu_button
    ),
    FALSE
  );
  g_return_val_if_fail(
    ntk_styler_create_toggle_style(
      self, (NtkStylerElement[]){NTK_STYLER_ELEMENT_OPTION, NTK_STYLER_ELEMENT_NONE}, &styles.option
    ),
    FALSE
  );
  g_return_val_if_fail(
    ntk_styler_create_toggle_style(
      self, (NtkStylerElement[]){NTK_STYLER_ELEMENT_INPUT_CHECKBOX, NTK_STYLER_ELEMENT_NONE}, &styles.checkbox
    ),
    FALSE
  );
  g_return_val_if_fail(ntk_styler_create_selectable_style(self, NULL, &styles.selectable), FALSE);
  return ntk_styler_apply_internal(self, ctx, styles);
}

void ntk_styler_restore(NtkStyler* self, NtkContext* ctx) {
  g_return_if_fail(NTK_IS_STYLER(self));
  g_return_if_fail(NTK_IS_CONTEXT(self));
  NtkStylerPrivate* priv = NTK_STYLER_PRIVATE(self);

  g_return_if_fail(ntk_styler_apply_internal(self, ctx, priv->saved_style));
}
