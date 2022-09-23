#include <ntk/styling/css/error.h>
#include "error-priv.h"

typedef struct {
  CssError error;
} NtkCSSErrorPrivate;

static void ntk_css_error_private_init(NtkCSSErrorPrivate* priv) {}

static void ntk_css_error_private_copy(const NtkCSSErrorPrivate* src_priv, NtkCSSErrorPrivate* dest_priv) {
  dest_priv->error.type = src_priv->error.type;
  dest_priv->error.first_line = src_priv->error.first_line;
  dest_priv->error.first_column = src_priv->error.first_column;
  dest_priv->error.last_line = src_priv->error.last_line;
  dest_priv->error.last_column = src_priv->error.last_column;
  memcpy(dest_priv->error.message, src_priv->error.message, CSS_ERROR_MSG_SIZE);
}

static void ntk_css_error_private_clear(NtkCSSErrorPrivate* priv) {}

G_DEFINE_EXTENDED_ERROR(NtkCSSError, ntk_css_error);

void ntk_css_error_set(GError** error, CssError* cerror) {
  NtkCSSErrorPrivate* priv;
  g_set_error(error, NTK_CSS_ERROR, NTK_CSS_ERROR_PARSER, "CSS parsing failed on (%d:%d) - (%d:%d): %s", cerror->first_line, cerror->first_column, cerror->last_line, cerror->last_column, cerror->message);
  if (error != NULL && *error != NULL) {
    priv = ntk_css_error_get_private(*error);
    g_return_if_fail(priv != NULL);
    priv->error.type = cerror->type;
    priv->error.first_line = cerror->first_line;
    priv->error.first_column = cerror->first_column;
    priv->error.last_line = cerror->last_line;
    priv->error.last_column = cerror->last_column;
    memcpy(priv->error.message, cerror->message, CSS_ERROR_MSG_SIZE);
  }
}
