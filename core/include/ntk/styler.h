#pragma once

#include "context.h"
#include <glib-object.h>

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
  NTK_STYLER_ELEMENT_BUTTON_MENU,
  NTK_STYLER_ELEMENT_BUTTON_CONTEXT,
  NTK_STYLER_ELEMENT_INPUT_TEXT,
  NTK_STYLER_ELEMENT_INPUT_TOGGLE,
  NTK_STYLER_ELEMENT_INPUT_SLIDER,
  NTK_STYLER_ELEMENT_INPUT_CHECKBOX,
  NTK_STYLER_ELEMENT_PROGRESS,
  NTK_STYLER_ELEMENT_SCROLLBAR_VERTICAL,
  NTK_STYLER_ELEMENT_SCROLLBAR_HORIZONTAL,
  NTK_STYLER_ELEMENT_TEXTAREA,
  NTK_STYLER_ELEMENT_TAB,
  NTK_STYLER_ELEMENT_OPTION,
  NTK_STYLER_ELEMENT_WINDOW,
  NTK_STYLER_ELEMENT_WINDOW_HEADER,
  NTK_STYLER_N_ELEMENTS
} NtkStylerElement;

/**
 * ntk_styler_element_extend:
 * @base: (array zero-terminated=1): A zero-terminated array of #NtkStylerElement
 * @extend: (array zero-terminated=1): A zero-terminated array of #NtkStylerElement
 *
 * Since: 0.1.0
 * Returns: (transfer full): A new array the size of both @base and @extend
 */
NtkStylerElement* ntk_styler_element_extend(NtkStylerElement* base, NtkStylerElement* extend);

/**
 * ntk_styler_element_has:
 * @elem: (array zero-terminated=1): A zero-terminated array of #NtkStylerElement
 * @wants: The #NtkStylerElement that is being looked for
 *
 * Since: 0.1.0
 * Returns: True or false if the element specified is included
 */
gboolean ntk_styler_element_has(NtkStylerElement* elem, NtkStylerElement wants);

/**
 * ntk_styler_element_get_depth:
 * @elem: (array zero-terminated=1): A zero-terminated array of #NtkStylerElement
 *
 * Computes the number of elements.
 *
 * Since: 0.1.0
 * Returns: The number of elements
 */
size_t ntk_styler_element_get_depth(NtkStylerElement* elem);

/**
 * ntk_styler_element_to_string:
 * @key: An #NtkStylerElement
 *
 * Since: 0.1.0
 * Returns: (transfer none): The string representation of the element
 */
const char* ntk_styler_element_to_string(NtkStylerElement elem);

/**
 * NtkStylerState:
 *
 * Since: 0.1.0
 */
typedef enum _NtkStylerState {
  NTK_STYLER_STATE_NONE = 0,
  NTK_STYLER_STATE_NORMAL,
  NTK_STYLER_STATE_HOVER,
  NTK_STYLER_STATE_ACTIVE,
  NTK_STYLER_STATE_PRESSED,
  NTK_STYLER_STATE_SELECTION,
  NTK_STYLER_N_STATES
} NtkStylerState;

/**
 * ntk_styler_state_has:
 * @state: (array zero-terminated=1): A zero-terminated array of #NtkStylerState
 * @wants: The #NtkStylerState that is being looked for
 *
 * Since: 0.1.0
 * Returns: True or false if the state specified is included
 */
gboolean ntk_styler_state_has(NtkStylerState* state, NtkStylerState wants);

/**
 * ntk_styler_state_get_depth:
 * @elem: (array zero-terminated=1): A zero-terminated array of #NtkStylerState
 *
 * Computes the number of elements.
 *
 * Returns: The number of elements
 */
size_t ntk_styler_state_get_depth(NtkStylerState* state);

/**
 * ntk_styler_state_to_string:
 * @key: An #NtkStylerState
 *
 * Since: 0.1.0
 * Returns: (transfer none): The string representation of the state
 */
const char* ntk_styler_state_to_string(NtkStylerState state);

/**
 * NtkStylerProperty:
 *
 * Since: 0.1.0
 */
typedef enum _NtkStylerProperty {
  NTK_STYLER_PROPERTY_UNKNOWN = 0,

  NTK_STYLER_PROPERTY_COLOR,
  NTK_STYLER_PROPERTY_BACKGROUND_COLOR,
  NTK_STYLER_PROPERTY_BACKGROUND_IMAGE,
  NTK_STYLER_PROPERTY_BORDER_COLOR,

  NTK_STYLER_PROPERTY_SPACING_TOP,
  NTK_STYLER_PROPERTY_SPACING_LEFT,
  NTK_STYLER_PROPERTY_PADDING_TOP,
  NTK_STYLER_PROPERTY_PADDING_LEFT,
  NTK_STYLER_PROPERTY_BORDER_RADIUS,
  NTK_STYLER_PROPERTY_WIDTH,
  NTK_STYLER_PROPERTY_HEIGHT,

  NTK_STYLER_PROPERTY_VISIBILITY,

  NTK_STYLER_PROPERTY_BORDER_WIDTH,
  NTK_STYLER_PROPERTY_TEXT_ALIGN,

  NTK_STYLER_N_PROPERTIES
} NtkStylerProperty;

/**
 * ntk_styler_property_to_string:
 * @key: An #NtkStylerProperty
 *
 * Since: 0.1.0
 * Returns: (transfer full): The string representation of the property
 */
const char* ntk_styler_property_to_string(NtkStylerProperty prop);

/**
 * NtkStylerKey:
 *
 * Since: 0.1.0
 */
typedef struct _NtkStylerKey {
  NtkStylerProperty prop;

  /**
   * NtkStylerKey.classes: (array zero-terminated=1): A zero-terminated array of strings
   */
  char** classes;

  /**
   * NtkStylerKey.state: (array zero-terminated=1): A zero-terminated array of #NtkStylerState
   */
  NtkStylerState* state;

  /**
   * NtkStylerKey.elem: (array zero-terminated=1): A zero-terminated array of #NtkStylerElement
   */
  NtkStylerElement* elem;
} NtkStylerKey;

/**
 * ntk_styler_key_build_element:
 * @key: An #NtkStylerKey
 * @elem: The element to append
 * @n_elems: The element size
 *
 * Since: 0.1.0
 */
void ntk_styler_key_build_element(NtkStylerKey* key, NtkStylerElement elem, size_t* n_elems);

/**
 * ntk_styler_key_build_state:
 * @key: An #NtkStylerKey
 * @elem: The state to append
 * @n_elems: The state size
 *
 * Since: 0.1.0
 */
void ntk_styler_key_build_state(NtkStylerKey* key, NtkStylerState state, size_t* n_states);

/**
 * ntk_styler_key_to_string:
 * @key: An #NtkStylerKey
 *
 * Since: 0.1.0
 * Returns: (transfer full): The string representation of the key
 */
const char* ntk_styler_key_to_string(NtkStylerKey* key);

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
 * ntk_styler_key_equal:
 * @item: The key we should check
 * @sm: The key which we want to reference for the match.
 *
 * Checks if the @item key roughtly matches what we want in @sm.
 *
 * Since: 0.1.0
 * Returns: True if @item is the best match for @sm
 */
gboolean ntk_styler_key_best_match(NtkStylerKey* item, NtkStylerKey* sm);

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

/**
 * ntk_styler_create_item_style:
 * @self: An #NtkStyler
 * @elems: (array zero-terminated=1): A zero-terminated array of #NtkStylerElement
 * @state: (array zero-terminated=1): A zero-terminated array of #NtkStylerState
 * @style: (out): The item style for Nuklear
 *
 * Since: 0.1.0
 * Returns: True if the style was created
 */
gboolean
ntk_styler_create_item_style(NtkStyler* self, NtkStylerElement* elems, NtkStylerState* state, struct nk_style_item* style);

/**
 * ntk_styler_create_item_style_for_key:
 * @self: An #NtkStyler
 * @key: The styler key
 * @style: (out): The item style for Nuklear
 *
 * Since: 0.1.0
 * Returns: True if the style was created
 */
gboolean
ntk_styler_create_item_style_for_key(NtkStyler* self, NtkStylerKey key, struct nk_style_item* style);

/**
 * ntk_styler_create_text_style:
 * @self: An #NtkStyler
 * @elems: (array zero-terminated=1): A zero-terminated array of #NtkStylerElement
 * @style: (out): The text style for Nuklear
 *
 * Since: 0.1.0
 * Returns: True if the style was created
 */
gboolean ntk_styler_create_text_style(NtkStyler* self, NtkStylerElement* elems, struct nk_style_text* style);

/**
 * ntk_styler_create_button_style:
 * @self: An #NtkStyler
 * @elems: (array zero-terminated=1): A zero-terminated array of #NtkStylerElement
 * @style: (out): The button style for Nuklear
 *
 * Since: 0.1.0
 * Returns: True if the style was created
 */
gboolean ntk_styler_create_button_style(NtkStyler* self, NtkStylerElement* elems, struct nk_style_button* style);

/**
 * ntk_styler_create_button_style_for_key:
 * @self: An #NtkStyler
 * @key: The style key to use
 * @style: (out): The button style for Nuklear
 *
 * Since: 0.1.0
 * Returns: True if the style was created
 */
gboolean ntk_styler_create_button_style_for_key(NtkStyler* self, NtkStylerKey key, struct nk_style_button* style);

/**
 * ntk_styler_create_toggle_style:
 * @self: An #NtkStyler
 * @elems: (array zero-terminated=1): A zero-terminated array of #NtkStylerElement
 * @style: (out): The toggle style for Nuklear
 *
 * Since: 0.1.0
 * Returns: True if the style was created
 */
gboolean ntk_styler_create_toggle_style(NtkStyler* self, NtkStylerElement* elems, struct nk_style_toggle* style);

/**
 * ntk_styler_create_selectable_style:
 * @self: An #NtkStyler
 * @elems: (array zero-terminated=1): A zero-terminated array of #NtkStylerElement
 * @style: (out): The selectable style for Nuklear
 *
 * Since: 0.1.0
 * Returns: True if the style was created
 */
gboolean ntk_styler_create_selectable_style(NtkStyler* self, NtkStylerElement* elems, struct nk_style_selectable* style);

/**
 * ntk_styler_create_slider_style:
 * @self: An #NtkStyler
 * @elems: (array zero-terminated=1): A zero-terminated array of #NtkStylerElement
 * @style: (out): The slider style for Nuklear
 *
 * Since: 0.1.0
 * Returns: True if the style was created
 */
gboolean ntk_styler_create_slider_style(NtkStyler* self, NtkStylerElement* elems, struct nk_style_slider* style);

/**
 * ntk_styler_create_progress_style:
 * @self: An #NtkStyler
 * @elems: (array zero-terminated=1): A zero-terminated array of #NtkStylerElement
 * @style: (out): The progress style for Nuklear
 *
 * Since: 0.1.0
 * Returns: True if the style was created
 */
gboolean ntk_styler_create_progress_style(NtkStyler* self, NtkStylerElement* elems, struct nk_style_progress* style);

/**
 * ntk_styler_create_scrollbar_style:
 * @self: An #NtkStyler
 * @elems: (array zero-terminated=1): A zero-terminated array of #NtkStylerElement
 * @style: (out): The scrollbar style for Nuklear
 *
 * Since: 0.1.0
 * Returns: True if the style was created
 */
gboolean ntk_styler_create_srollbar_style(NtkStyler* self, NtkStylerElement* elems, struct nk_style_scrollbar* style);

G_END_DECLS
