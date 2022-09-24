#include "error-priv.h"
#include "renderer-priv.h"
#include "shaders-priv.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <gio/gio.h>
#include <ntk-gles2-resources.h>
#include <ntk/backend/gles2/font.h>
#include <ntk/backend/gles2/renderer.h>

#define NTK_GLES2_RENDERER_PRIVATE(self)                                                                                    \
  ((self)->priv == NULL ? ntk_gles2_renderer_get_instance_private(self) : (self)->priv)

enum {
  PROP_0,
  PROP_PROC_LOADER,
  N_PROPERTIES,
};

enum {
  SIG_RENDER,
  N_SIGNALS
};

typedef struct {
  float pos[2];
  float uv[2];
  float col[4];
} NtkGLES2Vertex;

static GParamSpec* obj_props[N_PROPERTIES] = {
  NULL,
};
static guint obj_sigs[N_SIGNALS] = {0};

static void ntk_gles2_renderer_interface_init(GInitableIface* iface);

G_DEFINE_TYPE_WITH_CODE(
  NtkGLES2Renderer, ntk_gles2_renderer, NTK_TYPE_RENDERER,
  G_ADD_PRIVATE(NtkGLES2Renderer) G_IMPLEMENT_INTERFACE(G_TYPE_INITABLE, ntk_gles2_renderer_interface_init)
);

static const struct nk_draw_vertex_layout_element ntk_gles2_vertex_layout[] = {
  {NK_VERTEX_POSITION, NK_FORMAT_FLOAT, NK_OFFSETOF(NtkGLES2Vertex, pos)},
  {NK_VERTEX_TEXCOORD, NK_FORMAT_FLOAT, NK_OFFSETOF(NtkGLES2Vertex, uv)},
  {NK_VERTEX_COLOR, NK_FORMAT_R32G32B32A32_FLOAT, NK_OFFSETOF(NtkGLES2Vertex, col)},
  {NK_VERTEX_LAYOUT_END},
};

static gboolean ntk_gles2_renderer_has_ext(NtkGLES2Renderer* self, const char* ext) {
  NtkGLES2RendererPrivate* priv = NTK_GLES2_RENDERER_PRIVATE(self);
  const gchar* exts = priv->exts_str;

  size_t extlen = strlen(ext);
  const char* end = exts + strlen(exts);
  while (exts < end) {
    if (*exts == ' ') {
      exts++;
      continue;
    }
    size_t n = strcspn(exts, " ");
    if (n == extlen && strncmp(ext, exts, n) == 0) return TRUE;
    exts += n;
  }
  return FALSE;
}

static void ntk_gles2_renderer_log(
  GLenum src, GLenum type, GLuint id, GLenum severity, GLsizei len, const GLchar* msg, const void* user
) {
  switch (type) {
    case GL_DEBUG_TYPE_ERROR_KHR:
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_KHR:
      g_critical("GLES2: %s", msg);
      break;
    default:
      g_debug("GLES2: %s", msg);
      break;
  }
}

static void* ntk_gles2_renderer_load_proc_handler(NtkGLES2Renderer* self, const char* name, GError** error) {
  NtkGLES2RendererPrivate* priv = NTK_GLES2_RENDERER_PRIVATE(self);

  void* proc = priv->proc_handler(name);
  if (proc == NULL) {
    ntk_gles2_error_set_bad_proc(error, "proc address is NULL", name);
    return NULL;
  }

  g_debug("Loaded GL proc \"%s\" with address %p", name, proc);
  return proc;
}

#define ntk_gles2_renderer_load_proc(self, name, error)                                                                     \
  ((self)->priv->procs.name = ntk_gles2_renderer_load_proc_handler(self, #name, error))

static NtkRendererType ntk_gles2_renderer_get_render_type(NtkRenderer* renderer) {
  (void)renderer;
  return NTK_RENDERER_TYPE_VERTEX;
}

static NtkFont* ntk_gles2_renderer_get_font(NtkRenderer* renderer, gchar* name, int size, GError** error) {
  (void)renderer;

  PangoFontDescription* desc = pango_font_description_from_string(name);
  pango_font_description_set_size(desc, size * PANGO_SCALE);
  return ntk_gles2_font_layout_new(desc);
}

static gboolean ntk_gles2_renderer_configure_vertex(NtkRenderer* renderer, struct nk_convert_config* cfg, GError** error) {
  (void)renderer;
  (void)error;

  cfg->shape_AA = NK_ANTI_ALIASING_ON;
  cfg->line_AA = NK_ANTI_ALIASING_ON;
  cfg->vertex_layout = ntk_gles2_vertex_layout;
  cfg->vertex_size = sizeof(NtkGLES2Vertex);
  cfg->vertex_alignment = NK_ALIGNOF(NtkGLES2Vertex);
  cfg->circle_segment_count = 22;
  cfg->curve_segment_count = 22;
  cfg->arc_segment_count = 22;
  cfg->global_alpha = 1.0f;
  return TRUE;
}

static gboolean ntk_gles2_renderer_render_vertex(NtkRenderer* renderer, NtkRendererVertexCommand* cmd, GError** error) {
  return TRUE;
}

static gboolean ntk_gles2_renderer_pre_render(NtkRenderer* renderer, GError** error) {
  return TRUE;
}

static gboolean ntk_gles2_renderer_post_render(NtkRenderer* renderer, GError** error) {
  return TRUE;
}

static void ntk_gles2_renderer_finalize(GObject* obj) {
  G_OBJECT_CLASS(ntk_gles2_renderer_parent_class)->finalize(obj);
}

static void ntk_gles2_renderer_set_property(GObject* obj, guint prop_id, const GValue* value, GParamSpec* pspec) {
  NtkGLES2Renderer* self = NTK_GLES2_RENDERER(obj);
  NtkGLES2RendererPrivate* priv = NTK_GLES2_RENDERER_PRIVATE(self);

  switch (prop_id) {
    case PROP_PROC_LOADER:
      priv->proc_handler = g_value_get_pointer(value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
      break;
  }
}

static void ntk_gles2_renderer_get_property(GObject* obj, guint prop_id, GValue* value, GParamSpec* pspec) {
  NtkGLES2Renderer* self = NTK_GLES2_RENDERER(obj);
  NtkGLES2RendererPrivate* priv = NTK_GLES2_RENDERER_PRIVATE(self);

  switch (prop_id) {
    case PROP_PROC_LOADER:
      priv->proc_handler = g_value_get_pointer(value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
      break;
  }
}

static gboolean ntk_gles2_renderer_initable_init(GInitable* initable, GCancellable* cancellable, GError** error) {
  NtkGLES2Renderer* self = NTK_GLES2_RENDERER(initable);
  NtkGLES2RendererPrivate* priv = NTK_GLES2_RENDERER_PRIVATE(self);

  priv->exts_str = (const char*)glGetString(GL_EXTENSIONS);
  g_assert(priv->exts_str != NULL);

  g_debug("Queried GL extensions: %s", priv->exts_str);

  if (!ntk_gles2_renderer_has_ext(self, "GL_EXT_texture_format_BGRA8888")) {
    ntk_gles2_error_set_missing_ext(error, "BGRA8888 format not supported by GLES2", "GL_EXT_texture_format_BGRA8888");
    return FALSE;
  }

  if (!ntk_gles2_renderer_has_ext(self, "GL_EXT_unpack_subimage")) {
    ntk_gles2_error_set_missing_ext(error, "extension is not supported", "GL_EXT_unpack_subimage");
    return FALSE;
  }

  priv->exts.EXT_read_format_bgra = ntk_gles2_renderer_has_ext(self, "GL_EXT_read_format_bgra");
  priv->exts.EXT_texture_type_2_10_10_10_REV = ntk_gles2_renderer_has_ext(self, "GL_EXT_texture_type_2_10_10_10_REV");
  priv->exts.OES_texture_half_float_linear = ntk_gles2_renderer_has_ext(self, "GL_OES_texture_half_float_linear");
  priv->exts.EXT_texture_norm16 = ntk_gles2_renderer_has_ext(self, "GL_EXT_texture_norm16");

  if (ntk_gles2_renderer_has_ext(self, "GL_KHR_debug")) {
    priv->exts.KHR_debug = TRUE;
    if (!ntk_gles2_renderer_load_proc(self, glDebugMessageCallbackKHR, error)) return FALSE;
    if (!ntk_gles2_renderer_load_proc(self, glDebugMessageControlKHR, error)) return FALSE;
  }

  if (ntk_gles2_renderer_has_ext(self, "GL_OES_EGL_image_external")) {
    priv->exts.OES_texture_half_float_linear = TRUE;
    if (!ntk_gles2_renderer_load_proc(self, glEGLImageTargetTexture2DOES, error)) return FALSE;
  }

  if (ntk_gles2_renderer_has_ext(self, "GL_OES_EGL_image")) {
    priv->exts.OES_egl_image = TRUE;
    if (!ntk_gles2_renderer_load_proc(self, glEGLImageTargetRenderbufferStorageOES, error)) return FALSE;
  }

  if (priv->exts.KHR_debug) {
    glEnable(GL_DEBUG_OUTPUT_KHR);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_KHR);

    priv->procs.glDebugMessageCallbackKHR(ntk_gles2_renderer_log, NULL);
    priv->procs.glDebugMessageControlKHR(GL_DONT_CARE, GL_DEBUG_TYPE_POP_GROUP_KHR, GL_DONT_CARE, 0, NULL, GL_FALSE);
    priv->procs.glDebugMessageControlKHR(GL_DONT_CARE, GL_DEBUG_TYPE_PUSH_GROUP_KHR, GL_DONT_CARE, 0, NULL, GL_FALSE);
  }

  g_resources_register(ntk_gles2_get_resource());

  GLuint prog = ntk_gles2_link_program_from_resource_path(NTK_GLES2_BASE_SHADERS_PATH "/quad", error);
  if (prog == 0) return FALSE;

  g_debug("GL version: %s", glGetString(GL_VERSION));
  g_debug("GLSL version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
  g_debug("GL vendor: %s", glGetString(GL_VENDOR));
  g_debug("GL renderer: %s", glGetString(GL_RENDERER));
  return TRUE;
}

static void ntk_gles2_renderer_interface_init(GInitableIface* iface) {
  iface->init = ntk_gles2_renderer_initable_init;
}

static void ntk_gles2_renderer_class_init(NtkGLES2RendererClass* klass) {
  GObjectClass* object_class = G_OBJECT_CLASS(klass);
  NtkRendererClass* renderer_class = NTK_RENDERER_CLASS(klass);

  object_class->finalize = ntk_gles2_renderer_finalize;

  object_class->set_property = ntk_gles2_renderer_set_property;
  object_class->get_property = ntk_gles2_renderer_get_property;

  obj_props[PROP_PROC_LOADER] = g_param_spec_pointer(
    "proc-loader", "GL Proc Loader", "The function used for loading any functions for GL which are not already available",
    G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE
  );
  g_object_class_install_properties(object_class, N_PROPERTIES, obj_props);

  renderer_class->get_render_type = ntk_gles2_renderer_get_render_type;
  renderer_class->get_font = ntk_gles2_renderer_get_font;
  renderer_class->configure_vertex = ntk_gles2_renderer_configure_vertex;
  renderer_class->render_vertex = ntk_gles2_renderer_render_vertex;
  renderer_class->pre_render = ntk_gles2_renderer_pre_render;
}

static void ntk_gles2_renderer_init(NtkGLES2Renderer* self) {
  self->priv = ntk_gles2_renderer_get_instance_private(self);
}

NtkRenderer* ntk_gles2_renderer_new(NtkGLES2RendererLoadProcHandler proc_handler, GError** error) {
  return NTK_RENDERER(g_initable_new(NTK_GLES2_TYPE_RENDERER, NULL, error, "proc-loader", proc_handler, NULL));
}
