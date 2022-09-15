#define G_LOG_DOMAIN "NtkHWInput"
#include <ntk/hw/input.h>

static void ntk_hw_input_device_interface_init(NtkHWDeviceInterface* iface);

G_DEFINE_TYPE_WITH_CODE(NtkHWInput, ntk_hw_input, G_TYPE_OBJECT,
  G_IMPLEMENT_INTERFACE(NTK_HW_TYPE_DEVICE, ntk_hw_input_device_interface_init));

static void ntk_hw_input_device_interface_init(NtkHWDeviceInterface* iface) {}

static void ntk_hw_input_class_init(NtkHWInputClass* klass) {}

static void ntk_hw_input_init(NtkHWInput* self) {}
