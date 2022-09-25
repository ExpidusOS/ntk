#include <glib.h>
#include <ntk/color.h>
#include <ntk/styling/css.h>

#define BASIC_STYLING_PROP_COUNT 10
static const char BASIC_STYLING[] = "window, p {\n"
                                    "  background-image: url(\"~/path/to/image.png\");\n"
                                    "}\n"
                                    "input[type=\"text\"] {\n"
                                    "  background-color: #a9a9a9;\n"
                                    "  color: red;\n"
                                    "}\n"
                                    "button[type=\"context\"] {\n"
                                    "  background-color: #a9a9a9;\n"
                                    "  color: red;\n"
                                    "}\n"
                                    "scrollbar > button.inc {\n"
                                    "  background-color: #a9a9a9;\n"
                                    "  color: red;\n"
                                    "}\n"
                                    "window > window-header {\n"
                                    "  background-color: #a9a9a9;\n"
                                    "  color: red;\n"
                                    "}";

static void test_styler_load() {
  NtkCSSStyler* styler = NTK_CSS_STYLER(ntk_css_styler_new());
  g_assert(styler != NULL);

  GError* error = NULL;
  g_assert(ntk_css_styler_load(styler, BASIC_STYLING, sizeof(BASIC_STYLING) / sizeof(char), &error));
  g_assert_no_error(error);

  GHashTable* tbl = ntk_styler_export(NTK_STYLER(styler));
  g_assert(tbl != NULL);

  GHashTableIter tbl_iter;
  g_hash_table_iter_init(&tbl_iter, tbl);

  gpointer tbl_key;
  gpointer tbl_value;
  while (g_hash_table_iter_next(&tbl_iter, &tbl_key, &tbl_value)) {
    NtkStylerKey* key = (NtkStylerKey*)tbl_key;
    GValue* value = (GValue*)tbl_value;

    if (G_VALUE_TYPE(value) == NTK_TYPE_COLOR) {
      NtkColor* color = g_value_get_boxed(value);
      g_debug("Key: (%s), Value: %s", ntk_styler_key_to_string(key), ntk_color_to_string(color));
    } else {
      g_debug("Key: (%s), Value: %s", ntk_styler_key_to_string(key), g_strdup_value_contents(value));
    }
  }

  g_assert_cmpint(g_hash_table_size(tbl), ==, BASIC_STYLING_PROP_COUNT);

  g_hash_table_unref(tbl);
  g_object_unref(styler);
}

int main(int argc, char** argv) {
  g_test_init(&argc, &argv, NULL);
  g_test_add_func("/styler/load", test_styler_load);
  return g_test_run();
}
