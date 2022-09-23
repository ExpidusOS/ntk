cairo = dependency('cairo', required: get_option('cairo'))

if cairo.found()
  cairo_examples = get_option('cairo_examples').disable_auto_if(not get_option('examples'))

  pangocairo = dependency('pangocairo')
  cairo_gobject = dependency('cairo-gobject', required: false)

  configure_file(input: 'data/NtkCairo.deps.in', output: 'NtkCairo-@0@.deps'.format(shortver.split('-')[0]),
    configuration: conf_data)
  configure_file(input: 'data/NtkCairo.metadata.in', output: 'NtkCairo-@0@.metadata'.format(shortver.split('-')[0]),
    configuration: conf_data)
  configure_file(input: 'data/NtkCairo-custom.vala.in', output: 'NtkCairo-@0@-custom.vala'.format(shortver.split('-')[0]),
    configuration: conf_data)

  libntk_cairo_src = ['lib/renderer.c']
  libntk_cairo_headers = ['include/ntk/backend/cairo.h', 'include/ntk/backend/cairo/renderer.h']
  libntk_cairo_inc = include_directories('include')
  libntk_cairo_deps = [cairo, libntk, pangocairo, libntk_pango]

  if cairo_gobject.found()
    libntk_cairo_deps += [cairo_gobject]
  endif

  libntk_cairo_shlib = shared_library('ntk-cairo', libntk_cairo_src,
    c_args: ['-DG_LOG_DOMAIN="NtkCairo"'],
    include_directories: libntk_cairo_inc,
    version: shortver.split('-')[0],
    dependencies: libntk_cairo_deps,
    install: true)
  if use_gir
    libntk_cairo_gir = gnome.generate_gir(libntk_cairo_shlib,
      dependencies: [libntk_cairo_deps],
      namespace: 'NtkCairo',
      nsversion: shortver.split('-')[0],
      sources: [libntk_cairo_src, libntk_cairo_headers],
      header: 'ntk/backend/cairo.h',
      install: true)
    if use_vala
      libntk_cairo_vapi = gnome.generate_vapi('NtkCairo-@0@'.format(shortver.split('-')[0]),
        packages: [libntk_vapi, 'cairo', 'pangocairo'],
        sources: [libntk_cairo_gir[0], join_paths(meson.current_build_dir(), 'NtkCairo-@0@-custom.vala'.format(shortver.split('-')[0]))],
        vapi_dirs: [join_paths(meson.project_source_root(), 'nuklear', 'vapi'), join_paths(meson.project_build_root(), 'core'), join_paths(meson.project_source_root(), 'fonts', 'pango', 'vapi')],
        install: true)
    endif
  endif
  libntk_cairo = declare_dependency(link_with: libntk_cairo_shlib,
    include_directories: libntk_cairo_inc,
    dependencies: [libntk_cairo_deps])
  pkg.generate(libntk_cairo_shlib, description: 'Cairo renderer for NTK',
    name: 'ntk-cairo',
    url: 'https://github.com/ExpidusOS/ntk',
    version: longver)

  if not cairo_examples.disabled()
    if use_gir and use_vala
      executable('ntk-cairo-example-basic', ['examples/basic/main.vala'],
        vala_args: ['--vapidir', meson.current_build_dir(), '--vapidir', join_paths(meson.project_source_root(), 'fonts', 'pango', 'vapi')],
        dependencies: [nuklear, nuklear_vapi, libntk, libntk_vapi, libntk_cairo, libntk_cairo_vapi, libntk_pango, libntk_pango_vapi, pangocairo])
    endif
  endif
endif
