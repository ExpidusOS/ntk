#ifndef __NTK_HW_INPUT_H_
#define __NTK_HW_INPUT_H_ 1

#include <glib-object.h>
#include "device.h"

G_BEGIN_DECLS

#define NTK_HW_TYPE_INPUT ntk_hw_input_get_type()
G_DECLARE_DERIVABLE_TYPE(NtkHWInput, ntk_hw_input, NTK_HW, INPUT, GObject);

/**
 * NtkHWInput:
 *
 * Since: 0.1.0
 */
struct _NtkHWInputClass {
  GObjectClass parent_class;
};

G_END_DECLS

#endif
