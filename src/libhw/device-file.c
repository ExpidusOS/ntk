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

//NtkHWDeviceFile* ntk_hw_device_file_new(GType type, const gchar* path, GError** error, const gchar* first_property_name, ...) {
//}

NtkHWDeviceFile* ntk_hw_device_file_new_valist(GType type, gchar* path, GError** error, const gchar* first_property_name, va_list list) {
  size_t count = 1;

  if (first_property_name != NULL) {
    va_list list2;
    va_copy(list2, list);
    while (1) {
      const gchar* name = va_arg(list2, const gchar*);
      if (name == NULL) break;

      GValue* value = va_arg(list2, GValue*);
      g_assert(value != NULL);
      count++;
    }
    va_end(list2);
  }

  gchar** names = g_try_malloc0(sizeof (char*) * count);
  g_return_val_if_fail(names != NULL && count > 0, NULL);

  GValue* values = g_try_malloc0(sizeof (GValue) * count);
  g_return_val_if_fail(values != NULL && count > 0, NULL);

  for (size_t i = 0; i < (count - 1); i++) {
    names[i] = g_strdup(va_arg(list, const gchar*));

    GValue* value = va_arg(list, GValue*);
    g_value_copy(&values[i], value);
  }

  names[count - 1] = g_strdup("path");
  g_value_copy(&values[count - 1], g_value_init((GValue*)path, G_TYPE_STRING));

  NtkHWDeviceFile* self = NTK_HW_DEVICE_FILE(g_object_new_with_properties(type, count, (const gchar**)names, (const GValue*)values));

  for (size_t i = 0; i < (count - 1); i++) g_clear_pointer((char**)&names[i], g_free);
  g_clear_pointer(&names, g_free);
  g_clear_pointer(&values, g_value_unset);
  return self;
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
