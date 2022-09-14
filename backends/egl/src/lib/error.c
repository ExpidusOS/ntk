#include <ntk/backend/egl/error.h>
#include "error-priv.h"

typedef union {
  char* name;
  EGLint egl_error;
} NtkEGLErrorPrivate;

static void ntk_egl_error_private_init(NtkEGLErrorPrivate* priv) {}

static void ntk_egl_error_private_copy(const NtkEGLErrorPrivate* src_priv, NtkEGLErrorPrivate* dest_priv) {
  if (src_priv->name != NULL) dest_priv->name = g_strdup(src_priv->name);
  dest_priv->egl_error = src_priv->egl_error;
}

static void ntk_egl_error_private_clear(NtkEGLErrorPrivate* priv) {
  g_clear_pointer(&priv->name, g_free);
}

G_DEFINE_EXTENDED_ERROR(NtkEGLError, ntk_egl_error);

void ntk_egl_error_set_bad_proc(GError** error, const char* reason, const char* name) {
  NtkEGLErrorPrivate* priv;
  g_set_error(error, NTK_EGL_ERROR, NTK_EGL_ERROR_BAD_PROC, "Failed to get the proc address for EGL provided function \"%s\": %s", name, reason);
  if (error != NULL && *error != NULL) {
    priv = ntk_egl_error_get_private(*error);
    g_return_if_fail(priv != NULL);
    priv->name = g_strdup(name);
  }
}

void ntk_egl_error_set_missing_ext(GError** error, const char* reason, const char* name) {
  NtkEGLErrorPrivate* priv;
  g_set_error(error, NTK_EGL_ERROR, NTK_EGL_ERROR_MISSING_EXT, "Failed to find the \"%s\" extension for EGL: %s", name, reason);
  if (error != NULL && *error != NULL) {
    priv = ntk_egl_error_get_private(*error);
    g_return_if_fail(priv != NULL);
    priv->name = g_strdup(name);
  }
}

void ntk_egl_error_set_binding(GError** error, const char* reason) {
  NtkEGLErrorPrivate* priv;
  g_set_error(error, NTK_EGL_ERROR, NTK_EGL_ERROR_BINDING, "Failed to bind EGL: %s", reason);
}

void ntk_egl_error_set_egl(GError** error, const char* reason, EGLint e) {
  NtkEGLErrorPrivate* priv;
  g_set_error(error, NTK_EGL_ERROR, NTK_EGL_ERROR_EGL, "EGL function failed (%d): %s", e, reason);
  if (error != NULL && *error != NULL) {
    priv = ntk_egl_error_get_private(*error);
    g_return_if_fail(priv != NULL);
    priv->egl_error = e;
  }
}
