configure_file(input: 'data/NtkHW.deps.in', output: 'NtkHW-@0@.deps'.format(shortver.split('-')[0]),
  configuration: conf_data)
configure_file(input: 'data/NtkHW.metadata.in', output: 'NtkHW-@0@.metadata'.format(shortver.split('-')[0]),
  configuration: conf_data)

libntk_hw_src = ['lib/device.c', 'lib/display.c', 'lib/error.c', 'lib/input.c']
libntk_hw_headers = ['include/ntk/hw.h', 'include/ntk/hw/device.h', 'include/ntk/hw/display.h', 'include/ntk/hw/error.h', 'include/ntk/hw/input.h']
libntk_hw_inc = [include_directories('include')]
libntk_hw_deps = [libntk]
libntk_hw_cflags = ['-I @0@'.format(join_paths(meson.current_build_dir(), 'include'))]
libntk_hw_vala_extra = ''

subdir('include/ntk')
libntk_hw_deps += [libntk_hw_enums_dep]

if libdrm.found()
  libntk_hw_deps += [libdrm]
  libntk_hw_src += ['lib/drm/render.c']
  libntk_hw_headers += ['include/ntk/hw/drm.h']
  libntk_hw_vala_extra += fs.read(join_paths(meson.current_source_dir(), 'data', 'extra', 'drm.vala'), encoding: 'utf-8')
endif

libntk_hw_conf = configuration_data()
libntk_hw_conf.merge_from(conf_data)
libntk_hw_conf.set('HW_EXTRA_VALA', libntk_hw_vala_extra)

configure_file(input: 'data/NtkHW-custom.vala.in', output: 'NtkHW-@0@-custom.vala'.format(shortver.split('-')[0]),
  configuration: libntk_hw_conf)

libntk_hw_shlib = shared_library('ntk-hw', libntk_hw_enums, libntk_hw_src,
  c_args: [libntk_hw_cflags, '-DG_LOG_DOMAIN="NtkHW"'],
  version: shortver.split('-')[0],
  include_directories: libntk_hw_inc,
  dependencies: libntk_hw_deps,
  install: true)
if use_gir
  libntk_hw_gir = gnome.generate_gir(libntk_hw_shlib,
    extra_args: ['--cflags-begin', libntk_hw_cflags, '--cflags-end'],
    dependencies: [libntk_hw_deps],
    namespace: 'NtkHW',
    nsversion: shortver.split('-')[0],
    sources: [libntk_hw_src, libntk_hw_enums, libntk_hw_headers],
    header: 'ntk/hw.h',
    install: true)
  if use_vala
    libntk_hw_vapi = gnome.generate_vapi('NtkHW-@0@'.format(shortver.split('-')[0]),
      packages: [libntk_vapi],
      sources: [libntk_hw_gir[0], join_paths(meson.current_build_dir(), 'NtkHW-@0@-custom.vala'.format(shortver.split('-')[0]))],
      vapi_dirs: [join_paths(meson.project_source_root(), 'nuklear', 'vapi'), join_paths(meson.project_build_root(), 'core')],
      install: true)
  endif
endif
libntk_hw = declare_dependency(link_with: libntk_hw_shlib,
  include_directories: libntk_hw_inc,
  dependencies: [libntk_hw_deps])
pkg.generate(libntk_hw_shlib, description: 'Hardware support library for NTK',
  name: 'ntk-hw',
  url: 'https://github.com/ExpidusOS/ntk',
  version: longver)

