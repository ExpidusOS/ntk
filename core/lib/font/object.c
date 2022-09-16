#define G_LOG_DOMAIN "NtkFont"
#include <ntk/font/object.h>

G_DEFINE_TYPE(NtkFont, ntk_font, G_TYPE_OBJECT);

static void ntk_font_class_init(NtkFontClass* klass) {}

static void ntk_font_init(NtkFont* self) {}

NtkUserFont* ntk_font_get_handle(NtkFont* self) {
  g_return_val_if_fail(NTK_IS_FONT(self), NULL);

  NtkFontClass* klass = NTK_FONT_GET_CLASS(self);
  g_return_val_if_fail(klass != NULL, NULL);
  g_return_val_if_fail(klass->get_handle != NULL, NULL);
  return klass->get_handle(self);
}
