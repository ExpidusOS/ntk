#include <ntk/hw/device.h>

G_DEFINE_INTERFACE(NtkHWDevice, ntk_hw_device, G_TYPE_OBJECT);

static void ntk_hw_device_default_init(NtkHWDeviceInterface* iface) {}
