#pragma once

#include "styler.h"
#include <glib-object.h>

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
gboolean ntk_styler_create_item_style(NtkStyler* self, NtkStylerElement* elems, NtkStylerState* state, struct nk_style_item* style);

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
