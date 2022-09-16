#define G_LOG_DOMAIN "NtkPangoFont"
#include "object-priv.h"
#include <ntk/font/pango/layout.h>
#include <ntk/font/pango/object.h>
#include <ntk/font/user.h>

#define NTK_PANGO_FONT_PRIVATE(self) (ntk_pango_font_get_instance_private(self))

G_DEFINE_TYPE_WITH_PRIVATE(NtkPangoFont, ntk_pango_font, NTK_TYPE_FONT);

enum {
  PROP_0,
  PROP_DESCRIPTION,
  N_PROPERTIES,
};

static GParamSpec* obj_props[N_PROPERTIES] = {
  NULL,
};

static void ntk_pango_font_set_property(GObject* obj, guint prop_id, const GValue* value, GParamSpec* pspec) {
  NtkPangoFont* self = NTK_PANGO_FONT(obj);
  NtkPangoFontPrivate* priv = NTK_PANGO_FONT_PRIVATE(self);

  switch (prop_id) {
    case PROP_DESCRIPTION:
      priv->desc = g_value_get_boxed(value);
      if (priv->ctx) pango_context_set_font_description(priv->ctx, priv->desc);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
      break;
  }
}

static void ntk_pango_font_get_property(GObject* obj, guint prop_id, GValue* value, GParamSpec* pspec) {
  NtkPangoFont* self = NTK_PANGO_FONT(obj);
  NtkPangoFontPrivate* priv = NTK_PANGO_FONT_PRIVATE(self);

  switch (prop_id) {
    case PROP_DESCRIPTION:
      g_value_set_boxed(value, priv->desc);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
      break;
  }
}

static void ntk_pango_font_class_init(NtkPangoFontClass* klass) {
  GObjectClass* object_class = G_OBJECT_CLASS(klass);

  object_class->set_property = ntk_pango_font_set_property;
  object_class->get_property = ntk_pango_font_get_property;

  obj_props[PROP_DESCRIPTION] = g_param_spec_boxed(
    "description", "Pango Font Description", "The description of the font to utilize.", PANGO_TYPE_FONT_DESCRIPTION,
    G_PARAM_CONSTRUCT | G_PARAM_READWRITE
  );
  g_object_class_install_properties(object_class, N_PROPERTIES, obj_props);
}

static void ntk_pango_font_init(NtkPangoFont* self) {}

NtkFont* ntk_pango_font_new(PangoFontDescription* desc) {
  return g_object_new(NTK_PANGO_TYPE_FONT, "description", desc, NULL);
}

NtkFont* ntk_pango_font_new_with_context(PangoContext* ctx, PangoFontDescription* desc) {
  return g_object_new(NTK_PANGO_TYPE_FONT, "context", ctx, "description", desc, NULL);
}

void ntk_pango_font_set_description(NtkPangoFont* self, const PangoFontDescription* desc) {
  g_return_if_fail(NTK_PANGO_IS_FONT(self));
  g_return_if_fail(desc != NULL);

  NtkPangoFontPrivate* priv = NTK_PANGO_FONT_PRIVATE(self);
  g_return_if_fail(priv != NULL);

  g_clear_pointer(&priv->desc, pango_font_description_free);
  priv->desc = pango_font_description_copy(desc);

  g_object_notify_by_pspec(G_OBJECT(self), obj_props[PROP_DESCRIPTION]);
}

PangoFontDescription* ntk_pango_font_get_description(NtkPangoFont* self) {
  g_return_val_if_fail(NTK_PANGO_IS_FONT(self), NULL);

  NtkPangoFontPrivate* priv = NTK_PANGO_FONT_PRIVATE(self);
  g_return_val_if_fail(priv != NULL, NULL);

  return priv->desc;
}
