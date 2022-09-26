libntk_inc_installdir = join_paths(prefix, 'include', 'ntk-@0@'.format(shortver), 'ntk')

libntk_headers_rel = []
foreach hdr : libntk_headers
  libntk_headers_rel += hdr.replace('include/ntk/', meson.current_source_dir() + '/')
endforeach
libntk_enums = gnome.mkenums('ntk-enums', sources: libntk_headers_rel,
  c_template: 'ntk-enums.c.template',
  h_template: 'ntk-enums.h.template',
  install_header: true,
  install_dir: libntk_inc_installdir)

install_headers([
    'color.h',
    'context.h',
    'error.h',
    'font.h',
    'input-dispatcher.h',
    'ntk.h',
    'renderer.h',
    'styler.h',
    'utils.h'
  ],
  install_dir: libntk_inc_installdir)

configure_file(input: 'build.h.in', output: 'build.h',
  configuration: conf_data,
  install: true,
  install_dir: libntk_inc_installdir)

subdir('font')
