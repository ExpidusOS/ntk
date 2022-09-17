#include <ntk/font/object.h>

G_DEFINE_TYPE(NtkFont, ntk_font, G_TYPE_OBJECT);

enum {
  SIG_HANDLE,
  N_SIGNALS
};

static guint obj_sigs[N_SIGNALS] = {0};

static void ntk_font_class_init(NtkFontClass* klass) {
  GObjectClass* object_class = G_OBJECT_CLASS(klass);

  obj_sigs[SIG_HANDLE] =
    g_signal_new("handle", G_OBJECT_CLASS_TYPE(object_class), G_SIGNAL_RUN_LAST, 0, NULL, NULL, NULL, G_TYPE_NONE, 0);
}

static void ntk_font_init(NtkFont* self) {}

NtkUserFont* ntk_font_get_handle(NtkFont* self) {
  g_return_val_if_fail(NTK_IS_FONT(self), NULL);

  NtkFontClass* klass = NTK_FONT_GET_CLASS(self);
  g_return_val_if_fail(klass != NULL, NULL);
  g_return_val_if_fail(klass->get_handle != NULL, NULL);

  g_signal_emit(self, obj_sigs[SIG_HANDLE], 0);
  return klass->get_handle(self);
}

gchar* ntk_font_get_name(NtkFont* self) {
  g_return_val_if_fail(NTK_IS_FONT(self), NULL);

  NtkFontClass* klass = NTK_FONT_GET_CLASS(self);
  g_return_val_if_fail(klass != NULL, NULL);
  g_return_val_if_fail(klass->get_name != NULL, NULL);
  return klass->get_name(self);
}
