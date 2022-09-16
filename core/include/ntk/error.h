#pragma once

#include <glib.h>

/**
 * NtkError:
 *
 * Since: 0.1.0
 */
typedef enum {
  NTK_ERROR_BAD_RENDERER,
  NTK_ERROR_NUKLEAR_FAIL
} NtkError;

#define NTK_ERROR ntk_error_quark()
GQuark ntk_error_quark();
