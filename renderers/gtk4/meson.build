gtk4 = dependency('gtk4', required: false)

if gtk4.found()
  libtokyo = dependency('tokyo-gtk4', required: false)
  libadwaita = dependency('libadwaita-1', required: libtokyo.found())

  configure_file(input: 'src/NtkRendererGtk4.vapi.in', output: 'NtkRendererGtk4-@0@.vapi'.format(shortver.split('-')[0]),
    configuration: conf_data)
  configure_file(input: 'src/NtkRendererGtk4.deps.in', output: 'NtkRendererGtk4-@0@.deps'.format(shortver.split('-')[0]),
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
  libntk_renderer_gtk4 = declare_dependency(link_with: libntk_renderer_gtk4_shlib,
    dependencies: libntk_renderer_gtk4_deps,
    include_directories: libntk_renderer_gtk4_inc)

  if libtokyo.found()
    executable('ntk-gtk4-example-basic', ['src/examples/basic/main.vala'],
      vala_args: ['--vapidir', meson.current_build_dir()],
      dependencies: [
        libtokyo,
        libadwaita,
        libntk_renderer_gtk4,
        meson.get_compiler('vala').find_library('NtkRendererGtk4-@0@'.format(shortver.split('-')[0]), dirs: meson.current_build_dir()),
        meson.get_compiler('vala').find_library('Ntk-@0@'.format(shortver.split('-')[0]), dirs: meson.project_build_root()),
        meson.get_compiler('vala').find_library('nuklear', dirs: join_paths(meson.project_source_root(), 'vapi'))
      ])
  endif
endif
