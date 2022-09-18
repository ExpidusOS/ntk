#include "drm-priv.h"
#include <gio/gio.h>
#include <ntk/hw/device.h>
#include <ntk/hw/device/drm.h>

#define NTK_HW_DRM_DEVICE_PRIVATE(self) ((self)->priv == NULL ? ntk_hw_drm_display_get_instance_private(self) : (self)->priv)

static void ntk_hw_drm_display_initable_iface_init(GInitableIface* iface);

G_DEFINE_TYPE_WITH_CODE(
  NtkHWDrmDisplay, ntk_hw_drm_display, NTK_HW_TYPE_DISPLAY,
  G_ADD_PRIVATE(NtkHWDrmDisplay) G_IMPLEMENT_INTERFACE(G_TYPE_INITABLE, ntk_hw_drm_display_initable_iface_init)
);

static gboolean ntk_hw_drm_display_initable_init(GInitable* self, GCancellable* cancellable, GError** error) {
  return TRUE;
}

static void ntk_hw_drm_display_initable_iface_init(GInitableIface* iface) {
  iface->init = ntk_hw_drm_display_initable_init;
}

static void ntk_hw_drm_display_class_init(NtkHWDrmDisplayClass* klass) {}

static void ntk_hw_drm_display_init(NtkHWDrmDisplay* self) {
  self->priv = ntk_hw_drm_display_get_instance_private(self);
}

NtkHWDisplay* ntk_hw_drm_display_new(GError** error) {
  return NTK_HW_DISPLAY(g_initable_new(NTK_HW_TYPE_DRM_DEVICE, NULL, error, NULL));
}

NtkHWDisplay* ntk_hw_drm_display_new_with_path(const gchar* path, GError** error) {
  return NTK_HW_DISPLAY(g_initable_new(NTK_HW_TYPE_DRM_DEVICE, NULL, error, "path", path, NULL));
}
