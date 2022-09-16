#define G_LOG_DOMAIN "NtkHWDisplay"
#include <ntk/hw/display.h>

static void ntk_hw_display_device_interface_init(NtkHWDeviceInterface* iface);

G_DEFINE_TYPE_WITH_CODE(
  NtkHWDisplay, ntk_hw_display, G_TYPE_OBJECT,
  G_IMPLEMENT_INTERFACE(NTK_HW_TYPE_DEVICE, ntk_hw_display_device_interface_init)
);

static void ntk_hw_display_device_interface_init(NtkHWDeviceInterface* iface) {}

static void ntk_hw_display_class_init(NtkHWDisplayClass* klass) {}

static void ntk_hw_display_init(NtkHWDisplay* self) {}
