#pragma once

#include <glib.h>

/**
 * NtkHWError:
 *
 * Since: 0.1.0
 */
typedef enum {
  NTK_HW_ERROR_BAD_DEVICE
} NtkHWError;

#define NTK_HW_ERROR ntk_hw_error_quark()
GQuark ntk_hw_error_quark();
