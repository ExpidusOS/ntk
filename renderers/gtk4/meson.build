gtk4 = dependency('gtk4', required: false)

if gtk4.found()
  configure_file(input: 'src/NtkRendererGtk4.metadata.in', output: 'NtkRendererGtk4-@0@.metadata'.format(shortver.split('-')[0]),
    configuration: conf_data)

  libntk_renderer_gtk4_src = ['src/lib/renderer.c', 'src/lib/widget.c']
  libntk_renderer_gtk4_headers = ['include/ntk/renderer/gtk4.h', 'include/ntk/renderer/gtk4/renderer.h', 'include/ntk/renderer/gtk4/widget.h']
  libntk_renderer_gtk4_inc = include_directories('include')
  libntk_renderer_gtk4_deps = [gtk4, libntk]
  libntk_renderer_gtk4_shlib = shared_library('ntk-renderer-gtk4', libntk_renderer_gtk4_src,
    include_directories: libntk_renderer_gtk4_inc,
    version: shortver.split('-')[0],
    dependencies: libntk_renderer_gtk4_deps,
    install: true)
  libntk_renderer_gtk4_gir = gnome.generate_gir(libntk_renderer_gtk4_shlib,
    dependencies: [libntk_renderer_gtk4_deps],
    namespace: 'NtkRendererGtk4',
    nsversion: shortver.split('-')[0],
    sources: [libntk_renderer_gtk4_src, libntk_renderer_gtk4_headers],
    header: 'ntk/renderer/gtk4.h',
    install: true)
  libntk_renderer_gtk4_vapi = gnome.generate_vapi('NtkRendererGtk4-@0@'.format(shortver.split('-')[0]),
    packages: ['Ntk-@0@'.format(shortver.split('-')[0]), 'gtk4'],
    metadata_dirs: [meson.current_build_dir()],
    sources: libntk_renderer_gtk4_gir[0],
    install: true,
    gir_dirs: meson.project_build_root())
  libntk_renderer_gtk4 = declare_dependency(link_with: libntk_renderer_gtk4_shlib,
    dependencies: libntk_renderer_gtk4_deps,
    include_directories: libntk_renderer_gtk4_inc)
endif
