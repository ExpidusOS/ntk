libntk_egl_inc_installdir = join_paths(prefix, 'include', 'ntk-egl-@0@'.format(shortver), 'ntk', 'backend', 'egl')

install_headers([
    'egl.h'
  ], install_dir: join_paths(prefix, 'include', 'ntk-egl-@0@'.format(shortver), 'ntk', 'backend'))

libntk_egl_headers_rel = []
foreach hdr : libntk_egl_headers
  libntk_egl_headers_rel += hdr.replace('include/ntk/backend/', meson.current_source_dir() + '/')
endforeach
libntk_egl_enums = gnome.mkenums('ntk-egl-enums', sources: libntk_egl_headers_rel,
  c_template: 'ntk-egl-enums.c.template',
  h_template: 'ntk-egl-enums.h.template',
  install_header: true,
  install_dir: join_paths(prefix, 'include', 'ntk-egl-@0@'.format(shortver), 'ntk', 'backend'))

subdir('egl')
