#pragma once

#include <glib-object.h>

G_BEGIN_DECLS

/**
 * NtkCSSError:
 *
 * Since: 0.1.0
 */
typedef enum {
  NTK_CSS_ERROR_PARSER
} NtkCSSError;

#define NTK_CSS_ERROR ntk_css_error_quark()
GQuark ntk_css_error_quark();

G_END_DECLS
