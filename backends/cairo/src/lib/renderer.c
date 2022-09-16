#define G_LOG_DOMAIN "NtkCairoRenderer"
#include <ntk/backend/cairo/renderer.h>
#include <ntk/font.h>
#include <ntk/utils.h>
#ifdef CAIRO_HAS_GOBJECT_FUNCTIONS
#include <cairo-gobject.h>
#endif
#include "renderer-priv.h"
#include <math.h>
#include <pango/pangocairo.h>

#define NTK_CAIRO_RENDERER_PRIVATE(self)                                                                                    \
  ((self)->priv == NULL ? ntk_cairo_renderer_get_instance_private(self) : (self)->priv)

enum {
  PROP_0,
  PROP_WIDTH,
  PROP_HEIGHT,
  PROP_SURFACE,
  N_PROPERTIES,
};

static GParamSpec* obj_props[N_PROPERTIES] = {
  NULL,
};

G_DEFINE_TYPE_WITH_PRIVATE(NtkCairoRenderer, ntk_cairo_renderer, NTK_TYPE_RENDERER);

static NtkRendererType ntk_cairo_renderer_get_render_type(NtkRenderer* renderer) {
  return NTK_RENDERER_TYPE_COMMAND;
}

static gboolean ntk_cairo_renderer_render_command(NtkRenderer* renderer, const struct nk_command* cmd, GError** error) {
  NtkCairoRenderer* self = NTK_CAIRO_RENDERER(renderer);
  NtkCairoRendererPrivate* priv = NTK_CAIRO_RENDERER_PRIVATE(self);

  ntk_command_debug(cmd);

  switch (cmd->type) {
    case NK_COMMAND_NOP:
      break;
    case NK_COMMAND_SCISSOR:
      {
        struct nk_command_scissor* c = (struct nk_command_scissor*)cmd;

        cairo_reset_clip(priv->cr);
        cairo_new_path(priv->cr);
        cairo_move_to(priv->cr, c->x, c->y);
        cairo_line_to(priv->cr, c->x + c->w, c->y);
        cairo_line_to(priv->cr, c->x + c->w, c->y + c->h);
        cairo_line_to(priv->cr, c->x, c->y + c->h);
        cairo_line_to(priv->cr, c->x, c->y);
        cairo_clip(priv->cr);
      }
      break;
    case NK_COMMAND_LINE:
      {
        struct nk_command_line* c = (struct nk_command_line*)cmd;

        cairo_set_source_rgba(priv->cr, c->color.r / 255.0, c->color.g / 255.0, c->color.b / 255.0, c->color.a / 255.0);
        cairo_set_line_width(priv->cr, c->line_thickness);
        cairo_move_to(priv->cr, c->begin.x, c->begin.y);
        cairo_line_to(priv->cr, c->end.x, c->end.y);
        cairo_stroke(priv->cr);
      }
      break;
    case NK_COMMAND_POLYGON:
      {
        struct nk_command_polygon* c = (struct nk_command_polygon*)cmd;

        cairo_set_source_rgba(priv->cr, c->color.r / 255.0, c->color.g / 255.0, c->color.b / 255.0, c->color.a / 255.0);
        cairo_set_line_width(priv->cr, c->line_thickness);
        cairo_new_sub_path(priv->cr);
        cairo_move_to(priv->cr, c->points[0].x, c->points[0].y);

        for (int i = 0; i < c->point_count; i++) {
          cairo_line_to(priv->cr, c->points[i].x, c->points[i].y);
        }

        cairo_line_to(priv->cr, c->points[0].x, c->points[0].y);
        cairo_close_path(priv->cr);
        cairo_stroke(priv->cr);
      }
      break;
    case NK_COMMAND_POLYGON_FILLED:
      {
        struct nk_command_polygon_filled* c = (struct nk_command_polygon_filled*)cmd;

        cairo_set_source_rgba(priv->cr, c->color.r / 255.0, c->color.g / 255.0, c->color.b / 255.0, c->color.a / 255.0);
        cairo_new_sub_path(priv->cr);
        cairo_move_to(priv->cr, c->points[0].x, c->points[0].y);

        for (int i = 0; i < c->point_count; i++) {
          cairo_line_to(priv->cr, c->points[i].x, c->points[i].y);
        }

        cairo_line_to(priv->cr, c->points[0].x, c->points[0].y);
        cairo_close_path(priv->cr);
        cairo_fill(priv->cr);
      }
      break;
    case NK_COMMAND_ARC:
      {
        struct nk_command_arc* c = (struct nk_command_arc*)cmd;

        cairo_set_source_rgba(priv->cr, c->color.r / 255.0, c->color.g / 255.0, c->color.b / 255.0, c->color.a / 255.0);
        cairo_set_line_width(priv->cr, c->line_thickness);
        cairo_new_sub_path(priv->cr);
        cairo_arc(priv->cr, c->cx, c->cy, c->r, c->a[0], c->a[1]);
        cairo_close_path(priv->cr);
        cairo_stroke(priv->cr);
      }
      break;
    case NK_COMMAND_ARC_FILLED:
      {
        struct nk_command_arc_filled* c = (struct nk_command_arc_filled*)cmd;

        cairo_set_source_rgba(priv->cr, c->color.r / 255.0, c->color.g / 255.0, c->color.b / 255.0, c->color.a / 255.0);
        cairo_new_sub_path(priv->cr);
        cairo_arc(priv->cr, c->cx, c->cy, c->r, c->a[0], c->a[1]);
        cairo_close_path(priv->cr);
        cairo_fill(priv->cr);
      }
      break;
    case NK_COMMAND_CIRCLE:
      {
        struct nk_command_circle* c = (struct nk_command_circle*)cmd;
        double r = (c->w < c->h ? c->w : c->h) / 2;

        cairo_set_source_rgba(priv->cr, c->color.r / 255.0, c->color.g / 255.0, c->color.b / 255.0, c->color.a / 255.0);
        cairo_set_line_width(priv->cr, c->line_thickness);
        cairo_new_sub_path(priv->cr);
        cairo_arc(priv->cr, c->x + r, c->y + r, r, 0, 2 * M_PI);
        cairo_close_path(priv->cr);
        cairo_paint(priv->cr);
      }
      break;
    case NK_COMMAND_CIRCLE_FILLED:
      {
        struct nk_command_circle_filled* c = (struct nk_command_circle_filled*)cmd;
        double r = (c->w < c->h ? c->w : c->h) / 2;

        cairo_set_source_rgba(priv->cr, c->color.r / 255.0, c->color.g / 255.0, c->color.b / 255.0, c->color.a / 255.0);
        cairo_new_sub_path(priv->cr);
        cairo_arc(priv->cr, c->x + r, c->y + r, r, 0, 2 * M_PI);
        cairo_close_path(priv->cr);
        cairo_fill(priv->cr);
      }
      break;
    case NK_COMMAND_RECT:
      {
        struct nk_command_rect* c = (struct nk_command_rect*)cmd;
        double degrees = M_PI / 180.0;

        cairo_set_source_rgba(priv->cr, c->color.r / 255.0, c->color.g / 255.0, c->color.b / 255.0, c->color.a / 255.0);
        cairo_set_line_width(priv->cr, c->line_thickness);

        if (c->rounding == 0) {
          cairo_rectangle(priv->cr, c->x, c->y, c->w, c->h);
        } else {
          cairo_new_sub_path(priv->cr);
          cairo_arc(priv->cr, c->x + c->w - c->rounding, c->y + c->rounding, c->rounding, -90 * degrees, 0 * degrees);
          cairo_arc(priv->cr, c->x + c->w - c->rounding, c->y + c->h - c->rounding, c->rounding, 0 * degrees, 90 * degrees);
          cairo_arc(priv->cr, c->x + c->rounding, c->y + c->h - c->rounding, c->rounding, 90 * degrees, 180 * degrees);
          cairo_arc(priv->cr, c->x + c->rounding, c->y + c->rounding, c->rounding, 180 * degrees, 270 * degrees);
          cairo_close_path(priv->cr);
        }
        cairo_stroke(priv->cr);
      }
      break;
    case NK_COMMAND_RECT_FILLED:
      {
        struct nk_command_rect_filled* c = (struct nk_command_rect_filled*)cmd;
        double degrees = M_PI / 180.0;

        cairo_set_source_rgba(priv->cr, c->color.r / 255.0, c->color.g / 255.0, c->color.b / 255.0, c->color.a / 255.0);

        if (c->rounding == 0) {
          cairo_rectangle(priv->cr, c->x, c->y, c->w, c->h);
        } else {
          cairo_new_sub_path(priv->cr);
          cairo_arc(priv->cr, c->x + c->w - c->rounding, c->y + c->rounding, c->rounding, -90 * degrees, 0 * degrees);
          cairo_arc(priv->cr, c->x + c->w - c->rounding, c->y + c->h - c->rounding, c->rounding, 0 * degrees, 90 * degrees);
          cairo_arc(priv->cr, c->x + c->rounding, c->y + c->h - c->rounding, c->rounding, 90 * degrees, 180 * degrees);
          cairo_arc(priv->cr, c->x + c->rounding, c->y + c->rounding, c->rounding, 180 * degrees, 270 * degrees);
          cairo_close_path(priv->cr);
        }
        cairo_fill(priv->cr);
      }
      break;
    case NK_COMMAND_RECT_MULTI_COLOR:
      g_warning("NK_COMMAND_RECT_MULTI_COLOR is not yet implemented");
      break;
    case NK_COMMAND_TRIANGLE:
      {
        struct nk_command_triangle* c = (struct nk_command_triangle*)cmd;

        cairo_set_source_rgba(priv->cr, c->color.r / 255.0, c->color.g / 255.0, c->color.b / 255.0, c->color.a / 255.0);
        cairo_set_line_width(priv->cr, c->line_thickness);

        cairo_new_sub_path(priv->cr);
        cairo_move_to(priv->cr, c->a.x, c->a.y);
        cairo_line_to(priv->cr, c->b.x, c->b.y);
        cairo_line_to(priv->cr, c->c.x, c->c.y);
        cairo_line_to(priv->cr, c->a.x, c->a.y);
        cairo_close_path(priv->cr);
        cairo_paint(priv->cr);
      }
      break;
    case NK_COMMAND_TRIANGLE_FILLED:
      {
        struct nk_command_triangle_filled* c = (struct nk_command_triangle_filled*)cmd;

        cairo_set_source_rgba(priv->cr, c->color.r / 255.0, c->color.g / 255.0, c->color.b / 255.0, c->color.a / 255.0);

        cairo_new_sub_path(priv->cr);
        cairo_move_to(priv->cr, c->a.x, c->a.y);
        cairo_line_to(priv->cr, c->b.x, c->b.y);
        cairo_line_to(priv->cr, c->c.x, c->c.y);
        cairo_line_to(priv->cr, c->a.x, c->a.y);
        cairo_close_path(priv->cr);
        cairo_fill(priv->cr);
      }
      break;
    case NK_COMMAND_POLYLINE:
      {
        struct nk_command_polyline* c = (struct nk_command_polyline*)cmd;

        cairo_set_source_rgba(priv->cr, c->color.r / 255.0, c->color.g / 255.0, c->color.b / 255.0, c->color.a / 255.0);
        cairo_set_line_width(priv->cr, c->line_thickness);
        cairo_new_sub_path(priv->cr);
        cairo_move_to(priv->cr, c->points[0].x, c->points[0].y);

        for (int i = 0; i < c->point_count; i++) {
          cairo_line_to(priv->cr, c->points[i].x, c->points[i].y);
        }

        cairo_close_path(priv->cr);
        cairo_paint(priv->cr);
      }
      break;
    case NK_COMMAND_CURVE:
      {
        struct nk_command_curve* c = (struct nk_command_curve*)cmd;

        cairo_set_source_rgba(priv->cr, c->color.r / 255.0, c->color.g / 255.0, c->color.b / 255.0, c->color.a / 255.0);
        cairo_set_line_width(priv->cr, c->line_thickness);
        cairo_move_to(priv->cr, c->begin.x, c->begin.y);
        cairo_curve_to(priv->cr, c->ctrl[0].x, c->ctrl[0].y, c->ctrl[1].x, c->ctrl[1].y, c->end.x, c->end.y);
        cairo_stroke(priv->cr);
      }
      break;
    case NK_COMMAND_IMAGE:
      {
        struct nk_command_image* c = (struct nk_command_image*)cmd;

        cairo_set_source_rgba(priv->cr, c->col.r / 255.0, c->col.g / 255.0, c->col.b / 255.0, c->col.a / 255.0);

        cairo_matrix_t matrix;
        cairo_get_matrix(priv->cr, &matrix);

        double xs = cairo_image_surface_get_width(priv->surf) / c->img.w;
        double ys = cairo_image_surface_get_width(priv->surf) / c->img.h;

        cairo_scale(priv->cr, xs, ys);

        cairo_surface_t* surf = cairo_image_surface_create_for_data(
          c->img.handle.ptr, CAIRO_FORMAT_ARGB32, c->img.w, c->img.h,
          cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, c->img.w)
        );
        cairo_set_source_surface(priv->cr, surf, c->x / xs, c->y / ys);
        cairo_paint(priv->cr);
        cairo_set_matrix(priv->cr, &matrix);

        cairo_surface_destroy(surf);
      }
      break;
    case NK_COMMAND_TEXT:
      {
        struct nk_command_text* c = (struct nk_command_text*)cmd;

        cairo_set_source_rgba(
          priv->cr, c->background.r / 255.0, c->background.g / 255.0, c->background.b / 255.0, c->background.a / 255.0
        );
        cairo_rectangle(priv->cr, c->x, c->y, c->w, c->h);
        cairo_fill(priv->cr);

        cairo_set_source_rgba(
          priv->cr, c->foreground.r / 255.0, c->foreground.g / 255.0, c->foreground.b / 255.0, c->foreground.a / 255.0
        );

        NtkPangoLayoutFont* font = ntk_pango_layout_font_copy((NtkPangoLayoutFont*)c->font);
        PangoLayout* layout = PANGO_LAYOUT(font->userdata.ptr);

        pango_cairo_update_layout(priv->cr, layout);
        cairo_move_to(priv->cr, c->x, c->y);
        pango_layout_set_text(layout, c->string, c->length);
        pango_cairo_show_layout(priv->cr, layout);

        ntk_pango_layout_font_free(font);
      }
      break;
    case NK_COMMAND_CUSTOM:
      break;
  }
  return TRUE;
}

static struct nk_user_font* ntk_cairo_renderer_get_font(NtkRenderer* renderer, PangoFontDescription* desc, GError** error) {
  NtkCairoRenderer* self = NTK_CAIRO_RENDERER(renderer);
  NtkCairoRendererPrivate* priv = NTK_CAIRO_RENDERER_PRIVATE(self);

  PangoLayout* layout = pango_cairo_create_layout(priv->cr);
  return ntk_pango_layout_font_new(layout, desc);
}

static void ntk_cairo_renderer_get_size(NtkRenderer* renderer, int* width, int* height) {
  NtkCairoRenderer* self = NTK_CAIRO_RENDERER(renderer);
  NtkCairoRendererPrivate* priv = NTK_CAIRO_RENDERER_PRIVATE(self);

  if (priv->width > 0 && priv->height > 0) {
    if (width) *width = priv->width;
    if (height) *height = priv->height;
  } else {
    if (width) *width = cairo_image_surface_get_width(priv->surf);
    if (height) *height = cairo_image_surface_get_height(priv->surf);
  }
}

static void ntk_cairo_renderer_set_size(NtkRenderer* renderer, int width, int height) {
  NtkCairoRenderer* self = NTK_CAIRO_RENDERER(renderer);
  NtkCairoRendererPrivate* priv = NTK_CAIRO_RENDERER_PRIVATE(self);

  if (priv->cr) cairo_destroy(priv->cr);

  if (priv->width > 0 && priv->height > 0) {
    priv->width = width;
    priv->height = height;

    if (priv->surf) cairo_surface_destroy(priv->surf);
    priv->surf = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);

    g_object_notify_by_pspec(G_OBJECT(self), obj_props[PROP_WIDTH]);
    g_object_notify_by_pspec(G_OBJECT(self), obj_props[PROP_HEIGHT]);
    g_object_notify_by_pspec(G_OBJECT(self), obj_props[PROP_SURFACE]);
  }

  priv->cr = cairo_create(priv->surf);
}

static void ntk_cairo_renderer_constructed(GObject* obj) {
  G_OBJECT_CLASS(ntk_cairo_renderer_parent_class)->constructed(obj);

  NtkCairoRenderer* self = NTK_CAIRO_RENDERER(obj);
  NtkCairoRendererPrivate* priv = NTK_CAIRO_RENDERER_PRIVATE(self);

  priv->pango_ctx = pango_context_new();

  if (priv->width > 0 && priv->height > 0 && priv->surf == NULL) {
    priv->surf = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, priv->width, priv->height);
  }

  if (priv->cr == NULL) priv->cr = cairo_create(priv->surf);
}

static void ntk_cairo_renderer_finalize(GObject* obj) {
  NtkCairoRenderer* self = NTK_CAIRO_RENDERER(obj);
  NtkCairoRendererPrivate* priv = NTK_CAIRO_RENDERER_PRIVATE(self);

  g_clear_object(&priv->pango_ctx);
  g_clear_pointer(&priv->cr, cairo_destroy);
  g_clear_pointer(&priv->surf, cairo_surface_destroy);

  G_OBJECT_CLASS(ntk_cairo_renderer_parent_class)->finalize(obj);
}

static void ntk_cairo_renderer_set_property(GObject* obj, guint prop_id, const GValue* value, GParamSpec* pspec) {
  NtkCairoRenderer* self = NTK_CAIRO_RENDERER(obj);
  NtkCairoRendererPrivate* priv = NTK_CAIRO_RENDERER_PRIVATE(self);

  switch (prop_id) {
    case PROP_WIDTH:
      priv->width = g_value_get_int(value);
      ntk_renderer_set_size(NTK_RENDERER(self), priv->width, priv->height);
      break;
    case PROP_HEIGHT:
      priv->height = g_value_get_int(value);
      ntk_renderer_set_size(NTK_RENDERER(self), priv->width, priv->height);
      break;
    case PROP_SURFACE:
#ifdef CAIRO_HAS_GOBJECT_FUNCTIONS
      priv->surf = g_value_get_boxed(value);
#else
      priv->surf = cairo_image_surface_reference(g_value_get_pointer(value));
#endif
      ntk_renderer_set_size(NTK_RENDERER(self), priv->width, priv->height);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
      break;
  }
}

static void ntk_cairo_renderer_get_property(GObject* obj, guint prop_id, GValue* value, GParamSpec* pspec) {
  NtkCairoRenderer* self = NTK_CAIRO_RENDERER(obj);
  NtkCairoRendererPrivate* priv = NTK_CAIRO_RENDERER_PRIVATE(self);

  switch (prop_id) {
    case PROP_WIDTH:
      g_value_set_int(value, priv->width);
      break;
    case PROP_HEIGHT:
      g_value_set_int(value, priv->height);
      break;
    case PROP_SURFACE:
#ifdef CAIRO_HAS_GOBJECT_FUNCTIONS
      g_value_set_boxed(value, priv->surf);
#else
      g_value_set_pointer(value, priv->surf);
#endif
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
      break;
  }
}

static void ntk_cairo_renderer_class_init(NtkCairoRendererClass* klass) {
  GObjectClass* object_class = G_OBJECT_CLASS(klass);
  NtkRendererClass* renderer_class = NTK_RENDERER_CLASS(klass);

  object_class->constructed = ntk_cairo_renderer_constructed;
  object_class->finalize = ntk_cairo_renderer_finalize;

  object_class->set_property = ntk_cairo_renderer_set_property;
  object_class->get_property = ntk_cairo_renderer_get_property;

  obj_props[PROP_WIDTH] =
    g_param_spec_int("width", "Width", "The width to render at.", 0, G_MAXINT, 0, G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
  obj_props[PROP_HEIGHT] =
    g_param_spec_int("height", "Height", "The height to render at.", 0, G_MAXINT, 0, G_PARAM_CONSTRUCT | G_PARAM_READWRITE);

#ifdef CAIRO_HAS_GOBJECT_FUNCTIONS
  obj_props[PROP_SURFACE] = g_param_spec_boxed(
    "surface", "Cairo Surface", "The Cairo surface to render onto", CAIRO_GOBJECT_TYPE_SURFACE,
    G_PARAM_CONSTRUCT | G_PARAM_READWRITE
  );
#else
  obj_props[PROP_SURFACE] = g_param_spec_pointer(
    "surface", "Cairo Surface", "The Cairo surface to render onto", G_PARAM_CONSTRUCT | G_PARAM_READWRITE
  );
#endif

  g_object_class_install_properties(object_class, N_PROPERTIES, obj_props);

  renderer_class->get_render_type = ntk_cairo_renderer_get_render_type;
  renderer_class->render_command = ntk_cairo_renderer_render_command;
  renderer_class->get_font = ntk_cairo_renderer_get_font;
  renderer_class->get_size = ntk_cairo_renderer_get_size;
  renderer_class->set_size = ntk_cairo_renderer_set_size;
}

static void ntk_cairo_renderer_init(NtkCairoRenderer* self) {
  self->priv = ntk_cairo_renderer_get_instance_private(self);
}

NtkRenderer* ntk_cairo_renderer_new(int width, int height) {
  return NTK_RENDERER(g_object_new(NTK_CAIRO_TYPE_RENDERER, "width", width, "height", height, NULL));
}

NtkRenderer* ntk_cairo_renderer_new_for_surface(cairo_surface_t* surf) {
  return NTK_RENDERER(g_object_new(NTK_CAIRO_TYPE_RENDERER, "surface", surf, NULL));
}

void ntk_cairo_renderer_set_width(NtkCairoRenderer* self, int value) {
  g_return_if_fail(NTK_CAIRO_IS_RENDERER(self));
  NtkCairoRendererPrivate* priv = NTK_CAIRO_RENDERER_PRIVATE(self);

  if (priv->cr) cairo_destroy(priv->cr);

  if (priv->width > 0 && priv->height > 0) {
    priv->width = value;

    if (priv->surf) cairo_surface_destroy(priv->surf);
    priv->surf = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, priv->width, priv->height);

    g_object_notify_by_pspec(G_OBJECT(self), obj_props[PROP_WIDTH]);
    g_object_notify_by_pspec(G_OBJECT(self), obj_props[PROP_SURFACE]);
  }

  priv->cr = cairo_create(priv->surf);
}

int ntk_cairo_renderer_get_width(NtkCairoRenderer* self) {
  g_return_val_if_fail(NTK_CAIRO_IS_RENDERER(self), -1);
  NtkCairoRendererPrivate* priv = NTK_CAIRO_RENDERER_PRIVATE(self);

  if (priv->width > 0 && priv->height > 0) {
    return priv->width;
  }
  return cairo_image_surface_get_width(priv->surf);
}

void ntk_cairo_renderer_set_height(NtkCairoRenderer* self, int value) {
  g_return_if_fail(NTK_CAIRO_IS_RENDERER(self));
  NtkCairoRendererPrivate* priv = NTK_CAIRO_RENDERER_PRIVATE(self);

  if (priv->cr) cairo_destroy(priv->cr);

  if (priv->width > 0 && priv->height > 0) {
    priv->height = value;

    if (priv->surf) cairo_surface_destroy(priv->surf);
    priv->surf = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, priv->width, priv->height);

    g_object_notify_by_pspec(G_OBJECT(self), obj_props[PROP_HEIGHT]);
    g_object_notify_by_pspec(G_OBJECT(self), obj_props[PROP_SURFACE]);
  }

  priv->cr = cairo_create(priv->surf);
}

int ntk_cairo_renderer_get_height(NtkCairoRenderer* self) {
  g_return_val_if_fail(NTK_CAIRO_IS_RENDERER(self), -1);
  NtkCairoRendererPrivate* priv = NTK_CAIRO_RENDERER_PRIVATE(self);

  if (priv->width > 0 && priv->height > 0) {
    return priv->height;
  }
  return cairo_image_surface_get_height(priv->surf);
}

void ntk_cairo_renderer_set_surface(NtkCairoRenderer* self, cairo_surface_t* value) {
  g_return_if_fail(NTK_CAIRO_IS_RENDERER(self));
  NtkCairoRendererPrivate* priv = NTK_CAIRO_RENDERER_PRIVATE(self);

  g_clear_pointer(&priv->surf, cairo_surface_destroy);
  priv->surf = cairo_surface_reference(value);

  g_clear_pointer(&priv->cr, cairo_destroy);
  priv->cr = cairo_create(priv->surf);

  g_object_notify_by_pspec(G_OBJECT(self), obj_props[PROP_SURFACE]);
}

cairo_surface_t* ntk_cairo_renderer_get_surface(NtkCairoRenderer* self) {
  g_return_val_if_fail(NTK_CAIRO_IS_RENDERER(self), NULL);
  NtkCairoRendererPrivate* priv = NTK_CAIRO_RENDERER_PRIVATE(self);
  return priv->surf;
}
