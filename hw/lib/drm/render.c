#include "render-priv.h"
#include <gio/gio.h>
#include <ntk/hw/device.h>
#include <ntk/hw/drm/render.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include "../error-priv.h"

#define NTK_HW_DRM_RENDER_PRIVATE(self) ((self)->priv == NULL ? ntk_hw_drm_render_get_instance_private(self) : (self)->priv)

enum {
  PROP_0,
  PROP_FD,
  PROP_PATH,
  PROP_DEVICE,
  N_PROPERTIES
};

static GParamSpec* obj_props[N_PROPERTIES] = {
  NULL,
};

static void ntk_hw_drm_render_initable_iface_init(GInitableIface* iface);

G_DEFINE_TYPE_WITH_CODE(
  NtkHWDrmRender, ntk_hw_drm_render, NTK_HW_TYPE_DISPLAY,
  G_ADD_PRIVATE(NtkHWDrmRender) G_IMPLEMENT_INTERFACE(G_TYPE_INITABLE, ntk_hw_drm_render_initable_iface_init)
);

static gboolean ntk_hw_drm_render_initable_init(GInitable* initable, GCancellable* cancellable, GError** error) {
  NtkHWDrmRender* self = NTK_HW_DRM_RENDER(initable);
  NtkHWDrmRenderPrivate* priv = NTK_HW_DRM_RENDER_PRIVATE(self);

  if (priv->path == NULL) {
    ntk_hw_error_set_bad_device(error, "device path is not set", NTK_HW_DEVICE(self));
    return FALSE;
  }

  g_debug("Initializing DRM render for %s", priv->path);

  priv->fd = open(priv->path, O_RDWR | O_CLOEXEC);
  if (priv->fd < 0) {
    ntk_hw_error_set_bad_device(error, "failed to open device", NTK_HW_DEVICE(self));
    return FALSE;
  }

  g_debug("Opened DRM render device fd: %d", priv->fd);

  int ret = drmGetDevice(priv->fd, &priv->device);
  if (ret < 0) {
    ntk_hw_error_set_bad_device(error, "failed to get DRM device", NTK_HW_DEVICE(self));
    return FALSE;
  }
  return TRUE;
}

static void ntk_hw_drm_render_finalize(GObject* obj) {
  NtkHWDrmRender* self = NTK_HW_DRM_RENDER(obj);
  NtkHWDrmRenderPrivate* priv = NTK_HW_DRM_RENDER_PRIVATE(self);

  if (priv->fd > 0) {
    g_debug("Closing %s", priv->path);
    close(priv->fd);
    priv->fd = 0;
  }

  g_clear_pointer(&priv->path, g_free);

  if (priv->device != NULL) {
    drmFreeDevice(&priv->device);
    priv->device = NULL;
  }

  G_OBJECT_CLASS(ntk_hw_drm_render_parent_class)->finalize(obj);
}

static void ntk_hw_drm_render_set_property(GObject* obj, guint prop_id, const GValue* value, GParamSpec* pspec) {
  NtkHWDrmRender* self = NTK_HW_DRM_RENDER(obj);
  NtkHWDrmRenderPrivate* priv = NTK_HW_DRM_RENDER_PRIVATE(self);

  switch (prop_id) {
    case PROP_PATH:
      priv->path = g_value_dup_string(value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
      break;
  }
}

static void ntk_hw_drm_render_get_property(GObject* obj, guint prop_id, GValue* value, GParamSpec* pspec) {
  NtkHWDrmRender* self = NTK_HW_DRM_RENDER(obj);
  NtkHWDrmRenderPrivate* priv = NTK_HW_DRM_RENDER_PRIVATE(self);

  switch (prop_id) {
    case PROP_FD:
      g_value_set_int(value, priv->fd);
      break;
    case PROP_PATH:
      g_value_set_string(value, priv->path);
      break;
    case PROP_DEVICE:
      g_value_set_pointer(value, priv->device);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
      break;
  }
}

static void ntk_hw_drm_render_initable_iface_init(GInitableIface* iface) {
  iface->init = ntk_hw_drm_render_initable_init;
}

static void ntk_hw_drm_render_class_init(NtkHWDrmRenderClass* klass) {
  GObjectClass* object_class = G_OBJECT_CLASS(klass);

  object_class->finalize = ntk_hw_drm_render_finalize;

  object_class->set_property = ntk_hw_drm_render_set_property;
  object_class->get_property = ntk_hw_drm_render_get_property;

  obj_props[PROP_FD] = g_param_spec_int("fd", "File descriptor", "The file descriptor utilized with this device", 0, INT_MAX, 0, G_PARAM_READABLE);
  obj_props[PROP_PATH] = g_param_spec_string("path", "Device path", "The path to the DRM render device node", NULL, G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  obj_props[PROP_DEVICE] = g_param_spec_pointer("device", "Device", "The DRM device instance", G_PARAM_READABLE);
  g_object_class_install_properties(object_class, N_PROPERTIES, obj_props);
}

static void ntk_hw_drm_render_init(NtkHWDrmRender* self) {
  self->priv = ntk_hw_drm_render_get_instance_private(self);
}

NtkHWDisplay* ntk_hw_drm_render_new(GError** error) {
  int device_count = drmGetDevices2(0, NULL, 0);
  if (device_count < 0) {
    ntk_hw_error_set_bad_device(error, "failed to read the device count with DRM", NULL);
    return NULL;
  }

  drmDevice** devices = calloc(device_count, sizeof (drmDevice*));
  if (devices == NULL) {
    ntk_hw_error_set_bad_device(error, "failed to allocate the device array for DRM", NULL);
    return NULL;
  }

  device_count = drmGetDevices2(0, devices, device_count);
  if (device_count < 0) {
    free(devices);
    ntk_hw_error_set_bad_device(error, "failed to get the devices with DRM", NULL);
    return NULL;
  }

  for (int i = 0; i < device_count; i++) {
    drmDevice* dev = devices[i];
    if (dev->available_nodes & (1 << DRM_NODE_RENDER)) {
      const char* name = dev->nodes[DRM_NODE_RENDER];
      int fd = open(name, O_RDWR | O_CLOEXEC);
      g_debug("Found DRM render device node %s", name);
      if (fd < 0) {
        g_warning("Failed to open DRM render device node %s", name);
        continue;
      }

      NtkHWDisplay* display = ntk_hw_drm_render_new_with_path(name, error);
      for (int i = 0; i < device_count; i++) drmFreeDevice(&devices[i]);
      close(fd);
      free(devices);
      return display;
    }
  }

  for (int i = 0; i < device_count; i++) drmFreeDevice(&devices[i]);
  free(devices);
  ntk_hw_error_set_bad_device(error, "failed to find a DRM device with a render node", NULL);
  return NULL;
}

NtkHWDisplay* ntk_hw_drm_render_new_with_path(const gchar* path, GError** error) {
  return NTK_HW_DISPLAY(g_initable_new(NTK_HW_TYPE_DRM_DISPLAY, NULL, error, "path", path, NULL));
}

int ntk_hw_drm_render_get_fd(NtkHWDrmRender* self) {
  g_return_val_if_fail(NTK_HW_IS_DRM_RENDER(self), -1);
  NtkHWDrmRenderPrivate* priv = NTK_HW_DRM_RENDER_PRIVATE(self);
  g_return_val_if_fail(priv != NULL, -1);
  return priv->fd;
}

gchar* ntk_hw_drm_render_get_path(NtkHWDrmRender* self) {
  g_return_val_if_fail(NTK_HW_IS_DRM_RENDER(self), NULL);
  NtkHWDrmRenderPrivate* priv = NTK_HW_DRM_RENDER_PRIVATE(self);
  g_return_val_if_fail(priv != NULL, NULL);
  return priv->path;
}

drmDevice* ntk_hw_drm_render_get_device(NtkHWDrmRender* self) {
  g_return_val_if_fail(NTK_HW_IS_DRM_RENDER(self), NULL);
  NtkHWDrmRenderPrivate* priv = NTK_HW_DRM_RENDER_PRIVATE(self);
  g_return_val_if_fail(priv != NULL, NULL);
  return priv->device;
}

gboolean ntk_hw_drm_render_has_name(NtkHWDrmRender* self, const gchar* name) {
  g_return_val_if_fail(NTK_HW_IS_DRM_RENDER(self), FALSE);
  NtkHWDrmRenderPrivate* priv = NTK_HW_DRM_RENDER_PRIVATE(self);
  g_return_val_if_fail(priv != NULL, FALSE);

  for (size_t i = 0; i < DRM_NODE_MAX; i++) {
    if (!(priv->device->available_nodes & (1 << i))) continue;
    if (g_strcmp0(priv->device->nodes[i], name) == 0) return TRUE;
  }
  return FALSE;
}
