libntk_cairo_inc_installdir = join_paths(prefix, 'include', 'ntk-cairo-@0@'.format(shortver), 'ntk', 'backend', 'cairo')

install_headers([
    'cairo.h'
  ], install_dir: join_paths(prefix, 'include', 'ntk-cairo-@0@'.format(shortver), 'ntk', 'backend'))

libntk_cairo_headers_rel = []
foreach hdr : libntk_cairo_headers
  libntk_cairo_headers_rel += hdr.replace('include/ntk/backend/', meson.current_source_dir() + '/')
endforeach
libntk_cairo_enums = gnome.mkenums('ntk-cairo-enums', sources: libntk_cairo_headers_rel,
  c_template: 'ntk-cairo-enums.c.template',
  h_template: 'ntk-cairo-enums.h.template',
  install_header: true,
  install_dir: join_paths(prefix, 'include', 'ntk-cairo-@0@'.format(shortver), 'ntk', 'backend'))

subdir('cairo')
