#include <ntk/color.h>
#include <ntk/styler.h>

gboolean ntk_styler_create_window_style(NtkStyler* self, NtkStylerElement* elems, struct nk_style_window* style) {
  g_return_val_if_fail(NTK_IS_STYLER(self), FALSE);
  g_return_val_if_fail(style != NULL, FALSE);

  NtkStylerElement* full_elems =
    ntk_styler_element_extend(elems, (NtkStylerElement[]){NTK_STYLER_ELEMENT_WINDOW, NTK_STYLER_ELEMENT_NONE});
  g_return_val_if_fail(full_elems != NULL, FALSE);

  NtkStylerKey key;
  key.elem = full_elems;
  key.state = (NtkStylerState[]){ NTK_STYLER_STATE_NORMAL, NTK_STYLER_STATE_NONE };
  if (!ntk_styler_create_window_header_style_for_key(self, key, &style->header)) {
    g_free(full_elems);
    g_return_val_if_reached(FALSE);
  }

  g_free(full_elems);
  return TRUE;
}
