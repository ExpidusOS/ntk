#include <ntk/color.h>
#include <glib.h>

/* FIXME: We copy this from lib/color.c but we should define the values inside a macro */
static const char* ntk_colormap[][2] = {
  {"white", "#ffffff"}, {"red", "#ff0000"}, {"green", "#00ff00"}, {"blue", "#0000ff"}, {"black", "#000000"}};

static void test_color_name(char* entry[2]) {
  NtkColor* color = ntk_color_new(NTK_COLOR_FORMAT_NAMED, entry[0]);
  g_assert(color != NULL);
  g_assert_cmpint(color->fmt, ==, NTK_COLOR_FORMAT_NAMED);
  g_assert_cmpstr(color->value.rgb.h, ==, entry[1] + 1);

  const gchar* string = ntk_color_to_string(color);
  g_assert(string != NULL);
  g_assert_cmpstr(string, ==, entry[0]);

  ntk_color_free(color);
}

int main(int argc, char** argv) {
  g_test_init(&argc, &argv, NULL);

  for (size_t i = 0; i < (sizeof ntk_colormap / sizeof ntk_colormap[0]); i++) {
    g_test_add_data_func(g_strdup_printf("/color/name/%s", ntk_colormap[i][0]), ntk_colormap[i], (GTestDataFunc)test_color_name);
  }
  return g_test_run();
}
