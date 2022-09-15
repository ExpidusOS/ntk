#define G_LOG_DOMAIN "NtkHWDevice"
#include <ntk/hw/device.h>

NtkHWDeviceKind ntk_hw_device_get_kind(NtkHWDevice* self) {
  g_return_val_if_fail(NTK_IS_HW_DEVICE(self), NTK_HW_DEVICE_NONE);

  NtkHWDeviceClass* klass = NTK_HW_DEVICE_GET_CLASS(self);
  g_return_val_if_fail(klass != NULL, NTK_HW_DEVICE_NONE);
  g_return_val_if_fail(klass->get_kind != NULL, NTK_HW_DEVICE_NONE);
  return klass->get_kind(self);
}
