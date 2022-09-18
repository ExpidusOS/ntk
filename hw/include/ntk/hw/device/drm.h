#pragma once

#include <ntk/hw/build.h>

#ifndef NTK_HW_HAS_LIBDRM
#error "Libdrm support was not compiled into NTK"
#endif

#include <drm/drm.h>
#include <glib-object.h>
#include <ntk/hw/build.h>
#include <ntk/hw/device.h>
#include <ntk/hw/display.h>

G_BEGIN_DECLS

#define NTK_HW_TYPE_DRM_DEVICE ntk_hw_drm_display_get_type()
G_DECLARE_FINAL_TYPE(NtkHWDrmDisplay, ntk_hw_drm_display, NTK_HW, DRM_DEVICE, NtkHWDisplay);

/**
 * NtkHWDrmDisplay:
 *
 * Since: 0.1.0
 */
struct _NtkHWDrmDisplay {
  NtkHWDisplay parent_instance;

  /*< private >*/
  struct _NtkHWDrmDisplayPrivate* priv;
};

NtkHWDisplay* ntk_hw_drm_display_new(GError** error);
NtkHWDisplay* ntk_hw_drm_display_new_with_path(const gchar* path, GError** error);

G_END_DECLS
