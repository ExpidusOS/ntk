#ifndef __NTK_HW_DEVICE_H_
#define __NTK_HW_DEVICE_H_ 1

#include <glib-object.h>

G_BEGIN_DECLS

#define NTK_HW_TYPE_DEVICE ntk_hw_device_get_type()
G_DECLARE_DERIVABLE_TYPE(NtkHWDevice, ntk_hw_device, NTK_HW, DEVICE, GObject);

/**
 * NtkHWDeviceKind:
 *
 * Since: 0.1.0
 */
typedef enum {
  NTK_HW_DEVICE_NONE,
  NTK_HW_DEVICE_DISPLAY,
  NTK_HW_DEVICE_INPUT
} NtkHWDeviceKind;

/**
 * NtkHWDevice:
 *
 * Since: 0.1.0
 */
struct _NtkHWDeviceClass {
  GObjectClass parent_class;

  NtkHWDeviceKind (*get_kind)(NtkHWDevice* self);
};

NtkHWDeviceKind ntk_hw_device_get_kind(NtkHWDevice* self);

G_END_DECLS

#endif
