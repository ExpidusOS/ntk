#ifndef __NTK_HW_DISPLAY_H_
#define __NTK_HW_DISPLAY_H_ 1

#include "device.h"
#include <glib-object.h>

G_BEGIN_DECLS

#define NTK_HW_TYPE_DISPLAY ntk_hw_display_get_type()
G_DECLARE_DERIVABLE_TYPE(NtkHWDisplay, ntk_hw_display, NTK_HW, DISPLAY, GObject);

/**
 * NtkHWDisplay:
 *
 * Since: 0.1.0
 */
struct _NtkHWDisplayClass {
  GObjectClass parent_class;
};

G_END_DECLS

#endif
