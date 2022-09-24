#include <ntk/color.h>

G_DEFINE_BOXED_TYPE(NtkColor, ntk_color, ntk_color_copy, ntk_color_free);

NtkColor* ntk_color_new(NtkColorFormat fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  NtkColor* color = ntk_color_newv(fmt, ap);
  va_end(ap);
  return color;
}

NtkColor* ntk_color_newv(NtkColorFormat fmt, va_list ap) {
  NtkColor* self = g_try_malloc0(sizeof(NtkColor));
  g_return_val_if_fail(self != NULL, NULL);

  self->fmt = fmt;
  ntk_color_setv(self, ap);
  return self;
}

NtkColor* ntk_color_copy(NtkColor* self) {
  g_return_val_if_fail(self != NULL, NULL);

  NtkColor* c = g_try_malloc0(sizeof(NtkColor));
  g_return_val_if_fail(c != NULL, NULL);

  c->fmt = self->fmt;

  switch (c->fmt) {
    case NTK_COLOR_FORMAT_RGBA_FLOAT:
      c->value.rgba.f.r = self->value.rgba.f.r;
      c->value.rgba.f.g = self->value.rgba.f.g;
      c->value.rgba.f.b = self->value.rgba.f.b;
      c->value.rgba.f.a = self->value.rgba.f.a;
      break;
    case NTK_COLOR_FORMAT_RGBA_INT:
      c->value.rgba.i.r = self->value.rgba.i.r;
      c->value.rgba.i.g = self->value.rgba.i.g;
      c->value.rgba.i.b = self->value.rgba.i.b;
      c->value.rgba.i.a = self->value.rgba.i.a;
      break;
    case NTK_COLOR_FORMAT_RGBA_HEX:
      memcpy(c->value.rgba.h, self->value.rgba.h, sizeof(char) * 8);
      break;
    case NTK_COLOR_FORMAT_RGB_FLOAT:
      c->value.rgb.f.r = self->value.rgb.f.r;
      c->value.rgb.f.g = self->value.rgb.f.g;
      c->value.rgb.f.b = self->value.rgb.f.b;
      break;
    case NTK_COLOR_FORMAT_RGB_INT:
      c->value.rgb.i.r = self->value.rgb.i.r;
      c->value.rgb.i.g = self->value.rgb.i.g;
      c->value.rgb.i.b = self->value.rgb.i.b;
      break;
    case NTK_COLOR_FORMAT_RGB_HEX:
      memcpy(c->value.rgb.h, self->value.rgb.h, sizeof(char) * 6);
      break;
    case NTK_COLOR_FORMAT_HSVA_FLOAT:
      c->value.hsva.f.h = self->value.hsva.f.h;
      c->value.hsva.f.s = self->value.hsva.f.s;
      c->value.hsva.f.v = self->value.hsva.f.v;
      c->value.hsva.f.a = self->value.hsva.f.a;
      break;
    case NTK_COLOR_FORMAT_HSVA_INT:
      c->value.hsva.i.h = self->value.hsva.i.h;
      c->value.hsva.i.s = self->value.hsva.i.s;
      c->value.hsva.i.v = self->value.hsva.i.v;
      c->value.hsva.i.a = self->value.hsva.i.a;
      break;
    case NTK_COLOR_FORMAT_HSV_FLOAT:
      c->value.hsv.f.h = self->value.hsv.f.h;
      c->value.hsv.f.s = self->value.hsv.f.s;
      c->value.hsv.f.v = self->value.hsv.f.v;
      break;
    case NTK_COLOR_FORMAT_HSV_INT:
      c->value.hsv.i.h = self->value.hsv.i.h;
      c->value.hsv.i.s = self->value.hsv.i.s;
      c->value.hsv.i.v = self->value.hsv.i.v;
      break;
  }
  return c;
}

void ntk_color_free(NtkColor* self) {
  g_free(self);
}

NtkColor* ntk_color_convert(NtkColor* self, NtkColorFormat fmt) {
  NtkColor* c = ntk_color_copy(self);
  g_return_val_if_fail(c != NULL, NULL);

  c->fmt = fmt;
  if (self->fmt == fmt) return c;

  struct nk_color color;
  ntk_color_nuke(self, &color);

  switch (fmt) {
    case NTK_COLOR_FORMAT_RGBA_FLOAT:
      nk_color_f(&self->value.rgba.f.r, &self->value.rgba.f.g, &self->value.rgba.f.b, &self->value.rgba.f.a, color);
      break;
    case NTK_COLOR_FORMAT_RGBA_INT:
      self->value.rgba.i.r = color.r;
      self->value.rgba.i.g = color.g;
      self->value.rgba.i.b = color.b;
      self->value.rgba.i.a = color.a;
      break;
    case NTK_COLOR_FORMAT_RGBA_HEX:
      nk_color_hex_rgba(self->value.rgba.h, color);
      break;
    case NTK_COLOR_FORMAT_RGB_FLOAT:
      {
        struct nk_colorf colorf = nk_color_cf(color);
        self->value.rgb.f.r = colorf.r;
        self->value.rgb.f.g = colorf.g;
        self->value.rgb.f.b = colorf.b;
      }
      break;
    case NTK_COLOR_FORMAT_RGB_INT:
      self->value.rgb.i.r = color.r;
      self->value.rgb.i.g = color.g;
      self->value.rgb.i.b = color.b;
      break;
    case NTK_COLOR_FORMAT_RGB_HEX:
      nk_color_hex_rgb(self->value.rgb.h, color);
      break;
    case NTK_COLOR_FORMAT_HSVA_FLOAT:
      nk_color_hsva_f(&self->value.hsva.f.h, &self->value.hsva.f.s, &self->value.hsva.f.v, &self->value.hsva.f.a, color);
      break;
    case NTK_COLOR_FORMAT_HSVA_INT:
      nk_color_hsva_i(&self->value.hsva.i.h, &self->value.hsva.i.s, &self->value.hsva.i.v, &self->value.hsva.i.a, color);
      break;
    case NTK_COLOR_FORMAT_HSV_FLOAT:
      nk_color_hsv_f(&self->value.hsv.f.h, &self->value.hsv.f.s, &self->value.hsv.f.v, color);
      break;
    case NTK_COLOR_FORMAT_HSV_INT:
      nk_color_hsv_i(&self->value.hsv.i.h, &self->value.hsv.i.s, &self->value.hsv.i.v, color);
      break;
  }
  return c;
}

void ntk_color_set(NtkColor* self, ...) {
  g_return_if_fail(self != NULL);

  va_list ap;
  va_start(ap, self);
  ntk_color_setv(self, ap);
  va_end(ap);
}

void ntk_color_setv(NtkColor* self, va_list ap) {
  g_return_if_fail(self != NULL);

  switch (self->fmt) {
    case NTK_COLOR_FORMAT_RGBA_FLOAT:
      self->value.rgba.f.r = va_arg(ap, double);
      self->value.rgba.f.g = va_arg(ap, double);
      self->value.rgba.f.b = va_arg(ap, double);
      self->value.rgba.f.a = va_arg(ap, double);
      break;
    case NTK_COLOR_FORMAT_RGBA_INT:
      self->value.rgba.i.r = va_arg(ap, int);
      self->value.rgba.i.g = va_arg(ap, int);
      self->value.rgba.i.b = va_arg(ap, int);
      self->value.rgba.i.a = va_arg(ap, int);
      break;
    case NTK_COLOR_FORMAT_RGBA_HEX:
      {
        char* hex = va_arg(ap, char*);
        g_return_if_fail(strlen(hex) == 8);
        memcpy(self->value.rgba.h, hex, sizeof(char) * 8);
      }
      break;
    case NTK_COLOR_FORMAT_RGB_FLOAT:
      self->value.rgb.f.r = va_arg(ap, double);
      self->value.rgb.f.g = va_arg(ap, double);
      self->value.rgb.f.b = va_arg(ap, double);
      break;
    case NTK_COLOR_FORMAT_RGB_INT:
      self->value.rgb.i.r = va_arg(ap, int);
      self->value.rgb.i.g = va_arg(ap, int);
      self->value.rgb.i.b = va_arg(ap, int);
      break;
    case NTK_COLOR_FORMAT_RGB_HEX:
      {
        char* hex = va_arg(ap, char*);
        g_return_if_fail(strlen(hex) == 6);
        memcpy(self->value.rgb.h, hex, sizeof(char) * 6);
      }
      break;
    case NTK_COLOR_FORMAT_HSVA_FLOAT:
      self->value.hsva.f.h = va_arg(ap, double);
      self->value.hsva.f.s = va_arg(ap, double);
      self->value.hsva.f.v = va_arg(ap, double);
      self->value.hsva.f.a = va_arg(ap, double);
      break;
    case NTK_COLOR_FORMAT_HSVA_INT:
      self->value.hsva.i.h = va_arg(ap, int);
      self->value.hsva.i.s = va_arg(ap, int);
      self->value.hsva.i.v = va_arg(ap, int);
      self->value.hsva.i.a = va_arg(ap, int);
      break;
    case NTK_COLOR_FORMAT_HSV_FLOAT:
      self->value.hsv.f.h = va_arg(ap, double);
      self->value.hsv.f.s = va_arg(ap, double);
      self->value.hsv.f.v = va_arg(ap, double);
      break;
    case NTK_COLOR_FORMAT_HSV_INT:
      self->value.hsv.i.h = va_arg(ap, int);
      self->value.hsv.i.s = va_arg(ap, int);
      self->value.hsv.i.v = va_arg(ap, int);
      break;
  }
}

void ntk_color_nuke(NtkColor* self, struct nk_color* color_out) {
  g_return_if_fail(self != NULL);
  g_return_if_fail(color_out != NULL);

  switch (self->fmt) {
    case NTK_COLOR_FORMAT_RGBA_FLOAT:
      *color_out = nk_rgba_f(self->value.rgba.f.r, self->value.rgba.f.g, self->value.rgba.f.b, self->value.rgba.f.a);
      break;
    case NTK_COLOR_FORMAT_RGBA_INT:
      *color_out = nk_rgba(self->value.rgba.i.r, self->value.rgba.i.g, self->value.rgba.i.b, self->value.rgba.i.a);
      break;
    case NTK_COLOR_FORMAT_RGBA_HEX:
      *color_out = nk_rgba_hex(self->value.rgba.h);
      break;
    case NTK_COLOR_FORMAT_RGB_FLOAT:
      *color_out = nk_rgb_f(self->value.rgb.f.r, self->value.rgb.f.g, self->value.rgb.f.b);
      break;
    case NTK_COLOR_FORMAT_RGB_INT:
      *color_out = nk_rgb(self->value.rgb.i.r, self->value.rgb.i.g, self->value.rgb.i.b);
      break;
    case NTK_COLOR_FORMAT_RGB_HEX:
      *color_out = nk_rgb_hex(self->value.rgb.h);
      break;
    case NTK_COLOR_FORMAT_HSVA_FLOAT:
      *color_out = nk_hsva_f(self->value.hsva.f.h, self->value.hsva.f.s, self->value.hsva.f.v, self->value.hsva.f.a);
      break;
    case NTK_COLOR_FORMAT_HSVA_INT:
      *color_out = nk_hsva(self->value.hsva.i.h, self->value.hsva.i.s, self->value.hsva.i.v, self->value.hsva.i.a);
      break;
    case NTK_COLOR_FORMAT_HSV_FLOAT:
      *color_out = nk_hsv_f(self->value.hsv.f.h, self->value.hsv.f.s, self->value.hsv.f.v);
      break;
    case NTK_COLOR_FORMAT_HSV_INT:
      *color_out = nk_hsv(self->value.hsv.i.h, self->value.hsv.i.s, self->value.hsv.i.v);
      break;
  }
}

void ntk_color_nukef(NtkColor* self, struct nk_colorf* colorf_out) {
  g_return_if_fail(self != NULL);
  g_return_if_fail(colorf_out != NULL);

  struct nk_color color;
  ntk_color_nuke(self, &color);
  *colorf_out = nk_color_cf(color);
}
