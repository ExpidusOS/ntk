libntk_css_inc_installdir = join_paths(prefix, 'include', 'ntk-css-@0@'.format(shortver), 'ntk', 'styling', 'css')

install_headers([
    'css.h'
  ], install_dir: join_paths(prefix, 'include', 'ntk-css-@0@'.format(shortver), 'ntk', 'styling'))

libntk_css_headers_rel = []
foreach hdr : libntk_css_headers
  libntk_css_headers_rel += hdr.replace('include/ntk/styling/', meson.current_source_dir() + '/')
endforeach

libntk_css_enums = gnome.mkenums('ntk-css-enums', sources: libntk_css_headers_rel,
  c_template: 'ntk-css-enums.c.template',
  h_template: 'ntk-css-enums.h.template',
  install_header: true,
  install_dir: join_paths(prefix, 'include', 'ntk-css-@0@'.format(shortver), 'ntk', 'styling'))

subdir('css')
