#include "error-priv.h"
#include <ntk/hw/error.h>

typedef struct {
  int type;

  union {
    NtkHWDevice* device;
  };
} NtkHWErrorPrivate;

static void ntk_hw_error_private_init(NtkHWErrorPrivate* priv) {}

static void ntk_hw_error_private_copy(const NtkHWErrorPrivate* src_priv, NtkHWErrorPrivate* dest_priv) {
  dest_priv->type = src_priv->type;
  switch (src_priv->type) {
    case NTK_HW_ERROR_BAD_DEVICE:
      dest_priv->device = src_priv->device;
      break;
  }
}

static void ntk_hw_error_private_clear(NtkHWErrorPrivate* priv) {}

G_DEFINE_EXTENDED_ERROR(NtkHWError, ntk_hw_error);

void ntk_hw_error_set_bad_device(GError** error, const char* reason, NtkHWDevice* device) {
  NtkHWErrorPrivate* priv;
  g_set_error(error, NTK_HW_ERROR, NTK_HW_ERROR_BAD_DEVICE, "Failed to access the device (%p): %s", device, reason);
  if (error != NULL && *error != NULL) {
    priv = ntk_hw_error_get_private(*error);
    g_return_if_fail(priv != NULL);
    priv->device = device;
  }
}
