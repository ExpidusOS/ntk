#pragma once

#include <ntk/hw/build.h>

#ifndef NTK_HW_HAS_LIBDRM
#error "Libdrm support was not compiled into NTK"
#endif

#include <drm/drm.h>
#include <xf86drm.h>
#include <glib-object.h>
#include <ntk/hw/build.h>
#include <ntk/hw/device.h>
#include <ntk/hw/display.h>

G_BEGIN_DECLS

#define NTK_HW_TYPE_DRM_DISPLAY ntk_hw_drm_render_get_type()
G_DECLARE_FINAL_TYPE(NtkHWDrmRender, ntk_hw_drm_render, NTK_HW, DRM_RENDER, NtkHWDisplay);

/**
 * NtkHWDrmRender:
 *
 * Since: 0.1.0
 */
struct _NtkHWDrmRender {
  NtkHWDisplay parent_instance;

  /*< private >*/
  struct _NtkHWDrmRenderPrivate* priv;
};

NtkHWDisplay* ntk_hw_drm_render_new(GError** error);
NtkHWDisplay* ntk_hw_drm_render_new_with_path(const gchar* path, GError** error);

int ntk_hw_drm_render_get_fd(NtkHWDrmRender* self);
gchar* ntk_hw_drm_render_get_path(NtkHWDrmRender* self);
drmDevice* ntk_hw_drm_render_get_device(NtkHWDrmRender* self);
gboolean ntk_hw_drm_render_has_name(NtkHWDrmRender* self, const gchar* name);

G_END_DECLS
