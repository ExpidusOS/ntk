#include "error-priv.h"
#include <ntk/error.h>

typedef struct {
  int type;

  union {
    NtkRenderer* renderer;
    NtkContext* context;
  };
} NtkErrorPrivate;

static void ntk_error_private_init(NtkErrorPrivate* priv) {}

static void ntk_error_private_copy(const NtkErrorPrivate* src_priv, NtkErrorPrivate* dest_priv) {
  dest_priv->type = dest_priv->type;
  switch (src_priv->type) {
    case NTK_ERROR_BAD_RENDERER:
      dest_priv->renderer = src_priv->renderer;
      break;
    case NTK_ERROR_NUKLEAR_FAIL:
      dest_priv->context = src_priv->context;
      break;
  }
}

static void ntk_error_private_clear(NtkErrorPrivate* priv) {}

G_DEFINE_EXTENDED_ERROR(NtkError, ntk_error);

void ntk_error_set_bad_renderer(GError** error, const char* reason, NtkRenderer* renderer) {
  NtkErrorPrivate* priv;
  g_set_error(error, NTK_ERROR, NTK_ERROR_BAD_RENDERER, "The given renderer (%p) is a bad instance: %s", renderer, reason);
  if (error != NULL && *error != NULL) {
    priv = ntk_error_get_private(*error);
    g_return_if_fail(priv != NULL);
    priv->type = NTK_ERROR_BAD_RENDERER;
    priv->renderer = renderer;
  }
}

void ntk_error_set_nuklear_fail(GError** error, const char* reason, NtkContext* context) {
  NtkErrorPrivate* priv;
  g_set_error(error, NTK_ERROR, NTK_ERROR_NUKLEAR_FAIL, "Nuklear failed for the NTK context %p: %s", context, reason);
  if (error != NULL && *error != NULL) {
    priv = ntk_error_get_private(*error);
    g_return_if_fail(priv != NULL);
    priv->type = NTK_ERROR_NUKLEAR_FAIL;
    priv->context = context;
  }
}
