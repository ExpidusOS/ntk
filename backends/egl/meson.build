egl = dependency('egl', required: get_option('egl'))

if egl.found()
  egl_examples = get_option('egl_examples').require(get_option('examples'))

  configure_file(input: 'src/NtkEGL.deps.in', output: 'NtkEGL-@0@.deps'.format(shortver.split('-')[0]),
    configuration: conf_data)
  configure_file(input: 'src/NtkEGL.metadata.in', output: 'NtkEGL-@0@.metadata'.format(shortver.split('-')[0]),
    configuration: conf_data)
  configure_file(input: 'src/NtkEGL-custom.vala.in', output: 'NtkEGL-@0@-custom.vala'.format(shortver.split('-')[0]),
    configuration: conf_data)

  libntk_egl_src = ['src/lib/error.c', 'src/lib/renderer.c']
  libntk_egl_headers = ['include/ntk/backend/egl.h', 'include/ntk/backend/egl/renderer.h']
  libntk_egl_inc = include_directories('include')
  libntk_egl_deps = [egl, libntk]
  libntk_egl_shlib = shared_library('ntk-egl', libntk_egl_src,
    include_directories: libntk_egl_inc,
    version: shortver.split('-')[0],
    dependencies: libntk_egl_deps,
    install: true)
  if use_gir
    libntk_egl_gir = gnome.generate_gir(libntk_egl_shlib,
      dependencies: [libntk_egl_deps],
      namespace: 'NtkEGL',
      nsversion: shortver.split('-')[0],
      sources: [libntk_egl_src, libntk_egl_headers],
      header: 'ntk/backend/egl.h',
      install: true)
    if use_vala
      libntk_egl_vapi = gnome.generate_vapi('NtkEGL-@0@'.format(shortver.split('-')[0]),
        packages: ['Ntk-@0@'.format(shortver.split('-')[0])],
        sources: [libntk_egl_gir[0], join_paths(meson.current_build_dir(), 'NtkEGL-@0@-custom.vala'.format(shortver.split('-')[0]))],
        vapi_dirs: [join_paths(meson.project_source_root(), 'vapi'), meson.project_build_root()],
        install: true)
    endif
  endif
  libntk_egl = declare_dependency(link_with: libntk_egl_shlib,
    dependencies: libntk_egl_deps,
    include_directories: libntk_egl_inc)
  pkg.generate(libntk_egl_shlib, description: 'EGL renderer for NTK',
    name: 'ntk-egl',
    url: 'https://github.com/ExpidusOS/ntk',
    version: longver)

  if not egl_examples.disabled()
    if use_gir and use_vala
      executable('ntk-egl-example-basic', ['src/examples/basic/main.vala'],
        vala_args: ['--vapidir', meson.current_build_dir()],
        dependencies: [
          glib,
          nuklear,
          nuklear_vapi,
          libntk,
          libntk_vapi,
          declare_dependency(link_with: libntk_egl_shlib,
            include_directories: libntk_egl_inc),
          libntk_egl_vapi,
        ])
    endif
  endif
endif
