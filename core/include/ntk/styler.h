#pragma once

#include <glib-object.h>
#include "context.h"

G_BEGIN_DECLS

#define NTK_TYPE_STYLER ntk_styler_get_type()
G_DECLARE_DERIVABLE_TYPE(NtkStyler, ntk_styler, NTK, STYLER, GObject);

/**
 * NtkStylerElement:
 *
 * Since: 0.1.0
 */
typedef enum _NtkStylerElement {
  NTK_STYLER_ELEMENT_NONE = 0,
  NTK_STYLER_ELEMENT_TEXT,
  NTK_STYLER_ELEMENT_BUTTON,
  NTK_STYLER_ELEMENT_CONTEXT_BUTTON,
  NTK_STYLER_N_ELEMENTS
} NtkStylerElement;

/**
 * ntk_styler_element_get_depth:
 * @elem: (array zero-terminated=1): A zero-terminated array of #NtkStylerElement
 *
 * Computes the number of elements.
 *
 * Returns: The number of elements
 */
size_t ntk_styler_element_get_depth(NtkStylerElement* elem);

/**
 * NtkStylerState:
 *
 * Since: 0.1.0
 */
typedef enum _NtkStylerState {
  NTK_STYLER_STATE_NORMAL = 0,
  NTK_STYLER_STATE_HOVER,
  NTK_STYLER_STATE_ACTIVE,
  NTK_STYLER_STATE_SELECTION,
  NTK_STYLER_N_STATES
} NtkStylerState;

/**
 * NtkStylerProperty:
 *
 * Since: 0.1.0
 */
typedef enum _NtkStylerProperty {
  NTK_STYLER_PROPERTY_UNKNOWN = 0,

  NTK_STYLER_PROPERTY_COLOR,
  NTK_STYLER_PROPERTY_BACKGROUND_COLOR,
  NTK_STYLER_PROPERTY_BORDER_COLOR,

  NTK_STYLER_PROPERTY_PADDING,
  NTK_STYLER_PROPERTY_BORDER_RADIUS,

  NTK_STYLER_PROPERTY_VISIBILITY,

  NTK_STYLER_PROPERTY_BORDER_WIDTH,

  NTK_STYLER_N_PROPERTIES
} NtkStylerProperty;

/**
 * NtkStylerKey:
 *
 * Since: 0.1.0
 */
typedef struct _NtkStylerKey {
  NtkStylerProperty prop;
  NtkStylerState state;

  /**
   * NtkStylerKey.elem: (array zero-terminated=1): A zero-terminated array of #NtkStylerElement
   */
  NtkStylerElement* elem;
} NtkStylerKey;

/**
 * ntk_styler_key_hash:
 * @key: The key to compute the hash of
 *
 * Since: 0.1.0
 * Returns: The hash of the style key
 */
guint ntk_styler_key_hash(NtkStylerKey* key);

/**
 * ntk_styler_key_equal:
 * @a: The first key
 * @b: The second key
 *
 * Compares @a and @b and determin whether or not they are the same.
 *
 * Since: 0.1.0
 * Returns: True if the keys match
 */
gboolean ntk_styler_key_equal(NtkStylerKey* a, NtkStylerKey* b);

/**
 * NtkStylerEntry:
 *
 * Since: 0.1.0
 */
typedef struct _NtkStylerEntry {
  NtkStylerKey key;
  GValue value;
} NtkStylerEntry;

/**
 * NtkStyler:
 *
 * Since: 0.1.0
 */
struct _NtkStylerClass {
  GObjectClass parent_class;

  GHashTable* (*export)(NtkStyler* self);
  gboolean (*has_style_property)(NtkStyler* self, NtkStylerKey key);
  gboolean (*get_style_property)(NtkStyler* self, NtkStylerKey key, GValue* value);
  gboolean (*set_style_property)(NtkStyler* self, NtkStylerKey key, const GValue* value);
};

NtkStyler* ntk_styler_new();

/**
 * ntk_styler_import:
 * @self: An #NtkStyler
 * @styles: (array length=n_styles): List of styles
 * @n_styles: Length of @styles
 *
 * Since: 0.1.0
 * Returns: Returns true if the styles could be imported, false if it failed.
 */
gboolean ntk_styler_import(NtkStyler* self, NtkStylerEntry* styles, size_t n_styles);

/**
 * ntk_styler_export:
 * @self: An #NtkStyler
 *
 * Generates a new #GHashTable of #GValue set to every style property value.
 *
 * Since: 0.1.0
 * Returns: (element-type NtkStylerKey GValue) (transfer full): A #GHashTable
 */
GHashTable* ntk_styler_export(NtkStyler* self);

/**
 * ntk_styler_has_style_property:
 * @self: An #NtkStyler
 * @key: The key of the property
 *
 * Since: 0.1.0
 * Returns: True if the property exists.
 */
gboolean ntk_styler_has_style_property(NtkStyler* self, NtkStylerKey key);

/**
 * ntk_styler_get_style_property:
 * @self: An #NtkStyler
 * @key: The key of the property
 * @value: (out) (transfer full): The value of the property
 *
 * Since: 0.1.0
 * Returns: True if the property exists.
 */
gboolean ntk_styler_get_style_property(NtkStyler* self, NtkStylerKey key, GValue* value);

/**
 * ntk_styler_set_style_property:
 * @self: An #NtkStyler
 * @key: The key of the property
 * @value: The value of the property
 *
 * Since: 0.1.0
 * Returns: True if the property was set.
 */
gboolean ntk_styler_set_style_property(NtkStyler* self, NtkStylerKey key, const GValue* value);

/**
 * ntk_styler_apply:
 * @self: An #NtkStyler
 * @ctx: An #NtkContext
 *
 * Saves the context's style internally and applies the styler's styles to the context.
 * The context's style can be reset back to its previous styles using ntk_styler_restore.
 * Only caveat is only one style can be saved.
 *
 * Since: 0.1.0
 */
gboolean ntk_styler_apply(NtkStyler* self, NtkContext* ctx);

/**
 * ntk_styler_restore:
 * @self: An #NtkStyler
 * @ctx: An #NtkContext
 *
 * Restores the styles which were saved by ntk_styler_apply to the given context.
 * This can be used only after ntk_styler_apply is executed.
 *
 * Since: 0.1.0
 */
void ntk_styler_restore(NtkStyler* self, NtkContext* ctx);

G_END_DECLS
