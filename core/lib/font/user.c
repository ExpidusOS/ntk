#define G_LOG_DOMAIN "NtkFontUser"
#include <ntk/font/user.h>

G_DEFINE_BOXED_TYPE(NtkUserFont, ntk_user_font, ntk_user_font_copy, ntk_user_font_free);

NtkUserFont* ntk_user_font_copy(NtkUserFont* self) {
  g_return_val_if_fail(self != NULL, NULL);
  g_return_val_if_fail(self->impl_copy != NULL, NULL);
  return self->impl_copy(self);
}

void ntk_user_font_free(NtkUserFont* self) {
  g_return_if_fail(self != NULL);
  g_return_if_fail(self->impl_free != NULL);
  self->impl_free(self);
}
