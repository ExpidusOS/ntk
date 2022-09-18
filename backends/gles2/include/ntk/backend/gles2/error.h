#pragma once

#include <glib.h>

/**
 * NtkGLES2Error:
 *
 * Since: 0.1.0
 */
typedef enum {
  NTK_GLES2_ERROR_PLACEHOLDER /* TODO: replace with a real error type */
} NtkEGLError;

#define NTK_GLES2_ERROR ntk_gles2_error_quark()
GQuark ntk_gles2_error_quark();
