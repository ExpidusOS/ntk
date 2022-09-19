#pragma once

#include <GLES2/gl2.h>
#include <glib.h>

#define NTK_GLES2_BASE_SHADERS_PATH "/com/expidus/ntk/backend/gles2/shaders"

GLuint ntk_gles2_compile_shader(GLuint type, const GLchar* src, GError** error);
GLuint ntk_gles2_link_program(const GLchar* vert_src, const GLchar* frag_src, GError** error);
GLuint ntk_gles2_link_program_from_resource_path(const gchar* path, GError** error);
