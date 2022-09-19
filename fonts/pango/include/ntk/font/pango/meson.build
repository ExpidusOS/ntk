libntk_pango_inc_installdir = join_paths(prefix, 'include', 'ntk-font-pango-@0@'.format(shortver), 'ntk', 'font', 'pango')

configure_file(input: 'build.h.in', output: 'build.h',
  install: true,
  install_dir: libntk_pango_inc_installdir,
  configuration: libntk_pango_conf)

install_headers('layout.h', 'object.h', install_dir: libntk_pango_inc_installdir)
