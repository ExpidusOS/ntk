#ifndef __NTK_BACKEND_EGL_ERROR_H_
#define __NTK_BACKEND_EGL_ERROR_H_ 1

#include <glib.h>

/**
 * NtkEGLError:
 *
 * Since: 0.1.0
 */
typedef enum {
  NTK_EGL_ERROR_BAD_PROC,
  NTK_EGL_ERROR_MISSING_EXT,
  NTK_EGL_ERROR_BINDING,
  NTK_EGL_ERROR_EGL
} NtkEGLError;

#define NTK_EGL_ERROR ntk_egl_error_quark()
GQuark ntk_egl_error_quark();

#endif