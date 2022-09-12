cairo = dependency('cairo', required: false)

if cairo.found()
  cairo_gobject = dependency('cairo-gobject', required: false)

  configure_file(input: 'src/NtkRendererCairo.vapi.in', output: 'NtkRendererCairo-@0@.vapi'.format(shortver.split('-')[0]),
    configuration: conf_data)
  configure_file(input: 'src/NtkRendererCairo.deps.in', output: 'NtkRendererCairo-@0@.deps'.format(shortver.split('-')[0]),
    configuration: conf_data)

  libntk_renderer_cairo_src = ['src/lib/renderer.c']
  libntk_renderer_cairo_headers = ['include/ntk/renderer/cairo.h', 'include/ntk/renderer/cairo/renderer.h']
  libntk_renderer_cairo_inc = include_directories('include')
  libntk_renderer_cairo_deps = [cairo, libntk]

  if cairo_gobject.found()
    libntk_renderer_cairo_deps += [cairo_gobject]
  endif

  libntk_renderer_cairo_shlib = shared_library('ntk-renderer-cairo', libntk_renderer_cairo_src,
    include_directories: libntk_renderer_cairo_inc,
    version: shortver.split('-')[0],
    dependencies: libntk_renderer_cairo_deps,
    install: true)
  libntk_renderer_cairo_gir = gnome.generate_gir(libntk_renderer_cairo_shlib,
    dependencies: [libntk_renderer_cairo_deps],
    namespace: 'NtkRendererCairo',
    nsversion: shortver.split('-')[0],
    sources: [libntk_renderer_cairo_src, libntk_renderer_cairo_headers],
    header: 'ntk/renderer/cairo.h',
    install: true)
  libntk_renderer_cairo = declare_dependency(link_with: libntk_renderer_cairo_shlib,
    dependencies: libntk_renderer_cairo_deps,
    include_directories: libntk_renderer_cairo_inc)
endif
