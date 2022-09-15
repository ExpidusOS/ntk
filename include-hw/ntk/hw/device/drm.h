#ifndef __NTK_HW_DEVICE_DRM_H_
#define __NTK_HW_DEVICE_DRM_H_ 1

#include <ntk/hw/build.h>
#include <ntk/hw/device.h>
#include <ntk/hw/display.h>
#include <glib-object.h>

#ifndef NTK_HW_HAS_LIBDRM
#error "Libdrm support was not compiled into NTK"
#endif

G_BEGIN_DECLS

#define NTK_HW_TYPE_DRM_DEVICE ntk_hw_drm_device_get_type()
G_DECLARE_FINAL_TYPE(NtkHWDrmDevice, ntk_hw_drm_device, NTK_HW, DRM_DEVICE, NtkHWDisplay);

/**
 * NtkHWDrmDevice:
 *
 * Since: 0.1.0
 */
struct _NtkHWDrmDevice {
  NtkHWDisplay parent_instance;

  /*< private >*/
  struct _NtkHWDrmDevicePrivate* priv;
};

NtkHWDisplay* ntk_hw_drm_device_new(const gchar* path, GError** error);

G_END_DECLS

#endif
