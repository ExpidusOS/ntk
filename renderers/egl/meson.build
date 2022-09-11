egl = dependency('egl', required: false)

if egl.found()
  libdrm = dependency('libdrm')

  configure_file(input: 'src/NtkRendererEGL.vapi.in', output: 'NtkRendererEGL-@0@.vapi'.format(shortver.split('-')[0]),
    configuration: conf_data)
  configure_file(input: 'src/NtkRendererEGL.deps.in', output: 'NtkRendererEGL-@0@.deps'.format(shortver.split('-')[0]),
    configuration: conf_data)

  libntk_renderer_egl_src = ['src/lib/renderer.c']
  libntk_renderer_egl_headers = ['include/ntk/renderer/egl.h', 'include/ntk/renderer/egl/renderer.h']
  libntk_renderer_egl_inc = include_directories('include')
  libntk_renderer_egl_deps = [egl, libdrm, libntk]
  libntk_renderer_egl_shlib = shared_library('ntk-renderer-egl', libntk_renderer_egl_src,
    include_directories: libntk_renderer_egl_inc,
    version: shortver.split('-')[0],
    dependencies: libntk_renderer_egl_deps,
    install: true)
  libntk_renderer_egl_gir = gnome.generate_gir(libntk_renderer_egl_shlib,
    dependencies: [libntk_renderer_egl_deps],
    namespace: 'NtkRendererEGL',
    nsversion: shortver.split('-')[0],
    sources: [libntk_renderer_egl_src, libntk_renderer_egl_headers],
    header: 'ntk/renderer/egl.h',
    install: true)
  libntk_renderer_egl = declare_dependency(link_with: libntk_renderer_egl_shlib,
    dependencies: libntk_renderer_egl_deps,
    include_directories: libntk_renderer_egl_inc)
endif
