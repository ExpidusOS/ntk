libntk_hw_inc_installdir = join_paths(prefix, 'include', 'ntk-hw-@0@'.format(shortver), 'ntk', 'hw')

install_headers([
    'hw.h'
  ],
  install_dir: join_paths(prefix, 'include', 'ntk-hw-@0@'.format(shortver), 'ntk'))

libntk_hw_headers_rel = []
foreach hdr : libntk_hw_headers
  libntk_hw_headers_rel += hdr.replace('include/ntk/', meson.current_source_dir() + '/')
endforeach
libntk_hw_enums = gnome.mkenums('ntk-hw-enums', sources: libntk_hw_headers_rel,
  c_template: 'ntk-hw-enums.c.template',
  h_template: 'ntk-hw-enums.h.template',
  install_header: true,
  install_dir: join_paths(prefix, 'include', 'ntk-hw-@0@'.format(shortver), 'ntk'))

libntk_hw_enums_dep = declare_dependency(sources: [libntk_hw_enums])

subdir('hw')
