#include <ntk/font/pango/ft2/user.h>
#include <pango/pangoft2.h>
#include "user-priv.h"

G_DEFINE_BOXED_TYPE(NtkPangoFT2Font, ntk_pango_ft2_font, ntk_pango_ft2_font_copy, ntk_pango_ft2_font_free);

static NtkUserFont* ntk_pango_ft2_font_impl_copy(NtkUserFont* font) {
  NtkPangoFT2Font* self = (NtkPangoFT2Font*)font;
  NtkPangoFT2FontPrivate* priv = self->priv;
  return (NtkUserFont*)ntk_pango_ft2_font_new(pango_font_describe(priv->font), priv->gpu_upload, priv->gpu_upload_target);
}

static void ntk_pango_ft2_font_impl_free(NtkUserFont* font) {
  NtkPangoFT2Font* self = (NtkPangoFT2Font*)font;
  NtkPangoFT2FontPrivate* priv = self->priv;

  g_clear_object(&priv->font);
  g_clear_object(&priv->ctx);

  free(self->priv);
  free(self);
}

NtkPangoFT2Font* ntk_pango_ft2_font_new(PangoFontDescription* desc, NtkFontUploadGPUCallback gpu_upload, gpointer gpu_upload_target) {
  g_return_val_if_fail(desc != NULL, NULL);
  g_return_val_if_fail(gpu_upload != NULL, NULL);

  NtkPangoFT2Font* self = g_try_malloc0(sizeof (NtkPangoFT2Font));
  if (self == NULL) {
    g_critical("Failed to allocate NtkPangoFT2Font");
    return NULL;
  }

  NtkPangoFT2FontPrivate* priv = g_try_malloc0(sizeof (NtkPangoFT2FontPrivate));
  if (priv == NULL) {
    free(self);
    g_critical("Failed to allocate NtkPangoFT2FontPrivate");
    return NULL;
  }

  PangoFontMap* font_map = pango_ft2_font_map_new();
  if (font_map == NULL) {
    free(priv);
    free(self);
    g_critical("Failed to create a new PangoFontMap");
    return NULL;
  }

  priv->ctx = pango_font_map_create_context(font_map);
  if (priv->ctx == NULL) {
    g_clear_object(&font_map);
    free(priv);
    free(self);
    g_critical("Failed to create a new PangoContext");
    return NULL;
  }

  priv->font = pango_font_map_load_font(font_map, priv->ctx, desc);
  if (priv->font == NULL) {
    g_clear_object(&priv->ctx);
    g_clear_object(&font_map);
    free(priv);
    free(self);
    g_critical("Failed to load font");
    return NULL;
  }

  FcPattern* font_pattern = NULL;
  g_object_get(priv->font, "pattern", &font_pattern, NULL);
  if (font_pattern == NULL) {
    g_clear_object(&priv->font);
    g_clear_object(&priv->ctx);
    g_clear_object(&font_map);
    free(priv);
    free(self);
    g_critical("Failed to get the font pattern");
    return NULL;
  }

  char* filename = NULL;
  if (FcPatternGetString(font_pattern, FC_FILE, 0, (FcChar8**)(void*)&filename) != FcResultMatch) {
    g_clear_object(&priv->font);
    g_clear_object(&priv->ctx);
    g_clear_object(&font_map);
    free(priv);
    free(self);
    g_critical("Failed to get the font filename");
    return NULL;
  }

  g_debug("Font found %s", filename);

  priv->cfg = nk_font_config(pango_font_description_get_size(desc) / PANGO_SCALE);
  priv->cfg.merge_mode = nk_false;

  nk_font_atlas_init_default(&priv->atlas);
  nk_font_atlas_begin(&priv->atlas);
  priv->nk_font = nk_font_atlas_add_from_file(&priv->atlas, filename, pango_font_description_get_size(desc) / PANGO_SCALE, &priv->cfg);
  
  int atlas_width;
  int atlas_height;
  const void* img_data = nk_font_atlas_bake(&priv->atlas, &atlas_width, &atlas_height, NK_FONT_ATLAS_RGBA32);
  priv->img = nk_subimage_ptr((void*)img_data, atlas_width, atlas_height, nk_rect(0, 0, atlas_width, atlas_height));
  priv->img = gpu_upload(&priv->img, gpu_upload_target);

  nk_font_atlas_end(&priv->atlas, nk_handle_id(priv->img.handle.id), &priv->null);
  nk_font_atlas_cleanup(&priv->atlas);

  priv->gpu_upload = gpu_upload;
  priv->gpu_upload_target = gpu_upload_target;

  self->parent_instance.parent_instance = priv->nk_font->handle;
  self->parent_instance.impl_copy = ntk_pango_ft2_font_impl_copy;
  self->parent_instance.impl_free = ntk_pango_ft2_font_impl_free;
  self->priv = priv;
  return self;
}

NtkPangoFT2Font* ntk_pango_ft2_font_copy(NtkPangoFT2Font* self) {
  return (NtkPangoFT2Font*)ntk_user_font_copy((NtkUserFont*)self);
}

void ntk_pango_ft2_font_free(NtkPangoFT2Font* self) {
  ntk_user_font_free((NtkUserFont*)self);
}
