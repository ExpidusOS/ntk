#include "error-priv.h"
#include "ntk/backend/gles2/renderer.h"
#include "ntk/renderer.h"
#include "renderer-priv.h"
#include <gio/gio.h>
#include <ntk/backend/egl/renderer.h>
#include <ntk/font.h>
#include <ntk/hw.h>

#define NTK_EGL_RENDERER_PRIVATE(self) ((self)->priv == NULL ? ntk_egl_renderer_get_instance_private(self) : (self)->priv)

enum {
  PROP_0,
  PROP_DEVICE,
  PROP_DISPLAY,
  PROP_GL_RENDERER,
  N_PROPERTIES,
};

static GParamSpec* obj_props[N_PROPERTIES] = {
  NULL,
};

static void ntk_egl_renderer_interface_init(GInitableIface* iface);

G_DEFINE_TYPE_WITH_CODE(
  NtkEGLRenderer, ntk_egl_renderer, NTK_TYPE_RENDERER,
  G_ADD_PRIVATE(NtkEGLRenderer) G_IMPLEMENT_INTERFACE(G_TYPE_INITABLE, ntk_egl_renderer_interface_init)
);

static const char* ntk_egl_renderer_error_stringify(EGLint error) {
  switch (error) {
    case EGL_SUCCESS:
      return "EGL_SUCCESS";
    case EGL_NOT_INITIALIZED:
      return "EGL_NOT_INITIALIZED";
    case EGL_BAD_ACCESS:
      return "EGL_BAD_ACCESS";
    case EGL_BAD_ALLOC:
      return "EGL_BAD_ALLOC";
    case EGL_BAD_ATTRIBUTE:
      return "EGL_BAD_ATTRIBUTE";
    case EGL_BAD_CONTEXT:
      return "EGL_BAD_CONTEXT";
    case EGL_BAD_CONFIG:
      return "EGL_BAD_CONFIG";
    case EGL_BAD_CURRENT_SURFACE:
      return "EGL_BAD_CURRENT_SURFACE";
    case EGL_BAD_DISPLAY:
      return "EGL_BAD_DISPLAY";
    case EGL_BAD_DEVICE_EXT:
      return "EGL_BAD_DEVICE_EXT";
    case EGL_BAD_SURFACE:
      return "EGL_BAD_SURFACE";
    case EGL_BAD_MATCH:
      return "EGL_BAD_MATCH";
    case EGL_BAD_PARAMETER:
      return "EGL_BAD_PARAMETER";
    case EGL_BAD_NATIVE_PIXMAP:
      return "EGL_BAD_NATIVE_PIXMAP";
    case EGL_BAD_NATIVE_WINDOW:
      return "EGL_BAD_NATIVE_WINDOW";
    case EGL_CONTEXT_LOST:
      return "EGL_CONTEXT_LOST";
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
  void* proc = eglGetProcAddress(name);
  if (proc == NULL) {
    ntk_egl_error_set_bad_proc(error, "proc address is NULL", name);
    return NULL;
  }

  g_debug("Loaded EGL proc \"%s\" with address %p", name, proc);
  return proc;
}

static void ntk_egl_renderer_log(
  EGLenum error, const char* command, EGLint msg_type, EGLLabelKHR thread, EGLLabelKHR obj, const char* msg
) {
  g_critical("EGL command: %s, error: %s (0x%x): %s", command, ntk_egl_renderer_error_stringify(error), error, msg);
}

#define ntk_egl_renderer_load_proc(priv, name, error) ((priv)->procs.name = ntk_egl_renderer_load_proc_handler(#name, error))

#ifdef NTK_HW_HAS_LIBDRM
static EGLDeviceEXT ntk_egl_renderer_get_egl_device_from_drm(NtkEGLRenderer* self, GError** error) {
  NtkEGLRendererPrivate* priv = NTK_EGL_RENDERER_PRIVATE(self);

  if (!NTK_HW_IS_DRM_RENDER(priv->device)) {
    ntk_egl_error_set_binding(error, "The provided device is not a DRM render device");
    return FALSE;
  }

  EGLint nb_devices = 0;
  if (!priv->procs.eglQueryDevicesEXT(0, NULL, &nb_devices)) {
    ntk_egl_error_set_egl(error, "Failed to query EGL devices", EGL_BAD_PARAMETER);
    return EGL_NO_DEVICE_EXT;
  }

  EGLDeviceEXT* devices = calloc(nb_devices, sizeof(EGLDeviceEXT));
  if (devices == NULL) {
    ntk_egl_error_set_egl(error, "Failed to allocate EGL device list", EGL_BAD_ALLOC);
    return EGL_NO_DEVICE_EXT;
  }

  if (!priv->procs.eglQueryDevicesEXT(nb_devices, devices, &nb_devices)) {
    free(devices);
    ntk_egl_error_set_egl(error, "Failed to query EGL devices", EGL_BAD_PARAMETER);
    return EGL_NO_DEVICE_EXT;
  }

  for (int i = 0; i < nb_devices; i++) {
    const char* egl_device_name = priv->procs.eglQueryDeviceStringEXT(devices[i], EGL_DRM_DEVICE_FILE_EXT);
    if (egl_device_name == NULL) continue;

    if (ntk_hw_drm_render_has_name(NTK_HW_DRM_RENDER(priv->device), egl_device_name)) {
      g_debug("Found device: %s", egl_device_name);
      EGLDeviceEXT egl_device = devices[i];
      free(devices);
      return egl_device;
    }
  }

  free(devices);
  ntk_egl_error_set_egl(error, "No device was found", EGL_BAD_DEVICE_EXT);
  return EGL_NO_DEVICE_EXT;
}
#endif

static gboolean ntk_egl_renderer_begin(NtkEGLRenderer* self, GError** error) {
  NtkEGLRendererPrivate* priv = NTK_EGL_RENDERER_PRIVATE(self);
  if (!eglMakeCurrent(priv->display, EGL_NO_SURFACE, EGL_NO_SURFACE, priv->context)) {
    ntk_egl_error_set_egl(error, "Failed to make the EGL context current", eglGetError());
    return FALSE;
  }
  return TRUE;
}

static gboolean ntk_egl_renderer_end(NtkEGLRenderer* self, GError** error) {
  NtkEGLRendererPrivate* priv = NTK_EGL_RENDERER_PRIVATE(self);
  eglReleaseThread();
  return TRUE;
}

static NtkRendererType ntk_egl_renderer_get_render_type(NtkRenderer* renderer) {
  (void)renderer;
  return NTK_RENDERER_TYPE_VERTEX;
}

static gboolean ntk_egl_renderer_configure_vertex(NtkRenderer* renderer, struct nk_convert_config* cfg, GError** error) {
  NtkEGLRenderer* self = NTK_EGL_RENDERER(renderer);
  NtkEGLRendererPrivate* priv = NTK_EGL_RENDERER_PRIVATE(self);

  g_return_val_if_fail(ntk_egl_renderer_begin(self, error), FALSE);
  gboolean result = ntk_renderer_configure_vertex(NTK_RENDERER(priv->gl_renderer), cfg, error);
  g_return_val_if_fail(ntk_egl_renderer_end(self, error), FALSE);
  return result;
}

static gboolean ntk_egl_renderer_render_vertex(NtkRenderer* renderer, NtkRendererVertexCommand* cmd, GError** error) {
  NtkEGLRenderer* self = NTK_EGL_RENDERER(renderer);
  NtkEGLRendererPrivate* priv = NTK_EGL_RENDERER_PRIVATE(self);

  g_return_val_if_fail(ntk_egl_renderer_begin(self, error), FALSE);

  NtkRendererClass* render_class = NTK_RENDERER_GET_CLASS(priv->gl_renderer);
  g_assert(render_class != NULL);
  g_assert(render_class->render_vertex != NULL);
  gboolean result = render_class->render_vertex(NTK_RENDERER(priv->gl_renderer), cmd, error);

  g_return_val_if_fail(ntk_egl_renderer_end(self, error), FALSE);
  return result;
}

static void ntk_egl_renderer_set_size(NtkRenderer* renderer, int width, int height) {
  NtkEGLRenderer* self = NTK_EGL_RENDERER(renderer);
  NtkEGLRendererPrivate* priv = NTK_EGL_RENDERER_PRIVATE(self);

  GError* error = NULL;
  if (!ntk_egl_renderer_begin(self, &error)) {
    g_error("Failed to begin OpenGL %s:%d: %s", g_quark_to_string(error->domain), error->code, error->message);
    g_return_if_reached();
  }

  ntk_renderer_set_size(NTK_RENDERER(priv->gl_renderer), width, height);

  if (!ntk_egl_renderer_end(self, &error)) {
    g_error("Failed to end OpenGL %s:%d: %s", g_quark_to_string(error->domain), error->code, error->message);
    g_return_if_reached();
  }
}

static void ntk_egl_renderer_get_size(NtkRenderer* renderer, int* width, int* height) {
  NtkEGLRenderer* self = NTK_EGL_RENDERER(renderer);
  NtkEGLRendererPrivate* priv = NTK_EGL_RENDERER_PRIVATE(self);

  GError* error = NULL;
  if (!ntk_egl_renderer_begin(self, &error)) {
    g_error("Failed to begin OpenGL %s:%d: %s", g_quark_to_string(error->domain), error->code, error->message);
    g_return_if_reached();
  }

  ntk_renderer_get_size(NTK_RENDERER(priv->gl_renderer), width, height);

  if (!ntk_egl_renderer_end(self, &error)) {
    g_error("Failed to end OpenGL %s:%d: %s", g_quark_to_string(error->domain), error->code, error->message);
    g_return_if_reached();
  }
}

static NtkFont* ntk_egl_renderer_get_font(NtkRenderer* renderer, gchar* name, int size, GError** error) {
  NtkEGLRenderer* self = NTK_EGL_RENDERER(renderer);
  NtkEGLRendererPrivate* priv = NTK_EGL_RENDERER_PRIVATE(self);

  g_return_val_if_fail(ntk_egl_renderer_begin(self, error), FALSE);

  NtkRendererClass* render_class = NTK_RENDERER_GET_CLASS(priv->gl_renderer);
  g_assert(render_class != NULL);
  g_assert(render_class->get_font != NULL);
  NtkFont* result = render_class->get_font(NTK_RENDERER(priv->gl_renderer), name, size, error);

  g_return_val_if_fail(ntk_egl_renderer_end(self, error), FALSE);
  return result;
}

static void ntk_egl_renderer_finalize(GObject* obj) {
  NtkEGLRenderer* self = NTK_EGL_RENDERER(obj);
  NtkEGLRendererPrivate* priv = NTK_EGL_RENDERER_PRIVATE(self);

  if (priv->display != NULL) {
    if (priv->context != NULL) {
      if (priv->gl_renderer != NULL) {
        GError* error = NULL;
        if (!ntk_egl_renderer_begin(self, &error)) g_error("Failed to begin OpenGL %s:%d: %s", g_quark_to_string(error->domain), error->code, error->message);
        else g_clear_object(&priv->gl_renderer);
        if (!ntk_egl_renderer_end(self, &error)) g_error("Failed to end OpenGL %s:%d: %s", g_quark_to_string(error->domain), error->code, error->message);
      }

      g_debug("Destroying EGL context");
      eglDestroyContext(priv->display, priv->context);
      priv->context = NULL;
    }

    g_debug("Destroying EGL display");
    eglTerminate(priv->display);
    priv->display = NULL;
  }

  if (priv->atlas.pixel != NULL) {
    g_debug("Destroying Nuklear font atlas");
    nk_font_atlas_clear(&priv->atlas);
  }

  g_debug("Destroying device");
  g_clear_object(&priv->device);

  G_OBJECT_CLASS(ntk_egl_renderer_parent_class)->finalize(obj);
}

static void ntk_egl_renderer_set_property(GObject* obj, guint prop_id, const GValue* value, GParamSpec* pspec) {
  NtkEGLRenderer* self = NTK_EGL_RENDERER(obj);
  NtkEGLRendererPrivate* priv = NTK_EGL_RENDERER_PRIVATE(self);

  switch (prop_id) {
    case PROP_DEVICE:
      priv->device = g_value_dup_object(value);
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
    case PROP_GL_RENDERER:
      g_value_set_object(value, priv->gl_renderer);
      break;
    case PROP_DISPLAY:
      g_value_set_pointer(value, priv->display);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
      break;
  }
}

static gboolean ntk_egl_renderer_bootstrap_init(NtkEGLRenderer* self, GError** error) {
  NtkEGLRendererPrivate* priv = NTK_EGL_RENDERER_PRIVATE(self);

  const char* client_exts = eglQueryString(EGL_NO_DISPLAY, EGL_EXTENSIONS);
  if (client_exts == NULL) {
    EGLint e = eglGetError();
    if (e == EGL_BAD_DISPLAY) {
      ntk_egl_error_set_egl(error, "EGL_EXT_client_extensions isn't supported", e);
      return FALSE;
    } else {
      ntk_egl_error_set_egl(error, "Failed to query extensions.", e);
      return FALSE;
    }
  }

#define HAS_EXT(name) ntk_egl_renderer_has_ext(client_exts, name)

  g_debug("Queried base extensions: %s", client_exts);

  if (!HAS_EXT("EGL_EXT_platform_base")) {
    ntk_egl_error_set_missing_ext(error, "extension is not supported", "EGL_EXT_platform_base");
    return FALSE;
  }

  if (ntk_egl_renderer_load_proc(priv, eglGetPlatformDisplayEXT, error) == NULL) return FALSE;

  priv->exts.KHR_platform_gbm = HAS_EXT("EGL_KHR_platform_gbm");
  priv->exts.EXT_platform_device = HAS_EXT("EGL_EXT_platform_device");

  if (HAS_EXT("EGL_EXT_device_base") || HAS_EXT("EGL_EXT_device_enumeration")) {
    if (ntk_egl_renderer_load_proc(priv, eglQueryDevicesEXT, error) == NULL) return FALSE;
  }

  if (HAS_EXT("EGL_EXT_device_base") || HAS_EXT("EGL_EXT_device_query")) {
    priv->exts.EXT_device_query = TRUE;

    if (ntk_egl_renderer_load_proc(priv, eglQueryDeviceStringEXT, error) == NULL) return FALSE;
    if (ntk_egl_renderer_load_proc(priv, eglQueryDisplayAttribEXT, error) == NULL) return FALSE;
  }

  if (HAS_EXT("EGL_KHR_debug")) {
    if (ntk_egl_renderer_load_proc(priv, eglDebugMessageControlKHR, error) == NULL) return FALSE;

    static const EGLAttrib debug_attribs[] = {
      EGL_DEBUG_MSG_CRITICAL_KHR,
      EGL_TRUE,
      EGL_DEBUG_MSG_ERROR_KHR,
      EGL_TRUE,
      EGL_DEBUG_MSG_WARN_KHR,
      EGL_TRUE,
      EGL_DEBUG_MSG_INFO_KHR,
      EGL_TRUE,
      EGL_NONE};

    priv->procs.eglDebugMessageControlKHR(ntk_egl_renderer_log, debug_attribs);
  }
#undef HAS_EXT

  if (eglBindAPI(EGL_OPENGL_ES_API) == EGL_FALSE) {
    ntk_egl_error_set_binding(error, "Failed to bind the OpenGL ES API");
    return FALSE;
  }
  return TRUE;
}

static gboolean ntk_egl_renderer_display_init(NtkEGLRenderer* self, EGLenum platform, void* remote_display, GError** error) {
  NtkEGLRendererPrivate* priv = NTK_EGL_RENDERER_PRIVATE(self);
  priv->display = priv->procs.eglGetPlatformDisplayEXT(platform, remote_display, NULL);
  if (priv->display == EGL_NO_DISPLAY) {
    ntk_egl_error_set_egl(error, "Failed to create EGL display", EGL_BAD_DISPLAY);
    return FALSE;
  }

  EGLint major;
  EGLint minor;
  if (eglInitialize(priv->display, &major, &minor) == EGL_FALSE) {
    ntk_egl_error_set_egl(error, "Failed to initialize EGL", EGL_BAD_CONTEXT);
    return FALSE;
  }

  g_debug("EGL initialized with version: %d.%d", major, minor);

  const char* display_exts = eglQueryString(priv->display, EGL_EXTENSIONS);
  if (display_exts == NULL) {
    ntk_egl_error_set_egl(error, "Failed to query extensions.", eglGetError());
    return FALSE;
  }

#define HAS_EXT(name) ntk_egl_renderer_has_ext(display_exts, name)

  g_debug("Queried display extensions: %s", display_exts);

  if (HAS_EXT("EGL_KHR_image_base")) {
    priv->exts.KHR_image_base = TRUE;

    if (ntk_egl_renderer_load_proc(priv, eglCreateImageKHR, error) == NULL) return FALSE;
    if (ntk_egl_renderer_load_proc(priv, eglDestroyImageKHR, error) == NULL) return FALSE;
  }

  const char* device_exts = NULL;
  const char* driver_name = NULL;
  if (priv->exts.EXT_device_query) {
    EGLAttrib device_attrib;
    if (!priv->procs.eglQueryDisplayAttribEXT(priv->display, EGL_DEVICE_EXT, &device_attrib)) {
      ntk_egl_error_set_egl(error, "Failed to query device attribute", eglGetError());
      return FALSE;
    }

    priv->device_attrib = (EGLDeviceEXT)device_attrib;
    if ((device_exts = priv->procs.eglQueryDeviceStringEXT(priv->device_attrib, EGL_EXTENSIONS)) == NULL) {
      ntk_egl_error_set_egl(error, "Failed to query device extensions", eglGetError());
      return FALSE;
    }

#undef HAS_EXT
#define HAS_EXT(name) ntk_egl_renderer_has_ext(device_exts, name)

    g_debug("Queried device extensions: %s", device_exts);

#ifdef EGL_DRIVER_NAME_EXT
    if (HAS_EXT("EGL_EXT_device_persistent_id")) {
      driver_name = priv->procs.eglQueryDeviceStringEXT(priv->device_attrib, EGL_DRIVER_NAME_EXT);
      g_debug("EGL driver name: %s", driver_name);
    }
#endif
#ifdef NTK_HW_HAS_LIBDRM
    priv->exts.EXT_device_drm = HAS_EXT("EGL_EXT_device_drm");
    priv->exts.EXT_device_drm_render_node = HAS_EXT("EGL_EXT_device_drm_render_node");
#else
    if (HAS_EXT("EGL_EXT_device_drm") || HAS_EXT("EGL_EXT_device_drm_render_node")) g_warning("Device supports DRM but DRM support is disabled");
#endif

#undef HAS_EXT
  }
#define HAS_EXT(name) ntk_egl_renderer_has_ext(display_exts, name)

  if (!HAS_EXT("EGL_KHR_no_config_context") && !HAS_EXT("EGL_MESA_configless_context")) {
    ntk_egl_error_set_missing_ext(error, "extensions not supported", "EGL_KHR_no_config_context/EGL_MESA_configless_context");
    return FALSE;
  }

  if (!HAS_EXT("EGL_KHR_surfaceless_context")) {
    ntk_egl_error_set_missing_ext(error, "extension not supported", "EGL_KHR_surfaceless_context");
    return FALSE;
  }

  priv->exts.IMG_context_priority = HAS_EXT("EGL_IMG_context_priority");

#undef HAS_EXT

  g_debug("EGL vendor: %s", eglQueryString(priv->display, EGL_VENDOR));

  size_t atti = 0;
  EGLint attribs[5];
  attribs[atti++] = EGL_CONTEXT_CLIENT_VERSION;
  attribs[atti++] = 2;

  if (priv->exts.IMG_context_priority) {
    attribs[atti++] = EGL_CONTEXT_PRIORITY_LEVEL_IMG;
    attribs[atti++] = EGL_CONTEXT_PRIORITY_HIGH_IMG;
  }

  attribs[atti++] = EGL_NONE;
  g_assert(atti <= sizeof(attribs)/sizeof(attribs[0]));

  priv->context = eglCreateContext(priv->display, EGL_NO_CONFIG_KHR, EGL_NO_CONTEXT, attribs);
  if (priv->context == EGL_NO_CONTEXT) {
    ntk_egl_error_set_egl(error, "Failed to create EGL context", eglGetError());
    return FALSE;
  }

  if (priv->exts.IMG_context_priority) {
    EGLint priority = EGL_CONTEXT_PRIORITY_MEDIUM_IMG;
    eglQueryContext(priv->display, priv->context, EGL_CONTEXT_PRIORITY_LEVEL_IMG, &priority);
    if (priority != EGL_CONTEXT_PRIORITY_HIGH_IMG) g_error("Failed to obtain a high priority context");
    else g_debug("Obtained high priority context");
  }

  if (!ntk_egl_renderer_begin(self, error)) return FALSE;

  g_debug("Creating GLES2 renderer for EGL renderer");
  priv->gl_renderer = NTK_GLES2_RENDERER(ntk_gles2_renderer_new(error));

  if (!ntk_egl_renderer_end(self, error)) return FALSE;
  return TRUE;
}

static gboolean ntk_egl_renderer_initable_init(GInitable* initable, GCancellable* cancellable, GError** error) {
  NtkEGLRenderer* self = NTK_EGL_RENDERER(initable);
  NtkEGLRendererPrivate* priv = NTK_EGL_RENDERER_PRIVATE(self);

  nk_font_atlas_init_default(&priv->atlas);
  nk_font_atlas_begin(&priv->atlas);

  if (!ntk_egl_renderer_bootstrap_init(self, error)) return FALSE;

  if (priv->exts.EXT_platform_device) {
#ifdef NTK_HW_HAS_LIBDRM
    EGLDeviceEXT dev = ntk_egl_renderer_get_egl_device_from_drm(self, error);
    if (dev == EGL_NO_DEVICE_EXT) {
      if (*error == NULL) ntk_egl_error_set_egl(error, "Failed to get the DRM device", EGL_BAD_DEVICE_EXT);
      return FALSE;
    }

    if (!ntk_egl_renderer_display_init(self, EGL_PLATFORM_DEVICE_EXT, dev, error)) return FALSE;
    return TRUE;
#endif
  }

  ntk_egl_error_set_egl(error, "EGL was not set up", EGL_BAD_CONTEXT);
  return FALSE;
}

static void ntk_egl_renderer_interface_init(GInitableIface* iface) {
  iface->init = ntk_egl_renderer_initable_init;
}

static void ntk_egl_renderer_class_init(NtkEGLRendererClass* klass) {
  GObjectClass* object_class = G_OBJECT_CLASS(klass);
  NtkRendererClass* renderer_class = NTK_RENDERER_CLASS(klass);

  object_class->finalize = ntk_egl_renderer_finalize;

  object_class->set_property = ntk_egl_renderer_set_property;
  object_class->get_property = ntk_egl_renderer_get_property;

  obj_props[PROP_DEVICE] = g_param_spec_object(
    "device", "Ntk Hardware Display Device", "The Ntk hardware display to render onto.", NTK_HW_TYPE_DISPLAY,
    G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE
  );

  obj_props[PROP_GL_RENDERER] = g_param_spec_object(
    "gl-renderer", "GL Renderer", "The Ntk Renderer used for making GL calls.", NTK_GLES2_TYPE_RENDERER, G_PARAM_READABLE
  );

  /**
   * NtkEGLRenderer:display: (skip)
   */
  obj_props[PROP_DISPLAY] = g_param_spec_pointer(
    "display", "EGL Display", "The EGL Display Snapshot to render onto.", G_PARAM_READABLE
  );
  g_object_class_install_properties(object_class, N_PROPERTIES, obj_props);

  renderer_class->get_render_type = ntk_egl_renderer_get_render_type;
  renderer_class->configure_vertex = ntk_egl_renderer_configure_vertex;
  renderer_class->render_vertex = ntk_egl_renderer_render_vertex;
  renderer_class->set_size = ntk_egl_renderer_set_size;
  renderer_class->get_size = ntk_egl_renderer_get_size;
}

static void ntk_egl_renderer_init(NtkEGLRenderer* self) {
  self->priv = ntk_egl_renderer_get_instance_private(self);
}

NtkRenderer* ntk_egl_renderer_new(NtkHWDisplay* device, GError** error) {
  return g_initable_new(NTK_EGL_TYPE_RENDERER, NULL, error, "device", device, NULL);
}

EGLDisplay* ntk_egl_renderer_get_display(NtkEGLRenderer* self) {
  g_return_val_if_fail(NTK_EGL_IS_RENDERER(self), NULL);
  NtkEGLRendererPrivate* priv = NTK_EGL_RENDERER_PRIVATE(self);
  return priv->display;
}
