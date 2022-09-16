#include <ntk/utils.h>

void ntk_command_log(
  const gchar* domain, GLogLevelFlags log_level, const gchar* file, const gchar* line, const gchar* func,
  const struct nk_command* cmd
) {
#define LOG(t, fmt, ...)                                                                                                    \
  g_log_structured_standard(domain, log_level, file, line, func, "NK_COMMAND_" #t "(%d): " fmt, cmd->type, __VA_ARGS__)
  switch (cmd->type) {
    case NK_COMMAND_NOP:
      g_log_structured(domain, log_level, file, line, func, "NK_COMMAND_NOP(%d)", cmd->type);
      break;
    case NK_COMMAND_SCISSOR:
      {
        struct nk_command_scissor* c = (struct nk_command_scissor*)cmd;
        LOG(SCISSOR, "x: %d, y: %d, w: %d, h: %d", c->x, c->y, c->w, c->h);
      }
      break;
    case NK_COMMAND_LINE:
      {
        struct nk_command_line* c = (struct nk_command_line*)cmd;
        LOG(
          LINE, "line thickness: %d, begin: (%d, %d), end: (%d, %d), color: (r: %d, g: %d, b: %d, a: %d)", c->line_thickness,
          c->begin.x, c->begin.y, c->end.x, c->end.y, c->color.r, c->color.g, c->color.b, c->color.a
        );
      }
      break;
    case NK_COMMAND_POLYGON:
      {
        struct nk_command_polygon* c = (struct nk_command_polygon*)cmd;
        LOG(
          POLYGON, "line thickness: %d, point count: %d, color: (r: %d, g: %d, b: %d, a: %d)", c->line_thickness,
          c->point_count, c->color.r, c->color.g, c->color.b, c->color.a
        );
      }
      break;
    case NK_COMMAND_POLYGON_FILLED:
      {
        struct nk_command_polygon_filled* c = (struct nk_command_polygon_filled*)cmd;
        LOG(
          POLYGON_FILLED, "point count: %d, color: (r: %d, g: %d, b: %d, a: %d)", c->point_count, c->color.r, c->color.g,
          c->color.b, c->color.a
        );
      }
      break;
    case NK_COMMAND_ARC:
      {
        struct nk_command_arc* c = (struct nk_command_arc*)cmd;
        LOG(
          ARC, "cx: %d, cy: %d, line thickness: %d, r: %hu, a0: %f, a1: %f, color: (r: %d, g: %d, b: %d, a: %d)", c->cx,
          c->cy, c->line_thickness, c->r, c->a[0], c->a[1], c->color.r, c->color.g, c->color.b, c->color.a
        );
      }
      break;
    case NK_COMMAND_ARC_FILLED:
      {
        struct nk_command_arc_filled* c = (struct nk_command_arc_filled*)cmd;
        LOG(
          ARC_FILLED, "cx: %d, cy: %d, r: %hu, a: [%f, %f], color: (r: %d, g: %d, b: %d, a: %d)", c->cx, c->cy, c->r,
          c->a[0], c->a[1], c->color.r, c->color.g, c->color.b, c->color.a
        );
      }
      break;
    case NK_COMMAND_CIRCLE:
      {
        struct nk_command_circle* c = (struct nk_command_circle*)cmd;
        LOG(
          CIRCLE, "x: %d, y: %d, w: %d, h: %d, line thickness: %d, color: (r: %d, g: %d, b: %d, a: %d)", c->x, c->y, c->w,
          c->h, c->line_thickness, c->color.r, c->color.g, c->color.b, c->color.a
        );
      }
      break;
    case NK_COMMAND_CIRCLE_FILLED:
      {
        struct nk_command_circle_filled* c = (struct nk_command_circle_filled*)cmd;
        LOG(
          CIRCLE_FILLED, "x: %d, y: %d, w: %d, h: %d, color: (r: %d, g: %d, b: %d, a: %d)", c->x, c->y, c->w, c->h,
          c->color.r, c->color.g, c->color.b, c->color.a
        );
      }
      break;
    case NK_COMMAND_RECT:
      {
        struct nk_command_rect* c = (struct nk_command_rect*)cmd;
        LOG(
          RECT_FILLED, "rounding: %d, line thickness: %d, x: %d, y: %d, w: %d, h: %d, color: (r: %d, g: %d, b: %d, a: %d)",
          c->rounding, c->line_thickness, c->x, c->y, c->w, c->h, c->color.r, c->color.g, c->color.b, c->color.a
        );
      }
      break;
    case NK_COMMAND_RECT_FILLED:
      {
        struct nk_command_rect_filled* c = (struct nk_command_rect_filled*)cmd;
        LOG(
          RECT_FILLED, "rounding: %d, x: %d, y: %d, w: %d, h: %d, color: (r: %d, g: %d, b: %d, a: %d)", c->rounding, c->x,
          c->y, c->w, c->h, c->color.r, c->color.g, c->color.b, c->color.a
        );
      }
      break;
    case NK_COMMAND_RECT_MULTI_COLOR:
      {
        struct nk_command_rect_multi_color* c = (struct nk_command_rect_multi_color*)cmd;
        LOG(
          RECT_MULTI_COLOR,
          "x: %d, y: %d, w: %d, h: %d, top: (r: %d, g: %d, b: %d, a: %d), bottom: (r: %d, g: %d, b: %d, a: %d), left: (r: "
          "%d, g: %d, b: %d, a: %d), right: (r: %d, g: %d, b: %d, a: %d)",
          c->x, c->y, c->w, c->h, c->top.r, c->top.g, c->top.b, c->top.a, c->bottom.r, c->bottom.g, c->bottom.b, c->bottom.a,
          c->left.r, c->left.g, c->left.b, c->left.a, c->right.r, c->right.g, c->right.b, c->right.a
        );
      }
      break;
    case NK_COMMAND_TRIANGLE:
      {
        struct nk_command_triangle* c = (struct nk_command_triangle*)cmd;
        LOG(
          TRIANGLE, "line thickness: %d, a: (%d, %d), b: (%d, %d), c: (%d, %d), color: (r: %d, g: %d, b: %d, a: %d)",
          c->line_thickness, c->a.x, c->a.y, c->b.x, c->b.y, c->c.x, c->c.y, c->color.r, c->color.g, c->color.b, c->color.a
        );
      }
      break;
    case NK_COMMAND_TRIANGLE_FILLED:
      {
        struct nk_command_triangle_filled* c = (struct nk_command_triangle_filled*)cmd;
        LOG(
          TRIANGLE_FILLED, "a: (%d, %d), b: (%d, %d), c: (%d, %d), color: (r: %d, g: %d, b: %d, a: %d)", c->a.x, c->a.y,
          c->b.x, c->b.y, c->c.x, c->c.y, c->color.r, c->color.g, c->color.b, c->color.a
        );
      }
      break;
    case NK_COMMAND_POLYLINE:
      {
        struct nk_command_polyline* c = (struct nk_command_polyline*)cmd;
        LOG(
          POLYLINE, "line thickness: %d, point count: %d, color: (r: %d, g: %d, b: %d, a: %d)", c->line_thickness,
          c->point_count, c->color.r, c->color.g, c->color.b, c->color.a
        );
      }
      break;
    case NK_COMMAND_CURVE:
      {
        struct nk_command_curve* c = (struct nk_command_curve*)cmd;
        LOG(
          CURVE,
          "line thickness: %d, begin: (%d, %d), end: (%d, %d), ctrl: [(%d, %d), (%d, %d)], color: (r: %d, g: %d, b: %d, a: "
          "%d)",
          c->line_thickness, c->begin.x, c->begin.y, c->end.x, c->end.y, c->ctrl[0].x, c->ctrl[0].y, c->ctrl[1].x,
          c->ctrl[1].y, c->color.r, c->color.g, c->color.b, c->color.a
        );
      }
      break;
    case NK_COMMAND_IMAGE:
      {
        struct nk_command_image* c = (struct nk_command_image*)cmd;
        LOG(
          IMAGE, "image: (handle: %p, w: %d, h: %d), x: %d, y: %d, color: (r: %d, g: %d, b: %d, a: %d)", c->img.handle.ptr,
          c->img.w, c->img.h, c->x, c->y, c->col.r, c->col.g, c->col.b, c->col.a
        );
      }
      break;
    case NK_COMMAND_TEXT:
      {
        struct nk_command_text* c = (struct nk_command_text*)cmd;
        LOG(
          TEXT,
          "string: (\"%s\", %d), height: %f, w: %d, h: %d, x: %d, y: %d, background: (r: %d, g: %d, b: %d, a: %d), "
          "foreground: (r: %d, g: %d, b: %d, a: %d)",
          c->string, c->length, c->height, c->w, c->h, c->x, c->y, c->background.r, c->background.g, c->background.b,
          c->background.a, c->foreground.r, c->foreground.g, c->foreground.b, c->foreground.a
        );
      }
      break;
    case NK_COMMAND_CUSTOM:
      {
        struct nk_command_custom* c = (struct nk_command_custom*)cmd;
        LOG(
          CUSTOM, "x: %d, y: %d, w: %d, h: %d, callback: %p, callback data: %p", c->x, c->y, c->w, c->h, c->callback,
          c->callback_data.ptr
        );
      }
      break;
  }
}
