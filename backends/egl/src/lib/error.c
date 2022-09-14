#include <ntk/backend/egl/error.h>
#include "error-priv.h"

typedef union {
  char* proc_name;
} NtkEGLErrorPrivate;

static void ntk_egl_error_private_init(NtkEGLErrorPrivate* priv) {}

static void ntk_egl_error_private_copy(const NtkEGLErrorPrivate* src_priv, NtkEGLErrorPrivate* dest_priv) {
  if (src_priv->proc_name != NULL) dest_priv->proc_name = g_strdup(src_priv->proc_name);
}

static void ntk_egl_error_private_clear(NtkEGLErrorPrivate* priv) {
  g_clear_pointer(&priv->proc_name, g_free);
}

G_DEFINE_EXTENDED_ERROR(NtkEGLError, ntk_egl_error);

void ntk_egl_error_set_bad_proc(GError** error, const char* reason, const char* name) {
  NtkEGLErrorPrivate* priv;
  g_set_error(error, NTK_EGL_ERROR, NTK_EGL_ERROR_BAD_PROC, "Failed to get the proc address for EGL provided function \"%s\": %s", name, reason);
  if (error != NULL && *error != NULL) {
    priv = ntk_egl_error_get_private(*error);
    g_return_if_fail(priv != NULL);
    priv->proc_name = g_strdup(name);
  }
}
