egl = dependency('egl', required: false)

if egl.found()
  libdrm = dependency('libdrm')

  configure_file(input: 'src/NtkEGL.vapi.in', output: 'NtkEGL-@0@.vapi'.format(shortver.split('-')[0]),
    configuration: conf_data)
  configure_file(input: 'src/NtkEGL.deps.in', output: 'NtkEGL-@0@.deps'.format(shortver.split('-')[0]),
    configuration: conf_data)

  libntk_egl_src = ['src/lib/renderer.c']
  libntk_egl_headers = ['include/ntk/backend/egl.h', 'include/ntk/backend/egl/renderer.h']
  libntk_egl_inc = include_directories('include')
  libntk_egl_deps = [egl, libdrm, libntk]
  libntk_egl_shlib = shared_library('ntk-egl', libntk_egl_src,
    include_directories: libntk_egl_inc,
    version: shortver.split('-')[0],
    dependencies: libntk_egl_deps,
    install: true)
  libntk_egl_gir = gnome.generate_gir(libntk_egl_shlib,
    dependencies: [libntk_egl_deps],
    namespace: 'NtkEGL',
    nsversion: shortver.split('-')[0],
    sources: [libntk_egl_src, libntk_egl_headers],
    header: 'ntk/backend/egl.h',
    install: true)
  libntk_egl = declare_dependency(link_with: libntk_egl_shlib,
    dependencies: libntk_egl_deps,
    include_directories: libntk_egl_inc)
endif