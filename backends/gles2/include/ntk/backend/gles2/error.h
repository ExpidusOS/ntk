#pragma once

#include <glib.h>

/**
 * NtkGLES2Error:
 *
 * Since: 0.1.0
 */
typedef enum {
  NTK_GLES2_ERROR_MISSING_EXT,
  NTK_GLES2_ERROR_BAD_PROC,
  NTK_GLES2_ERROR_GL
} NtkEGLError;

#define NTK_GLES2_ERROR ntk_gles2_error_quark()
GQuark ntk_gles2_error_quark();
