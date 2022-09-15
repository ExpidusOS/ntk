#ifndef __NTK_HW_DEVICE_FILE_H_
#define __NTK_HW_DEVICE_FILE_H_ 1

#include <glib-object.h>
#include "device.h"

G_BEGIN_DECLS

#define NTK_HW_TYPE_DEVICE_FILE ntk_hw_device_file_get_type()
G_DECLARE_INTERFACE(NtkHWDeviceFile, ntk_hw_device_file, NTK_HW, DEVICE_FILE, GObject);

/**
 * NtkHWDeviceFile:
 *
 * Since: 0.1.0
 */
struct _NtkHWDeviceFileInterface {
  GTypeInterface parent_iface;

  gboolean (*open)(NtkHWDeviceFile* self, const gchar* path, GError** error);
  gboolean (*close)(NtkHWDeviceFile* self, GError** error);
  int (*get_fd)(NtkHWDeviceFile* self);
};

gboolean ntk_hw_device_file_open(NtkHWDeviceFile* self, const gchar* path, GError** error);
gboolean ntk_hw_device_file_close(NtkHWDeviceFile* self, GError** error);
int ntk_hw_device_file_get_fd(NtkHWDeviceFile* self);

G_END_DECLS

#endif
