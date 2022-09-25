#pragma once

#include <glib-object.h>
#include <nuklear.h>
#include <stdarg.h>

G_BEGIN_DECLS

#define NTK_TYPE_COLOR ntk_color_get_type()
GType ntk_color_get_type();

/**
 * NtkColorFormat:
 *
 * Since: 0.1.0
 */
typedef enum {
  NTK_COLOR_FORMAT_RGBA_FLOAT,
  NTK_COLOR_FORMAT_RGBA_INT,
  NTK_COLOR_FORMAT_RGBA_HEX,
  NTK_COLOR_FORMAT_RGB_FLOAT,
  NTK_COLOR_FORMAT_RGB_INT,
  NTK_COLOR_FORMAT_RGB_HEX,
  NTK_COLOR_FORMAT_HSVA_FLOAT,
  NTK_COLOR_FORMAT_HSVA_INT,
  NTK_COLOR_FORMAT_HSV_FLOAT,
  NTK_COLOR_FORMAT_HSV_INT,
  NTK_COLOR_FORMAT_NAMED
} NtkColorFormat;

/**
 * NtkColorRGBA:
 *
 * Since: 0.1.0
 */
typedef union {
  struct {
    float r;
    float g;
    float b;
    float a;
  } f;

  struct {
    int r;
    int g;
    int b;
    int a;
  } i;

  gchar h[8];
} NtkColorRGBA;

/**
 * NtkColorRGB:
 *
 * Since: 0.1.0
 */
typedef union {
  struct {
    float r;
    float g;
    float b;
  } f;

  struct {
    int r;
    int g;
    int b;
  } i;

  gchar h[8];
} NtkColorRGB;

/**
 * NtkColorHSVA:
 *
 * Since: 0.1.0
 */
typedef union {
  struct {
    float h;
    float s;
    float v;
    float a;
  } f;

  struct {
    int h;
    int s;
    int v;
    int a;
  } i;
} NtkColorHSVA;

/**
 * NtkColorHSV:
 *
 * Since: 0.1.0
 */
typedef union {
  struct {
    float h;
    float s;
    float v;
  } f;

  struct {
    int h;
    int s;
    int v;
  } i;
} NtkColorHSV;

/**
 * NtkColor:
 *
 * Since: 0.1.0
 */
typedef struct {
  NtkColorFormat fmt;

  union {
    NtkColorRGBA rgba;
    NtkColorRGB rgb;
    NtkColorHSVA hsva;
    NtkColorHSV hsv;
  } value;
} NtkColor;

/**
 * ntk_colormap: (array length=ntk_colormap_size):
 *
 * A 2-D array of two strings, [0] is the color name and [1] is the hex color.
 * This is used by #NtkColor for named color support with the NTK_COLOR_FORMAT_NAMED color format.
 *
 * Since: 0.1.0
 */
extern const char* ntk_colormap[][2];

/**
 * ntk_colormap_size:
 *
 * The number of elements in ntk_colormap.
 *
 * Since: 0.1.0
 */
extern const size_t ntk_colormap_size;

NtkColor* ntk_color_new(NtkColorFormat fmt, ...);
NtkColor* ntk_color_newv(NtkColorFormat fmt, va_list ap);

NtkColor* ntk_color_copy(NtkColor* self);
void ntk_color_free(NtkColor* self);

NtkColor* ntk_color_convert(NtkColor* self, NtkColorFormat fmt);

void ntk_color_set(NtkColor* self, ...);
void ntk_color_setv(NtkColor* self, va_list ap);

/**
 * ntk_color_nuke:
 * @self: An #NtkColor
 * @color_out: (out): The Nuklear color to dump the value to
 *
 * Converts the #NtkColor to a Nuklear color struct.
 *
 * Since: 0.1.0
 */
void ntk_color_nuke(NtkColor* self, struct nk_color* color_out);

/**
 * ntk_color_fnuke:
 * @self: An #NtkColor
 * @colorf_out: (out): The Nuklear floating-point color to dump the value to
 *
 * Converts the #NtkColor to a Nuklear floating-point color struct.
 *
 * Since: 0.1.0
 */
void ntk_color_fnuke(NtkColor* self, struct nk_colorf* colorf_out);

/**
 * ntk_color_to_string:
 * @self: An #NtkColor
 *
 * Since: 0.1.0
 * Returns: (transfer full): String representation of the color
 */
const char* ntk_color_to_string(NtkColor* self);

G_END_DECLS
