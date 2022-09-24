#pragma once

#include <glib-object.h>
#include <ntk/styler.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#define NTK_CSS_TYPE_STYLER ntk_css_styler_get_type()
G_DECLARE_FINAL_TYPE(NtkCSSStyler, ntk_css_styler, NTK_CSS, STYLER, NtkStyler);

/**
 * NtkCSSStyler:
 *
 * Since: 0.1.0
 */
struct _NtkCSSStyler {
  NtkStyler parent_instance;

  /*< private >*/
  struct _NtkCSSStylerPrivate* priv;
};

NtkStyler* ntk_css_styler_new();

/**
 * ntk_css_styler_load:
 * @self: An #NtkCSSStyler
 * @str: (array length=length): The CSS string
 * @length: The length of the CSS string
 * @error: (out): The #GError which was outputted
 *
 * Since: 0.1.0
 * Returns: True if the CSS was loaded in
 */
gboolean ntk_css_styler_load(NtkCSSStyler* self, const gchar* str, size_t length, GError** error);

/**
 * ntk_css_styler_load_path:
 * @self: An #NtkCSSStyler
 * @path: The path to the file
 * @error: (out): The #GError which was outputted
 *
 * Since: 0.1.0
 * Returns: True if the CSS was loaded in
 */
gboolean ntk_css_styler_load_path(NtkCSSStyler* self, const gchar* path, GError** error);

/**
 * ntk_css_styler_load_file:
 * @self: An #NtkCSSStyler
 * @file: The GIO File
 * @error: (out): The #GError which was outputted
 *
 * Since: 0.1.0
 * Returns: True if the CSS was loaded in
 */
gboolean ntk_css_styler_load_file(NtkCSSStyler* self, GFile* file, GError** error);

/**
 * ntk_css_styler_load_resource:
 * @self: An #NtkCSSStyler
 * @path: The resource path
 * @error: (out): The #GError which was outputted
 *
 * Since: 0.1.0
 * Returns: True if the CSS was loaded in
 */
gboolean ntk_css_styler_load_resource(NtkCSSStyler* self, const gchar* path, GError** error);

G_END_DECLS
