libntk_gles2_inc_installdir = join_paths(prefix, 'include', 'ntk-gles2-@0@'.format(shortver), 'ntk', 'backend', 'gles2')

install_headers([
    'gles2.h'
  ], install_dir: join_paths(prefix, 'include', 'ntk-gles2-@0@'.format(shortver), 'ntk', 'backend'))

libntk_gles2_headers_rel = []
foreach hdr : libntk_gles2_headers
  libntk_gles2_headers_rel += hdr.replace('include/ntk/backend/', meson.current_source_dir() + '/')
endforeach
libntk_gles2_enums = gnome.mkenums('ntk-gles2-enums', sources: libntk_gles2_headers_rel,
  c_template: 'ntk-gles2-enums.c.template',
  h_template: 'ntk-gles2-enums.h.template',
  install_header: true,
  install_dir: join_paths(prefix, 'include', 'ntk-gles2-@0@'.format(shortver), 'ntk', 'backend'))

subdir('gles2')
