#define G_LOG_DOMAIN "NtkEGLRenderer"
#include <ntk/backend/egl/renderer.h>
#include <ntk/font.h>
#include "error-priv.h"
#include "renderer-priv.h"

#define NTK_EGL_RENDERER_PRIVATE(self) ((self)->priv == NULL ? ntk_egl_renderer_get_instance_private(self) : (self)->priv)

enum {
  PROP_0,
  PROP_EGL_DISPLAY,
  N_PROPERTIES,
};

typedef struct {
  float pos[2];
  float uv[2];
  float col[4];
} NtkEGLVertex;

static GParamSpec* obj_props[N_PROPERTIES] = { NULL, };

G_DEFINE_TYPE_WITH_PRIVATE(NtkEGLRenderer, ntk_egl_renderer, NTK_TYPE_RENDERER);

static const struct nk_draw_vertex_layout_element ntk_egl_vertex_layout[] = {
  { NK_VERTEX_POSITION, NK_FORMAT_FLOAT, NK_OFFSETOF(NtkEGLVertex, pos) },
  { NK_VERTEX_TEXCOORD, NK_FORMAT_FLOAT, NK_OFFSETOF(NtkEGLVertex, uv) },
  { NK_VERTEX_COLOR, NK_FORMAT_R32G32B32A32_FLOAT, NK_OFFSETOF(NtkEGLVertex, col) },
  { NK_VERTEX_LAYOUT_END },
};

static const char* ntk_egl_renderer_error_stringify(EGLint error) {
  switch (error) {
    case EGL_SUCCESS: return "EGL_SUCCESS";
    case EGL_NOT_INITIALIZED: return "EGL_NOT_INITIALIZED";
    case EGL_BAD_ACCESS: return "EGL_BAD_ACCESS";
    case EGL_BAD_ALLOC: return "EGL_BAD_ALLOC";
    case EGL_BAD_ATTRIBUTE: return "EGL_BAD_ATTRIBUTE";
    case EGL_BAD_CONTEXT: return "EGL_BAD_CONTEXT";
    case EGL_BAD_CONFIG: return "EGL_BAD_CONFIG";
    case EGL_BAD_CURRENT_SURFACE: return "EGL_BAD_CURRENT_SURFACE";
    case EGL_BAD_DISPLAY: return "EGL_BAD_DISPLAY";
    case EGL_BAD_DEVICE_EXT: return "EGL_BAD_DEVICE_EXT";
    case EGL_BAD_SURFACE: return "EGL_BAD_SURFACE";
    case EGL_BAD_MATCH: return "EGL_BAD_MATCH";
    case EGL_BAD_PARAMETER: return "EGL_BAD_PARAMETER";
    case EGL_BAD_NATIVE_PIXMAP: return "EGL_BAD_NATIVE_PIXMAP";
    case EGL_BAD_NATIVE_WINDOW: return "EGL_BAD_NATIVE_WINDOW";
    case EGL_CONTEXT_LOST: return "EGL_CONTEXT_LOST";
  }
  return "unknown error";
}


static gboolean ntk_egl_renderer_has_ext(const char* exts, const char* ext) {
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

static void* ntk_egl_renderer_load_proc_handler(const char* name, GError** error) {
  void* proc = (void*)eglGetProcAddress("eglGetPlatformDisplayEXT");
  if (proc == NULL) {
    ntk_egl_error_set_bad_proc(error, "proc address is NULL", name);
    return NULL;
  }

  g_debug("Loaded EGL proc \"%s\" with address %p", name, proc);
  return proc;
}

static void ntk_egl_renderer_log(EGLenum error, const char* command, EGLint msg_type, EGLLabelKHR thread, EGLLabelKHR obj, const char* msg) {
  g_critical("EGL command: %s, error: %s (0x%x): %s", command, ntk_egl_renderer_error_stringify(error), error, msg);
}

#define ntk_egl_renderer_load_proc(priv, name, error) ((priv)->procs.name = ntk_egl_renderer_load_proc_handler(#name, error))

static EGLDeviceEXT ntk_egl_renderer_get_egl_device_from_drm(NtkEGLRenderer* self, GError** error) {
  NtkEGLRendererPrivate* priv = NTK_EGL_RENDERER_PRIVATE(self);

  EGLint nb_devices = 0;
  if (!priv->procs.eglQueryDevicesEXT(0, NULL, &nb_devices)) {
    g_error("Failed to query EGL devices");
  }

  EGLDeviceEXT* devices = calloc(nb_devices, sizeof(EGLDeviceEXT));
  if (devices == NULL) {
    g_error("Failed to allocate EGL device list");
  }

  if (!priv->procs.eglQueryDevicesEXT(nb_devices, devices, &nb_devices)) {
    free(devices);
    g_error("Failed to query EGL devices");
  }

  free(devices);
}

static NtkRendererType ntk_egl_renderer_get_render_type(NtkRenderer* renderer) {
  (void)renderer;
  return NTK_RENDERER_TYPE_VERTEX;
}

static gboolean ntk_egl_renderer_configure_vertex(NtkRenderer* renderer, struct nk_convert_config* cfg, GError** error) {
  NtkEGLRenderer* self = NTK_EGL_RENDERER(renderer);
  NtkEGLRendererPrivate* priv = NTK_EGL_RENDERER_PRIVATE(self);

  cfg->shape_AA = NK_ANTI_ALIASING_ON;
  cfg->line_AA = NK_ANTI_ALIASING_ON;
  cfg->vertex_layout = ntk_egl_vertex_layout;
  cfg->vertex_size = sizeof (NtkEGLVertex);
  cfg->vertex_alignment = NK_ALIGNOF(NtkEGLVertex);
  cfg->circle_segment_count = 22;
  cfg->curve_segment_count = 22;
  cfg->arc_segment_count = 22;
  cfg->global_alpha = 1.0f;
  return TRUE;
}

static gboolean ntk_egl_renderer_render_vertex(NtkRenderer* renderer, NtkRendererVertexCommand* cmd, GError** error) {
  NtkEGLRenderer* self = NTK_EGL_RENDERER(renderer);
  NtkEGLRendererPrivate* priv = NTK_EGL_RENDERER_PRIVATE(self);
  return TRUE;
}

static struct nk_user_font* ntk_egl_renderer_get_font(NtkRenderer* renderer, PangoFontDescription* desc, GError** error) {
  NtkEGLRenderer* self = NTK_EGL_RENDERER(renderer);
  NtkEGLRendererPrivate* priv = NTK_EGL_RENDERER_PRIVATE(self);

  // TODO: figure out how to do this with pango
  struct nk_font_config cfg = nk_font_config(pango_font_description_get_size(desc) / PANGO_SCALE);
  cfg.merge_mode = FALSE;
  cfg.range = nk_font_default_glyph_ranges();
  cfg.coord_type = NK_COORD_PIXEL;

  priv->atlas.font_num = 1;
  struct nk_font* font = nk_font_atlas_add_default(&priv->atlas, pango_font_description_get_size(desc) / PANGO_SCALE, &cfg);

  int img_width;
  int img_height;
  const void* img = nk_font_atlas_bake(&priv->atlas, &img_width, &img_height, NK_FONT_ATLAS_RGBA32);
  (void)img;

  // TODO: insert into our font map<string(desc), void*(img)>
  nk_font_atlas_end(&priv->atlas, nk_handle_id(1), 0);
  return &font->handle;
}

static void ntk_egl_renderer_constructed(GObject* obj) {
  G_OBJECT_CLASS(ntk_egl_renderer_parent_class)->constructed(obj);

  NtkEGLRenderer* self = NTK_EGL_RENDERER(obj);
  NtkEGLRendererPrivate* priv = NTK_EGL_RENDERER_PRIVATE(self);

  nk_font_atlas_init_default(&priv->atlas);
  nk_font_atlas_begin(&priv->atlas);

  GError* error = NULL;

  const char* client_exts = eglQueryString(EGL_NO_DISPLAY, EGL_EXTENSIONS);
  if (client_exts == NULL) {
    if (eglGetError() == EGL_BAD_DISPLAY) {
      g_error("EGL_EXT_client_extensions isn't supported");
    } else {
      g_error("Failed to query EGL client extensions");
    }
  }

#define HAS_EXT(name) ntk_egl_renderer_has_ext(client_exts, name)

  g_debug("Queried extensions: %s", client_exts);

  if (!HAS_EXT("EGL_EXT_platform_base")) {
    g_error("EGL_EXT_platform_base isn't supported");
  }

  if (ntk_egl_renderer_load_proc(priv, eglGetPlatformDisplayEXT, &error) == NULL) {
    g_error("%s:%d: %s", g_quark_to_string(error->domain), error->code, error->message);
  }

  priv->exts.KHR_platform_gbm = HAS_EXT("EGL_KHR_platform_gbm");
  priv->exts.EXT_platform_device = HAS_EXT("EGL_EXT_platform_device");

  if (HAS_EXT("EGL_EXT_device_base") || HAS_EXT("EGL_EXT_device_enumeration")) {
    if (ntk_egl_renderer_load_proc(priv, eglQueryDevicesEXT, &error) == NULL) {
      g_error("%s:%d: %s", g_quark_to_string(error->domain), error->code, error->message);
    }
  }

  if (HAS_EXT("EGL_EXT_device_base") || HAS_EXT("EGL_EXT_device_query")) {
    priv->exts.EXT_device_query = TRUE;

    if (ntk_egl_renderer_load_proc(priv, eglQueryDeviceStringEXT, &error) == NULL) {
      g_error("%s:%d: %s", g_quark_to_string(error->domain), error->code, error->message);
    }

    if (ntk_egl_renderer_load_proc(priv, eglQueryDisplayAttribEXT, &error) == NULL) {
      g_error("%s:%d: %s", g_quark_to_string(error->domain), error->code, error->message);
    }
  }

  if (HAS_EXT("EGL_KHR_debug")) {
    if (ntk_egl_renderer_load_proc(priv, eglDebugMessageControlKHR, &error) == NULL) {
      g_error("%s:%d: %s", g_quark_to_string(error->domain), error->code, error->message);
    }

    static const EGLAttrib debug_attribs[] = {
      EGL_DEBUG_MSG_CRITICAL_KHR, EGL_TRUE,
      EGL_DEBUG_MSG_ERROR_KHR, EGL_TRUE,
      EGL_DEBUG_MSG_WARN_KHR, EGL_TRUE,
      EGL_DEBUG_MSG_INFO_KHR, EGL_TRUE,
      EGL_NONE
		};

    priv->procs.eglDebugMessageControlKHR(ntk_egl_renderer_log, debug_attribs);
  }

  if (eglBindAPI(EGL_OPENGL_ES_API) == EGL_FALSE) {
    g_error("Failed to bind OpenGL ES API");
  }

  if (priv->exts.EXT_platform_device) {
    g_debug("Getting DRM device");

    EGLDeviceEXT dev = ntk_egl_renderer_get_egl_device_from_drm(self, &error);
    if (dev == EGL_NO_DEVICE_EXT) {
      g_error("%s:%d: %s", g_quark_to_string(error->domain), error->code, error->message);
    }
  }

#undef HAS_EXT
}

static void ntk_egl_renderer_finalize(GObject* obj) {
  NtkEGLRenderer* self = NTK_EGL_RENDERER(obj);
  NtkEGLRendererPrivate* priv = NTK_EGL_RENDERER_PRIVATE(self);

  if (priv->atlas.pixel != NULL) {
    nk_font_atlas_clear(&priv->atlas);
  }

  G_OBJECT_CLASS(ntk_egl_renderer_parent_class)->finalize(obj);
}

static void ntk_egl_renderer_set_property(GObject* obj, guint prop_id, const GValue* value, GParamSpec* pspec) {
  NtkEGLRenderer* self = NTK_EGL_RENDERER(obj);
  NtkEGLRendererPrivate* priv = NTK_EGL_RENDERER_PRIVATE(self);

  switch (prop_id) {
    case PROP_EGL_DISPLAY:
      priv->egl_display = g_value_get_pointer(value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
      break;
  }
}

static void ntk_egl_renderer_get_property(GObject* obj, guint prop_id, GValue* value, GParamSpec* pspec) {
  NtkEGLRenderer* self = NTK_EGL_RENDERER(obj);
  NtkEGLRendererPrivate* priv = NTK_EGL_RENDERER_PRIVATE(self);

  switch (prop_id) {
    case PROP_EGL_DISPLAY:
      g_value_set_pointer(value, priv->egl_display);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
      break;
  }
}

static void ntk_egl_renderer_class_init(NtkEGLRendererClass* klass) {
  GObjectClass* object_class = G_OBJECT_CLASS(klass);
  NtkRendererClass* renderer_class = NTK_RENDERER_CLASS(klass);

  object_class->constructed = ntk_egl_renderer_constructed;
  object_class->finalize = ntk_egl_renderer_finalize;

  object_class->set_property = ntk_egl_renderer_set_property;
  object_class->get_property = ntk_egl_renderer_get_property;

  obj_props[PROP_EGL_DISPLAY] = g_param_spec_pointer("egl-display", "EGL Display", "The EGL Display Snapshot to render onto.", G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  g_object_class_install_properties(object_class, N_PROPERTIES, obj_props);

  renderer_class->get_render_type = ntk_egl_renderer_get_render_type;
  renderer_class->configure_vertex = ntk_egl_renderer_configure_vertex;
  renderer_class->render_vertex = ntk_egl_renderer_render_vertex;
  renderer_class->get_font = ntk_egl_renderer_get_font;
}

static void ntk_egl_renderer_init(NtkEGLRenderer* self) {
  self->priv = ntk_egl_renderer_get_instance_private(self);
}

NtkRenderer* ntk_egl_renderer_new() {
  return NTK_RENDERER(g_object_new(NTK_EGL_TYPE_RENDERER, NULL));
}

EGLDisplay* ntk_egl_renderer_get_display(NtkEGLRenderer* self) {
  g_return_val_if_fail(NTK_EGL_IS_RENDERER(self), NULL);
  NtkEGLRendererPrivate* priv = NTK_EGL_RENDERER_PRIVATE(self);
  return priv->egl_display;
}
