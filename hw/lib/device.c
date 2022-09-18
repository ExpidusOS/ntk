#include <ntk/hw/device.h>

G_DEFINE_TYPE(NtkHWDevice, ntk_hw_device, G_TYPE_OBJECT);

static void ntk_hw_device_class_init(NtkHWDeviceClass* klass) {}

static void ntk_hw_device_init(NtkHWDevice* self) {}

gchar* ntk_hw_device_get_name(NtkHWDevice* self) {
  g_return_val_if_fail(NTK_HW_IS_DEVICE(self), NULL);
  NtkHWDeviceClass* klass = NTK_HW_DEVICE_GET_CLASS(self);
  g_return_val_if_fail(klass != NULL, NULL);
  g_return_val_if_fail(klass->get_name != NULL, NULL);
  return klass->get_name(self);
}
