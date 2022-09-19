#include "shaders-priv.h"
#include <gio/gio.h>

GLuint ntk_gles2_compile_shader(GLuint type, const GLchar* src, GError** error) {
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &src, NULL);
  glCompileShader(shader);

  GLint ok;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
  if (ok == GL_FALSE) {
    GLint log_len = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);

    gchar* log = g_malloc0(sizeof (char) * log_len);
    glGetShaderInfoLog(shader, log_len, &log_len, (GLchar*)log);
    gchar* message = g_strdup_printf("Failed to compile shader: %s", log);
    g_free(log);

    ntk_gles2_error_set_gl(error, message, glGetError());
    g_free(message);
    glDeleteShader(shader);
    return 0;
  }

  g_debug("Compiled shader %d (type: %d): \"%s\"", shader, type, src);
  return shader;
}

GLuint ntk_gles2_link_program(const GLchar* vert_src, const GLchar* frag_src, GError** error) {
  GLuint vert = ntk_gles2_compile_shader(GL_VERTEX_SHADER, vert_src, error);
  if (!vert) return 0;

  GLuint frag = ntk_gles2_compile_shader(GL_FRAGMENT_SHADER, frag_src, error);
  if (!frag) {
    glDeleteShader(vert);
    return 0;
  }

  GLuint prog = glCreateProgram();
  glAttachShader(prog, vert);
  glAttachShader(prog, frag);
  glLinkProgram(prog);

  glDetachShader(prog, vert);
	glDetachShader(prog, frag);
	glDeleteShader(vert);
	glDeleteShader(frag);

  GLint ok;
  glGetProgramiv(prog, GL_LINK_STATUS, &ok);
  if (ok == GL_FALSE) {
    ntk_gles2_error_set_gl(error, "Failed to link shader program", glGetError());
    glDeleteProgram(prog);
    return 0;
  }

  g_debug("Created shader program: %d", prog);
  return prog;
}

GLuint ntk_gles2_link_program_from_resource_path(const gchar* path, GError** error) {
  gchar* vert_path = g_strdup_printf("%s/vert.glsl", path);
  g_return_val_if_fail(vert_path != NULL, 0);

  gchar* frag_path = g_strdup_printf("%s/frag.glsl", path);
  if (frag_path == NULL) {
    g_clear_pointer(&vert_path, g_free);
    return 0;
  }
  
  g_debug("Creating shader program (vert path: %s, frag path: %s)", vert_path, frag_path);

  GBytes* vert_bytes = g_resources_lookup_data(vert_path, G_RESOURCE_LOOKUP_FLAGS_NONE, error);
  g_clear_pointer(&vert_path, g_free);
  if (vert_bytes == NULL) {
    g_clear_pointer(&frag_path, g_free);
    return 0;
  }

  GBytes* frag_bytes = g_resources_lookup_data(frag_path, G_RESOURCE_LOOKUP_FLAGS_NONE, error);
  g_clear_pointer(&frag_path, g_free);
  if (frag_bytes == NULL) {
    g_clear_pointer(&vert_bytes, g_bytes_unref);
    return 0;
  }

  GLuint prog = ntk_gles2_link_program(g_bytes_get_data(vert_bytes, NULL), g_bytes_get_data(frag_bytes, NULL), error);
  g_clear_pointer(&vert_bytes, g_bytes_unref);
  g_clear_pointer(&frag_bytes, g_bytes_unref);
  return prog;
}
