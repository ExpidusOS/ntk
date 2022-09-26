gtk4_examples = get_option('gtk4_examples').disable_auto_if(not get_option('examples'))

libtokyo = dependency('tokyo-gtk4', required: gtk4_examples)
libadwaita = dependency('libadwaita-1', required: libtokyo.found())

configure_file(input: 'data/NtkGtk4.deps.in', output: 'NtkGtk4-@0@.deps'.format(shortver.split('-')[0]),
  configuration: conf_data)
configure_file(input: 'data/NtkGtk4.metadata.in', output: 'NtkGtk4-@0@.metadata'.format(shortver.split('-')[0]),
  configuration: conf_data)
configure_file(input: 'data/NtkGtk4-custom.vala.in', output: 'NtkGtk4-@0@-custom.vala'.format(shortver.split('-')[0]),
  configuration: conf_data)

libntk_gtk4_src = ['lib/input-dispatcher.c', 'lib/renderer.c', 'lib/widget.c']
libntk_gtk4_headers = ['include/ntk/backend/gtk4.h', 'include/ntk/backend/gtk4/input-dispatcher.h', 'include/ntk/backend/gtk4/renderer.h', 'include/ntk/backend/gtk4/widget.h']
libntk_gtk4_inc = include_directories('include')
libntk_gtk4_deps = [gtk4, libntk, libntk_cairo]

subdir('include/ntk/backend')

libntk_gtk4_shlib = shared_library('ntk-gtk4', libntk_gtk4_src, libntk_gtk4_enums,
  c_args: ['-DG_LOG_DOMAIN="NtkGtk4"'],
  include_directories: libntk_gtk4_inc,
  version: shortver.split('-')[0],
  dependencies: libntk_gtk4_deps,
  install: true)
libntk_gtk4_gir = gnome.generate_gir(libntk_gtk4_shlib,
  dependencies: [libntk_gtk4_deps],
  namespace: 'NtkGtk4',
  nsversion: shortver.split('-')[0],
  sources: [libntk_gtk4_src, libntk_gtk4_headers],
  header: 'ntk/backend/gtk4.h',
  install: true)
libntk_gtk4_vapi = gnome.generate_vapi('NtkGtk4-@0@'.format(shortver.split('-')[0]),
  packages: [libntk_vapi, libntk_cairo_vapi, 'gtk4', 'cairo'],
  sources: [libntk_gtk4_gir[0], join_paths(meson.current_build_dir(), 'NtkGtk4-@0@-custom.vala'.format(shortver.split('-')[0]))],
  vapi_dirs: [join_paths(meson.project_source_root(), 'nuklear', 'vapi'), join_paths(meson.project_build_root(), 'core'), join_paths(meson.project_build_root(), 'backends', 'cairo')],
  install: true)
libntk_gtk4 = declare_dependency(link_with: libntk_gtk4_shlib,
  include_directories: libntk_gtk4_inc,
  dependencies: [libntk_gtk4_deps])
pkg.generate(libntk_gtk4_shlib, description: 'GTK4 widget, input dispatcher, and renderer for NTK',
  name: 'ntk-gtk4',
  url: 'https://github.com/ExpidusOS/ntk',
  version: longver)

if libtokyo.found()
  executable('ntk-gtk4-example-basic', ['examples/basic/main.vala'],
    vala_args: ['--vapidir', meson.current_build_dir(), '--vapidir', join_paths(meson.project_source_root(), 'fonts', 'pango', 'vapi')],
    dependencies: [libtokyo, libadwaita, nuklear, nuklear_vapi, libntk, libntk_vapi, libntk_gtk4, libntk_gtk4_vapi, libntk_pango, libntk_pango_vapi, libntk_cairo, libntk_cairo_vapi])
endif
