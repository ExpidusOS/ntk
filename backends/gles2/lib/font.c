#include <GLES2/gl2.h>
#include <ntk/backend/gles2/font.h>
#include <nuklear.h>

G_DEFINE_TYPE(NtkGLES2Font, ntk_gles2_font, NTK_PANGO_TYPE_FONT);

static struct nk_image ntk_gles2_font_upload(struct nk_image* src, gpointer userdata) {
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)src->w, (GLsizei)src->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, src->handle.ptr);
  glBindTexture(GL_TEXTURE_2D, 0);
  g_debug("Uploading font to GPU with texture ID: %d", texture);
  return nk_subimage_id(texture, src->w, src->h, nk_rect(src->region[0], src->region[1], src->region[2], src->region[2]));
}

static NtkUserFont* ntk_gles2_font_get_handle(NtkFont* font) {
  NtkPangoFont* pango_font = NTK_PANGO_FONT(font);
  return (NtkUserFont*)ntk_pango_ft2_font_new(ntk_pango_font_get_description(pango_font), ntk_gles2_font_upload, NULL);
}

static void ntk_gles2_font_class_init(NtkGLES2FontClass* klass) {
  NtkFontClass* font_class = NTK_FONT_CLASS(klass);

  font_class->get_handle = ntk_gles2_font_get_handle;
}

static void ntk_gles2_font_init(NtkGLES2Font* self) {}

NtkFont* ntk_gles2_font_layout_new(PangoFontDescription* desc) {
  return g_object_new(NTK_GLES2_TYPE_FONT, "description", desc, NULL);
}
