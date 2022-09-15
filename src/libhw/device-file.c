#define G_LOG_DOMAIN "NtkHWDeviceFile"
#include <ntk/hw/device-file.h>
#include <gio/gio.h>

static void ntk_hw_device_file_initable_interface_init(GInitableIface* iface);
static void ntk_hw_device_file_device_interface_init(NtkHWDeviceInterface* iface);

G_DEFINE_INTERFACE_WITH_CODE(NtkHWDeviceFile, ntk_hw_device_file, NTK_HW_TYPE_DEVICE_FILE,
  G_IMPLEMENT_INTERFACE(G_TYPE_INITABLE, ntk_hw_device_file_initable_interface_init)
  G_IMPLEMENT_INTERFACE(NTK_HW_TYPE_DEVICE, ntk_hw_device_file_device_interface_init));

static gboolean ntk_hw_device_file_initable_init(GInitable* initable, GCancellable* cancellable, GError** error) {
  g_return_val_if_fail(NTK_HW_IS_DEVICE_FILE(initable), FALSE);
  NtkHWDeviceFile* self = NTK_HW_DEVICE_FILE(initable);
  g_return_val_if_fail(self != NULL, FALSE);

  gchar* path = NULL;
  g_object_get(G_OBJECT(self), "path", &path, NULL);
  g_return_val_if_fail(path != NULL, FALSE);

  gboolean result = ntk_hw_device_file_open(self, path, error);
  g_free(path);
  return result;
}

static void ntk_hw_device_file_initable_interface_init(GInitableIface* iface) {
  iface->init = ntk_hw_device_file_initable_init;
}

static void ntk_hw_device_file_device_interface_init(NtkHWDeviceInterface* iface) {}

static void ntk_hw_device_file_default_init(NtkHWDeviceFileInterface* iface) {
  g_object_interface_install_property(iface, g_param_spec_string("path", "Path", "The path to the device file", NULL, G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE));
}

gboolean ntk_hw_device_file_open(NtkHWDeviceFile* self, const gchar* path, GError** error) {
  g_return_val_if_fail(NTK_HW_IS_DEVICE_FILE(self), FALSE);
  g_return_val_if_fail(error == NULL || *error == NULL, FALSE);

  NtkHWDeviceFileInterface* iface = NTK_HW_DEVICE_FILE_GET_IFACE(self);
  g_return_val_if_fail(iface != NULL, FALSE);
  g_return_val_if_fail(iface->open != NULL, FALSE);
  return iface->open(self, path, error);
}

gboolean ntk_hw_device_file_close(NtkHWDeviceFile* self, GError** error) {
  g_return_val_if_fail(NTK_HW_IS_DEVICE_FILE(self), FALSE);
  g_return_val_if_fail(error == NULL || *error == NULL, FALSE);

  NtkHWDeviceFileInterface* iface = NTK_HW_DEVICE_FILE_GET_IFACE(self);
  g_return_val_if_fail(iface != NULL, FALSE);
  g_return_val_if_fail(iface->close != NULL, FALSE);
  return iface->close(self, error);
}

int ntk_hw_device_file_get_fd(NtkHWDeviceFile* self) {
  g_return_val_if_fail(NTK_HW_IS_DEVICE_FILE(self), FALSE);

  NtkHWDeviceFileInterface* iface = NTK_HW_DEVICE_FILE_GET_IFACE(self);
  g_return_val_if_fail(iface != NULL, FALSE);
  g_return_val_if_fail(iface->get_fd != NULL, FALSE);
  return iface->get_fd(self);
}
