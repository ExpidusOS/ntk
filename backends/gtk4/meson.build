gtk4 = dependency('gtk4', required: get_option('gtk4'))

if gtk4.found()
  gtk4_examples = get_option('gtk4_examples').require(get_option('examples'))

  libtokyo = dependency('tokyo-gtk4', required: gtk4_examples)
  libadwaita = dependency('libadwaita-1', required: libtokyo.found())

  configure_file(input: 'src/NtkGtk4.vapi.in', output: 'NtkGtk4-@0@.vapi'.format(shortver.split('-')[0]),
    configuration: conf_data)
  configure_file(input: 'src/NtkGtk4.deps.in', output: 'NtkGtk4-@0@.deps'.format(shortver.split('-')[0]),
    configuration: conf_data)

  libntk_gtk4_src = ['src/lib/input-dispatcher.c', 'src/lib/renderer.c', 'src/lib/widget.c']
  libntk_gtk4_headers = ['include/ntk/backend/gtk4.h', 'include/ntk/backend/gtk4/input-dispatcher.h', 'include/ntk/backend/gtk4/renderer.h', 'include/ntk/backend/gtk4/widget.h']
  libntk_gtk4_inc = include_directories('include')
  libntk_gtk4_deps = [gtk4, libntk, libntk_cairo]
  libntk_gtk4_shlib = shared_library('ntk-gtk4', libntk_gtk4_src,
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
  libntk_gtk4 = declare_dependency(link_with: libntk_gtk4_shlib,
    dependencies: libntk_gtk4_deps,
    include_directories: libntk_gtk4_inc)

  if libtokyo.found()
    executable('ntk-gtk4-example-basic', ['src/examples/basic/main.vala'],
      vala_args: ['--vapidir', meson.current_build_dir()],
      dependencies: [
        libtokyo,
        libadwaita,
        libntk_gtk4,
        meson.get_compiler('vala').find_library('NtkGtk4-@0@'.format(shortver.split('-')[0]), dirs: meson.current_build_dir()),
        meson.get_compiler('vala').find_library('Ntk-@0@'.format(shortver.split('-')[0]), dirs: meson.project_build_root()),
        meson.get_compiler('vala').find_library('nuklear', dirs: join_paths(meson.project_source_root(), 'vapi'))
      ])
  endif
endif
