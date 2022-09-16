#pragma once

#include <glib-object.h>

G_BEGIN_DECLS

#define NTK_HW_TYPE_DEVICE ntk_hw_device_get_type()
G_DECLARE_INTERFACE(NtkHWDevice, ntk_hw_device, NTK_HW, DEVICE, GObject);

/**
 * NtkHWDevice:
 *
 * Since: 0.1.0
 */
struct _NtkHWDeviceInterface {
  GTypeInterface parent_iface;

  gchar* (*get_name)(NtkHWDevice* self);
};

gchar* ntk_hw_device_get_name(NtkHWDevice* self);

G_END_DECLS
