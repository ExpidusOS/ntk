cairo = dependency('cairo', required: get_option('cairo'))

if cairo.found()
  cairo_examples = get_option('cairo_examples').require(get_option('examples'))

  pangocairo = dependency('pangocairo')
  cairo_gobject = dependency('cairo-gobject', required: false)

  configure_file(input: 'src/NtkCairo.vapi.in', output: 'NtkCairo-@0@.vapi'.format(shortver.split('-')[0]),
    configuration: conf_data)
  configure_file(input: 'src/NtkCairo.deps.in', output: 'NtkCairo-@0@.deps'.format(shortver.split('-')[0]),
    configuration: conf_data)

  libntk_cairo_src = ['src/lib/renderer.c']
  libntk_cairo_headers = ['include/ntk/backend/cairo.h', 'include/ntk/backend/cairo/renderer.h']
  libntk_cairo_inc = include_directories('include')
  libntk_cairo_deps = [cairo, libntk, pangocairo]

  if cairo_gobject.found()
    libntk_cairo_deps += [cairo_gobject]
  endif

  libntk_cairo_shlib = shared_library('ntk-cairo', libntk_cairo_src,
    include_directories: libntk_cairo_inc,
    version: shortver.split('-')[0],
    dependencies: libntk_cairo_deps,
    install: true)
  libntk_cairo_gir = gnome.generate_gir(libntk_cairo_shlib,
    dependencies: [libntk_cairo_deps],
    namespace: 'NtkCairo',
    nsversion: shortver.split('-')[0],
    sources: [libntk_cairo_src, libntk_cairo_headers],
    header: 'ntk/backend/cairo.h',
    install: true)
  libntk_cairo = declare_dependency(link_with: libntk_cairo_shlib,
    dependencies: libntk_cairo_deps,
    include_directories: libntk_cairo_inc)

  if not cairo_examples.disabled()
    executable('ntk-cairo-example-basic', ['src/examples/basic/main.vala'],
      vala_args: ['--vapidir', meson.current_build_dir()],
      dependencies: [
        glib,
        libntk_cairo,
        meson.get_compiler('vala').find_library('NtkCairo-@0@'.format(shortver.split('-')[0]), dirs: meson.current_build_dir()),
        libntk_vapi,
        meson.get_compiler('vala').find_library('nuklear', dirs: join_paths(meson.project_source_root(), 'vapi'))
      ])
  endif
endif
