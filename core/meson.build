configure_file(input: 'data/Ntk.deps.in', output: 'Ntk-@0@.deps'.format(shortver.split('-')[0]),
  configuration: conf_data)
configure_file(input: 'data/Ntk.metadata.in', output: 'Ntk-@0@.metadata'.format(shortver.split('-')[0]),
  configuration: conf_data)
configure_file(input: 'data/Ntk-custom.vala.in', output: 'Ntk-@0@-custom.vala'.format(shortver.split('-')[0]),
  configuration: conf_data)

libntk_src = ['lib/color.c', 'lib/context.c', 'lib/error.c', 'lib/input-dispatcher.c', 'lib/renderer.c', 'lib/styler.c', 'lib/utils.c',
  'lib/font/object.c', 'lib/font/user.c', 'lib/styles/button.c', 'lib/styles/edit.c', 'lib/styles/item.c', 'lib/styles/progress.c',
  'lib/styles/scrollbar.c', 'lib/styles/selectable.c', 'lib/styles/slider.c', 'lib/styles/text.c', 'lib/styles/toggle.c']
libntk_headers = ['include/ntk/ntk.h', 'include/ntk/color.h', 'include/ntk/context.h', 'include/ntk/error.h', 'include/ntk/font.h', 'include/ntk/input-dispatcher.h',
  'include/ntk/renderer.h', 'include/ntk/styler.h', 'include/ntk/utils.h',
  'include/ntk/font/object.h', 'include/ntk/font/user.h']

subdir('include/ntk')

libntk_inc = [include_directories('include')]
libntk_deps = [glib, gobject, nuklear]

libntk_cflags = ['-I' + join_paths(meson.current_build_dir(), 'include'), '-I' + join_paths(meson.current_source_dir(), 'include')]
libntk_shlib = shared_library('ntk', libntk_src, libntk_enums,
  c_args: [libntk_cflags, '-DG_LOG_DOMAIN="Ntk"'],
  version: shortver.split('-')[0],
  include_directories: libntk_inc,
  implicit_include_directories: true,
  dependencies: libntk_deps,
  install: true)

if use_gir
  libntk_gir = gnome.generate_gir(libntk_shlib,
    extra_args: ['--cflags-begin', libntk_cflags, '--cflags-end'],
    dependencies: libntk_deps,
    namespace: 'Ntk',
    nsversion: shortver.split('-')[0],
    sources: [libntk_src, libntk_enums, libntk_headers],
    header: 'ntk/ntk.h',
    install: true)
  if use_vala
    libntk_vapi = gnome.generate_vapi('Ntk-@0@'.format(shortver.split('-')[0]),
      packages: ['nuklear', 'gobject-2.0', 'glib-2.0', 'gio-2.0'],
      sources: [libntk_gir[0], join_paths(meson.current_build_dir(), 'Ntk-@0@-custom.vala'.format(shortver.split('-')[0]))],
      vapi_dirs: join_paths(meson.project_source_root(), 'nuklear', 'vapi'),
      install: true)
  endif
endif

libntk = declare_dependency(link_with: libntk_shlib,
  include_directories: libntk_inc,
  dependencies: [libntk_deps])
pkg.generate(libntk_shlib, description: 'GObject based UI toolkit for Nuklear',
  name: 'ntk',
  url: 'https://github.com/ExpidusOS/ntk',
  version: longver)

if get_option('tests')
  test_prog = executable('ntk-tests', ['tests/main.c'],
    dependencies: libntk)

  test('core', test_prog, env: [
    'G_TEST_SRCDIR=@0@'.format(meson.current_source_dir()),
    'G_TEST_BUILDDIR=@0@'.format(meson.current_build_dir())
  ], protocol: 'tap')
endif
