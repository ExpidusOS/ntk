#include "error-priv.h"
#include <ntk/backend/gles2/error.h>

typedef struct {
  int type;
  union {
    char* name;
    GLint gl_error;
  };
} NtkGLES2ErrorPrivate;

static void ntk_gles2_error_private_init(NtkGLES2ErrorPrivate* priv) {}

static void ntk_gles2_error_private_copy(const NtkGLES2ErrorPrivate* src_priv, NtkGLES2ErrorPrivate* dest_priv) {
  dest_priv->type = src_priv->type;
  switch (src_priv->type) {
    case NTK_GLES2_ERROR_MISSING_EXT:
    case NTK_GLES2_ERROR_BAD_PROC:
      dest_priv->name = g_strdup(src_priv->name);
      break;
    case NTK_GLES2_ERROR_GL:
      dest_priv->gl_error = src_priv->gl_error;
      break;
  }
}

static void ntk_gles2_error_private_clear(NtkGLES2ErrorPrivate* priv) {
  switch (priv->type) {
    case NTK_GLES2_ERROR_MISSING_EXT:
    case NTK_GLES2_ERROR_BAD_PROC:
      g_free(priv->name);
      break;
  }
}

G_DEFINE_EXTENDED_ERROR(NtkGLES2Error, ntk_gles2_error);

void ntk_gles2_error_set_missing_ext(GError** error, const char* reason, const char* name) {
  NtkGLES2ErrorPrivate* priv;
  g_set_error(
    error, NTK_GLES2_ERROR, NTK_GLES2_ERROR_MISSING_EXT, "Failed to find the \"%s\" extension for GL: %s", name, reason
  );
  if (error != NULL && *error != NULL) {
    priv = ntk_gles2_error_get_private(*error);
    g_return_if_fail(priv != NULL);
    priv->type = NTK_GLES2_ERROR_MISSING_EXT;
    priv->name = g_strdup(name);
  }
}

void ntk_gles2_error_set_bad_proc(GError** error, const char* reason, const char* name) {
  NtkGLES2ErrorPrivate* priv;
  g_set_error(
    error, NTK_GLES2_ERROR, NTK_GLES2_ERROR_BAD_PROC, "Failed to get the proc address for GLES2 provided function \"%s\": %s",
    name, reason
  );
  if (error != NULL && *error != NULL) {
    priv = ntk_gles2_error_get_private(*error);
    g_return_if_fail(priv != NULL);
    priv->type = NTK_GLES2_ERROR_BAD_PROC;
    priv->name = g_strdup(name);
  }
}

void ntk_gles2_error_set_gl(GError** error, const char* reason, GLint e) {
  NtkGLES2ErrorPrivate* priv;
  g_set_error(error, NTK_GLES2_ERROR, NTK_GLES2_ERROR_GL, "GL function failed (%d): %s", e, reason);
  if (error != NULL && *error != NULL) {
    priv = ntk_gles2_error_get_private(*error);
    g_return_if_fail(priv != NULL);
    priv->type = NTK_GLES2_ERROR_GL;
    priv->gl_error = e;
  }
}
