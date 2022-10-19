configure_file(input: 'data/NtkCSS.deps.in', output: 'NtkCSS-@0@.deps'.format(shortver.split('-')[0]),
  configuration: conf_data)
configure_file(input: 'data/NtkCSS.metadata.in', output: 'NtkCSS-@0@.metadata'.format(shortver.split('-')[0]),
  configuration: conf_data)
configure_file(input: 'data/NtkCSS-custom.vala.in', output: 'NtkCSS-@0@-custom.vala'.format(shortver.split('-')[0]),
  configuration: conf_data)

libntk_css_src = ['lib/error.c', 'lib/styler.c']
libntk_css_headers = ['include/ntk/styling/css.h', 'include/ntk/styling/css/error.h', 'include/ntk/styling/css/styler.h']

subdir('include/ntk/styling')

libntk_css_inc = include_directories('include')
libntk_css_deps = [libntk, cssparser, libntk_css_enums_dep]

libntk_css_cflags = ['-I @0@'.format(join_paths(meson.current_build_dir(), 'include'))]

libntk_css_shlib = shared_library('ntk-css', libntk_css_enums, libntk_css_src, 
  c_args: [libntk_css_cflags, '-DG_LOG_DOMAIN="NtkCSS"'],
  version: shortver.split('-')[0],
  include_directories: libntk_css_inc,
  dependencies: libntk_css_deps,
  install: true)

if use_gir
  libntk_css_gir = gnome.generate_gir(libntk_css_shlib,
    extra_args: ['--cflags-begin', libntk_css_cflags, '--cflags-end'],
    dependencies: libntk_css_deps,
    namespace: 'NtkCSS',
    nsversion: shortver.split('-')[0],
    sources: [libntk_css_src, libntk_css_headers],
    header: 'ntk/styling/css.h',
    install: true)
  if use_vala
    libntk_css_vapi = gnome.generate_vapi('NtkCSS-@0@'.format(shortver.split('-')[0]),
      packages: [libntk_vapi],
      sources: [libntk_css_gir[0], join_paths(meson.current_build_dir(), 'NtkCSS-@0@-custom.vala'.format(shortver.split('-')[0]))],
      vapi_dirs: [join_paths(meson.project_source_root(), 'nuklear', 'vapi'), join_paths(meson.project_build_root(), 'core')],
      install: true)
  endif
endif

libntk_css = declare_dependency(link_with: libntk_css_shlib,
  include_directories: libntk_css_inc,
  dependencies: libntk_css_deps)
pkg.generate(libntk_css_shlib, description: 'CSS styling suppport for Ntk',
  name: 'ntk-css',
  url: 'https://github.com/ExpidusOS/ntk',
  version: longver)

if get_option('tests')
  test_prog = executable('ntk-styling-css-tests', ['tests/main.c'],
    dependencies: libntk_css)

  test('styling/css', test_prog, env: [
    'G_TEST_SRCDIR=@0@'.format(meson.current_source_dir()),
    'G_TEST_BUILDDIR=@0@'.format(meson.current_build_dir())
  ], protocol: 'tap')
endif
