#define G_LOG_DOMAIN "NtkHWDrmDevice"
#include <ntk/hw/device/drm.h>
#include <gio/gio.h>
#include "drm-priv.h"

#define NTK_HW_DRM_DEVICE_PRIVATE(self) ((self)->priv == NULL ? ntk_hw_drm_device_get_instance_private(self) : (self)->priv)

G_DEFINE_TYPE_WITH_PRIVATE(NtkHWDrmDevice, ntk_hw_drm_device, NTK_HW_TYPE_DISPLAY);

static void ntk_hw_drm_device_class_init(NtkHWDrmDeviceClass* klass) {
  NtkHWDeviceClass* device_class = NTK_HW_DEVICE_CLASS(klass);
}

static void ntk_hw_drm_device_init(NtkHWDrmDevice* self) {
  self->priv = ntk_hw_drm_device_get_instance_private(self);
}

NtkHWDisplay* ntk_hw_drm_device_new(const gchar* path, GError** error) {
  return NTK_HW_DISPLAY(g_initable_new(NTK_HW_TYPE_DRM_DEVICE, NULL, error, "path", path, NULL));
}
