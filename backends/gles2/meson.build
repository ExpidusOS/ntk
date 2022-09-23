gles2 = dependency('glesv2', required: get_option('gles2').require(pangoft2.found()))

if gles2.found()
  configure_file(input: 'data/NtkGLES2.deps.in', output: 'NtkGLES2-@0@.deps'.format(shortver.split('-')[0]),
    configuration: conf_data)
  configure_file(input: 'data/NtkGLES2.metadata.in', output: 'NtkGLES2-@0@.metadata'.format(shortver.split('-')[0]),
    configuration: conf_data)
  configure_file(input: 'data/NtkGLES2-custom.vala.in', output: 'NtkGLES2-@0@-custom.vala'.format(shortver.split('-')[0]),
    configuration: conf_data)
  libntk_gles2_res_source = configure_file(input: 'data/ntk-gles2.gresource.xml.in', output: 'ntk-gles2.gresource.xml',
    configuration: conf_data)

  libntk_gles2_res = gnome.compile_resources('ntk-gles2-resources', libntk_gles2_res_source,
    c_name: 'ntk_gles2')

  libntk_gles2_src = ['lib/error.c', 'lib/font.c', 'lib/pixel-format.c', 'lib/renderer.c', 'lib/shaders.c', libntk_gles2_res]
  libntk_gles2_headers = ['include/ntk/backend/gles2.h', 'include/ntk/backend/gles2/error.h', 'include/ntk/backend/gles2/font.h', 'include/ntk/backend/gles2/renderer.h']
  libntk_gles2_inc = include_directories('include')
  libntk_gles2_deps = [gles2, libntk, libntk_pango, libntk_hw]
  libntk_gles2_shlib = shared_library('ntk-gles2', libntk_gles2_src,
    c_args: ['-DG_LOG_DOMAIN="NtkGLES2"'],
    include_directories: libntk_gles2_inc,
    version: shortver.split('-')[0],
    dependencies: libntk_gles2_deps,
    install: true)
  if use_gir
    libntk_gles2_gir = gnome.generate_gir(libntk_gles2_shlib,
      dependencies: [libntk_gles2_deps],
      namespace: 'NtkGLES2',
      nsversion: shortver.split('-')[0],
      sources: [libntk_gles2_src, libntk_gles2_headers],
      header: 'ntk/backend/gles2.h',
      install: true)
    if use_vala
      libntk_gles2_vapi = gnome.generate_vapi('NtkGLES2-@0@'.format(shortver.split('-')[0]),
        packages: [libntk_vapi, libntk_pango_vapi, libntk_hw_vapi],
        sources: [libntk_gles2_gir[0], join_paths(meson.current_build_dir(), 'NtkGLES2-@0@-custom.vala'.format(shortver.split('-')[0]))],
        vapi_dirs: [join_paths(meson.project_source_root(), 'nuklear', 'vapi'), join_paths(meson.project_build_root(), 'core'), join_paths(meson.project_build_root(), 'fonts', 'pango'), join_paths(meson.project_build_root(), 'hw')],
        install: true)
    endif
  endif
  libntk_gles2 = declare_dependency(link_with: libntk_gles2_shlib,
    dependencies: libntk_gles2_deps,
    include_directories: libntk_gles2_inc)
  pkg.generate(libntk_gles2_shlib, description: 'GLES2 renderer for NTK',
    name: 'ntk-gles2',
    url: 'https://github.com/ExpidusOS/ntk',
    version: longver)
endif
