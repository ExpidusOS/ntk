#include "error-priv.h"
#include <ntk/backend/gles2/error.h>

typedef struct {
  int type;
} NtkGLES2ErrorPrivate;

static void ntk_gles2_error_private_init(NtkGLES2ErrorPrivate* priv) {}

static void ntk_gles2_error_private_copy(const NtkGLES2ErrorPrivate* src_priv, NtkGLES2ErrorPrivate* dest_priv) {
  dest_priv->type = src_priv->type;
}

static void ntk_gles2_error_private_clear(NtkGLES2ErrorPrivate* priv) {
}

G_DEFINE_EXTENDED_ERROR(NtkGLES2Error, ntk_gles2_error);
