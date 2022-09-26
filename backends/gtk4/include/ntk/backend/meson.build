libntk_gtk4_inc_installdir = join_paths(prefix, 'include', 'ntk-gtk4-@0@'.format(shortver), 'ntk', 'backend', 'gtk4')

install_headers([
    'gtk4.h'
  ], install_dir: join_paths(prefix, 'include', 'ntk-gtk4-@0@'.format(shortver), 'ntk', 'backend'))

libntk_gtk4_headers_rel = []
foreach hdr : libntk_gtk4_headers
  libntk_gtk4_headers_rel += hdr.replace('include/ntk/backend/', meson.current_source_dir() + '/')
endforeach
libntk_gtk4_enums = gnome.mkenums('ntk-gtk4-enums', sources: libntk_gtk4_headers_rel,
  c_template: 'ntk-gtk4-enums.c.template',
  h_template: 'ntk-gtk4-enums.h.template',
  install_header: true,
  install_dir: join_paths(prefix, 'include', 'ntk-gtk4-@0@'.format(shortver), 'ntk', 'backend'))

subdir('gtk4')
