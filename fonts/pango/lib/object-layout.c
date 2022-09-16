#define G_LOG_DOMAIN "NtkPangoFontLayout"
#include <ntk/font/pango/layout.h>
#include <ntk/font/pango/object-layout.h>
#include "object-layout-priv.h"

#define NTK_PANGO_FONT_LAYOUT_PRIVATE(self) ((self)->priv == NULL ? ntk_pango_font_layout_get_instance_private(self) : (self)->priv)

G_DEFINE_TYPE_WITH_PRIVATE(NtkPangoFontLayout, ntk_pango_font_layout, NTK_PANGO_TYPE_FONT);

enum {
  PROP_0,
  PROP_LAYOUT,
  N_PROPERTIES,
};

static GParamSpec* obj_props[N_PROPERTIES] = {
  NULL,
};

static void ntk_pango_font_layout_constructed(GObject* obj) {
  G_OBJECT_CLASS(ntk_pango_font_layout_parent_class)->constructed(obj);

  NtkPangoFontLayout* self = NTK_PANGO_FONT_LAYOUT(obj);
  NtkPangoFontLayoutPrivate* priv = NTK_PANGO_FONT_LAYOUT_PRIVATE(self);

  if (priv->layout == NULL) {
    g_debug("Creating a new Pango Layout");
    priv->layout = pango_layout_new(pango_context_new());
  }

  pango_layout_set_font_description(priv->layout, ntk_pango_font_get_description(NTK_PANGO_FONT(self)));
}

static void ntk_pango_font_layout_finalize(GObject* obj) {
  NtkPangoFontLayout* self = NTK_PANGO_FONT_LAYOUT(obj);
  NtkPangoFontLayoutPrivate* priv = NTK_PANGO_FONT_LAYOUT_PRIVATE(self);

  g_clear_object(&priv->layout);

  G_OBJECT_CLASS(ntk_pango_font_layout_parent_class)->finalize(obj);
}

static void ntk_pango_font_layout_set_property(GObject* obj, guint prop_id, const GValue* value, GParamSpec* pspec) {
  NtkPangoFontLayout* self = NTK_PANGO_FONT_LAYOUT(obj);
  NtkPangoFontLayoutPrivate* priv = NTK_PANGO_FONT_LAYOUT_PRIVATE(self);

  switch (prop_id) {
    case PROP_LAYOUT:
      priv->layout = g_value_dup_object(value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
      break;
  }
}

static void ntk_pango_font_layout_get_property(GObject* obj, guint prop_id, GValue* value, GParamSpec* pspec) {
  NtkPangoFontLayout* self = NTK_PANGO_FONT_LAYOUT(obj);
  NtkPangoFontLayoutPrivate* priv = NTK_PANGO_FONT_LAYOUT_PRIVATE(self);

  switch (prop_id) {
    case PROP_LAYOUT:
      g_value_set_object(value, priv->layout);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
      break;
  }
}

static NtkUserFont* ntk_pango_font_layout_get_handle(NtkFont* font) {
  NtkPangoFontLayout* self = NTK_PANGO_FONT_LAYOUT(font);
  g_return_val_if_fail(self != NULL, NULL);

  NtkPangoFontLayoutPrivate* priv = NTK_PANGO_FONT_LAYOUT_PRIVATE(self);
  g_return_val_if_fail(priv != NULL, NULL);

  return (NtkUserFont*)ntk_pango_layout_font_new(priv->layout, (PangoFontDescription*)pango_layout_get_font_description(priv->layout));
}

static void ntk_pango_font_layout_class_init(NtkPangoFontLayoutClass* klass) {
  GObjectClass* object_class = G_OBJECT_CLASS(klass);
  NtkFontClass* font_class = NTK_FONT_CLASS(klass);

  object_class->constructed = ntk_pango_font_layout_constructed;
  object_class->finalize = ntk_pango_font_layout_finalize;

  object_class->set_property = ntk_pango_font_layout_set_property;
  object_class->get_property = ntk_pango_font_layout_get_property;

  obj_props[PROP_LAYOUT] = g_param_spec_object(
    "layout", "Pango Layout", "The Pango layout to use", PANGO_TYPE_LAYOUT,
    G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE
  );
  g_object_class_install_properties(object_class, N_PROPERTIES, obj_props);

  font_class->get_handle = ntk_pango_font_layout_get_handle;
}

static void ntk_pango_font_layout_init(NtkPangoFontLayout* self) {
  self->priv = ntk_pango_font_layout_get_instance_private(self);
}

NtkFont* ntk_pango_font_layout_new(PangoFontDescription* desc) {
  return g_object_new(NTK_PANGO_TYPE_FONT_LAYOUT, "description", desc, NULL);
}

NtkFont* ntk_pango_font_layout_new_with_layout(PangoLayout* layout, PangoFontDescription* desc) {
  return g_object_new(NTK_PANGO_TYPE_FONT_LAYOUT, "layout", layout, "description", desc, NULL);
}

PangoLayout* ntk_pango_font_layout_get_layout(NtkPangoFontLayout* self) {
  g_return_val_if_fail(NTK_PANGO_IS_FONT_LAYOUT(self), NULL);

  NtkPangoFontLayoutPrivate* priv = NTK_PANGO_FONT_LAYOUT_PRIVATE(self);
  g_return_val_if_fail(priv != NULL, NULL);

  return priv->layout;
}
