configure_file(input: 'data/NtkPango.deps.in', output: 'NtkPango-@0@.deps'.format(shortver.split('-')[0]),
  configuration: conf_data)
configure_file(input: 'data/NtkPango.metadata.in', output: 'NtkPango-@0@.metadata'.format(shortver.split('-')[0]),
  configuration: conf_data)
configure_file(input: 'data/NtkPango-custom.vala.in', output: 'NtkPango-@0@-custom.vala'.format(shortver.split('-')[0]),
  configuration: conf_data)

subdir('include/ntk/font/pango')

libntk_pango_src = ['lib/layout.c', 'lib/object.c', 'lib/object-layout.c']
libntk_pango_headers = ['include/ntk/font/pango.h', 'include/ntk/font/pango/layout.h', 'include/ntk/font/pango/object.h', 'include/ntk/font/pango/object-layout.h']

libntk_pango_inc = [include_directories('include')]
libntk_pango_deps = [libntk, pango]

libntk_pango_cflags = ['-I', join_paths(meson.current_build_dir(), 'include')]

libntk_pango_shlib = shared_library('ntk-pango', libntk_pango_src,
  c_args: [libntk_pango_cflags, '-DG_LOG_DOMAIN="NtkPango"'],
  version: shortver.split('-')[0],
  include_directories: libntk_pango_inc,
  dependencies: libntk_pango_deps,
  install: true)

if use_gir
  libntk_pango_gir = gnome.generate_gir(libntk_pango_shlib,
    extra_args: ['--cflags-begin', libntk_pango_cflags, '--cflags-end'],
    dependencies: libntk_pango_deps,
    namespace: 'NtkPango',
    nsversion: shortver.split('-')[0],
    sources: [libntk_pango_src, libntk_pango_headers],
    header: 'ntk/font/pango.h',
    install: true)
  if use_vala
    libntk_pango_vapi = gnome.generate_vapi('NtkPango-@0@'.format(shortver.split('-')[0]),
      packages: ['Ntk-@0@'.format(shortver.split('-')[0]), 'pango'],
      sources: [libntk_pango_gir[0], join_paths(meson.current_build_dir(), 'NtkPango-@0@-custom.vala'.format(shortver.split('-')[0]))],
      vapi_dirs: [join_paths(meson.project_source_root(), 'nuklear', 'vapi'), join_paths(meson.project_build_root(), 'core')],
      install: true)
  endif
endif

libntk_pango = declare_dependency(link_with: libntk_pango_shlib,
  include_directories: libntk_pango_inc,
  dependencies: [libntk_pango_deps])
pkg.generate(libntk_pango_shlib, description: 'Pango font support for Ntk',
  name: 'ntk-pango',
  url: 'https://github.com/ExpidusOS/ntk',
  version: longver)
