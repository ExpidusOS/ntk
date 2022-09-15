project('ntk', 'vala', 'c', license: 'GPL-3.0-only', version: '0.1.0-prealpha')

gnome = import('gnome')
pkg = import('pkgconfig')

prefix = get_option('prefix')
bindir = prefix / get_option('bindir')
datadir = prefix / get_option('datadir')
libdir = prefix / get_option('libdir')
libexecdir = prefix / get_option('libexecdir')
localstatedir = get_option('localstatedir')

longver = '@0@'.format(meson.project_version())
shortver = longver
git = find_program('git', native: true, required: false)
if git.found()
  git_commit = run_command([git, 'rev-parse', '--short', 'HEAD'])
  git_branch = run_command([git, 'rev-parse', '--abbrev-ref', 'HEAD'])
  if git_commit.returncode() == 0 and git_branch.returncode() == 0
    shortver = '@0@-@1@'.format(meson.project_version(), git_commit.stdout().strip())
    longver = '@0@ (branch \'@1@\')'.format(shortver, git_branch.stdout().strip())
  endif
endif

add_project_arguments(['--vapidir', meson.current_build_dir()], language: 'vala')

glib = [dependency('glib-2.0'), dependency('gio-2.0')]
gobject = dependency('gobject-2.0')
pango = dependency('pango')
libmath = meson.get_compiler('c').find_library('m', required: false)
libdrm = meson.get_compiler('c').find_library('libdrm', required: false)

valadoc = find_program('valadoc', native: true, required: false)

conf_data = configuration_data()
conf_data.set('PREFIX', prefix)
conf_data.set('BINDIR', bindir)
conf_data.set('DATADIR', datadir)
conf_data.set('LIBDIR', libdir)
conf_data.set('LIBEXECDIR', libexecdir)
conf_data.set('VERSION', longver)
conf_data.set('SMALL_VERSION', shortver.split('-')[0])
conf_data.set('BUILDDIR', meson.current_build_dir())
conf_data.set('SOURCEDIR', meson.current_source_dir())
conf_data.set('NTK_HW_HAS_LIBDRM', libdrm.found())

nuklear_cflags = [
  '-DNK_INCLUDE_DEFAULT_FONT=1',
  '-DNK_INCLUDE_FONT_BAKING=1',
  '-DNK_INCLUDE_DEFAULT_ALLOCATOR=1',
  '-DNK_INCLUDE_VERTEX_BUFFER_OUTPUT=1',
  '-DNK_INCLUDE_FIXED_TYPES=1',
  '-DNK_INCLUDE_STANDARD_IO=1',
  '-DNK_INCLUDE_STANDARD_VARARGS=1',
  '-DNK_MEMSET=memset',
  '-DNK_MEMCPY=memcpy',
  '-DNK_DTOA=nk_dtoa',
  '-DNK_COS=cos',
  '-DNK_SIN=sin',
  '-DNK_INV_SQRT=nk_inv_sqrt',
]

if meson.get_compiler('c').has_argument('-Wno-nullability-completeness')
  nuklear_cflags += ['-Wno-nullability-completeness']
endif

if meson.get_compiler('c').has_argument('-Wno-implicit-function-declaration')
  nuklear_cflags += ['-Wno-implicit-function-declaration']
endif

if meson.get_compiler('c').has_argument('-Wno-expansion-to-defined')
  nuklear_cflags += ['-Wno-expansion-to-defined']
endif

if meson.get_compiler('c').has_argument('-Wno-unused-function')
  nuklear_cflags += ['-Wno-unused-function']
endif

nuklear_src = [
  'deps/nuklear/src/nuklear_combo.c', 'deps/nuklear/src/nuklear_menu.c',
  'deps/nuklear/src/nuklear_group.c', 'deps/nuklear/src/nuklear_progress.c',
  'deps/nuklear/src/nuklear_util.c', 'deps/nuklear/src/nuklear_slider.c',
  'deps/nuklear/src/nuklear_window.c', 'deps/nuklear/src/nuklear_math.c',
  'deps/nuklear/src/nuklear_property.c', 'deps/nuklear/src/nuklear_toggle.c',
  'deps/nuklear/src/nuklear_page_element.c', 'deps/nuklear/src/nuklear_color.c',
  'deps/nuklear/src/nuklear_vertex.c', 'deps/nuklear/src/nuklear_tree.c',
  'deps/nuklear/src/nuklear_font.c', 'deps/nuklear/src/nuklear_chart.c',
  'deps/nuklear/src/nuklear_widget.c', 'deps/nuklear/src/nuklear_string.c',
  'deps/nuklear/src/nuklear_input.c', 'deps/nuklear/src/nuklear_pool.c',
  'deps/nuklear/src/nuklear_buffer.c', 'deps/nuklear/src/nuklear_color_picker.c',
  'deps/nuklear/src/nuklear_draw.c', 'deps/nuklear/src/nuklear_context.c',
  'deps/nuklear/src/nuklear_table.c', 'deps/nuklear/src/nuklear_contextual.c',
  'deps/nuklear/src/nuklear_edit.c', 'deps/nuklear/src/nuklear_utf8.c',
  'deps/nuklear/src/nuklear_scrollbar.c', 'deps/nuklear/src/nuklear_panel.c',
  'deps/nuklear/src/nuklear_list_view.c', 'deps/nuklear/src/nuklear_text.c',
  'deps/nuklear/src/nuklear_9slice.c', 'deps/nuklear/src/nuklear_text_editor.c',
  'deps/nuklear/src/nuklear_popup.c', 'deps/nuklear/src/nuklear_tooltip.c',
  'deps/nuklear/src/nuklear_selectable.c', 'deps/nuklear/src/nuklear_button.c',
  'deps/nuklear/src/nuklear_image.c', 'deps/nuklear/src/nuklear_style.c',
  'deps/nuklear/src/nuklear_layout.c', 'deps/nk_utils.c'
]

nuklear_deps = []

if libmath.found()
  nuklear_deps += [libmath]
endif

nuklear_version = '4.10.1'
nuklear_shlib = shared_library('nuklear', nuklear_src,
  dependencies: nuklear_deps,
  c_args: nuklear_cflags,
  install: true)
nuklear = declare_dependency(link_with: nuklear_shlib,
  dependencies: nuklear_deps,
  compile_args: nuklear_cflags,
  include_directories: include_directories('deps/nuklear'))
nuklear_vapi = meson.get_compiler('vala').find_library('nuklear', dirs: join_paths(meson.project_source_root(), 'vapi'))
install_data('vapi/nuklear.vapi', install_dir: join_paths(datadir, 'vala', 'vapi'))
install_data('deps/nuklear/nuklear.h', install_dir: join_paths(prefix, 'include', 'ntk-@0@'.format(shortver)))

nuklear_pc_cflags = nuklear_cflags
nuklear_pc_cflags += ['-I${includedir}/ntk-@0@'.format(shortver)]
pkg.generate(nuklear_shlib, description: 'A minimal state immediate mode graphical user interface toolkit.',
  name: 'nuklear',
  url: 'https://github.com/Immediate-Mode-UI/Nuklear',
  version: nuklear_version,
  extra_cflags: nuklear_pc_cflags)

configure_file(input: 'src/Ntk.deps.in', output: 'Ntk-@0@.deps'.format(shortver.split('-')[0]),
  configuration: conf_data)
configure_file(input: 'src/Ntk.metadata.in', output: 'Ntk-@0@.metadata'.format(shortver.split('-')[0]),
  configuration: conf_data)
configure_file(input: 'src/Ntk-custom.vala.in', output: 'Ntk-@0@-custom.vala'.format(shortver.split('-')[0]),
  configuration: conf_data)

subdir('include/ntk')
subdir('include-hw/ntk')

libntk_src = ['src/lib/context.c', 'src/lib/error.c', 'src/lib/input-dispatcher.c', 'src/lib/renderer.c', 'src/lib/utils.c',
  'src/lib/font/object.c', 'src/lib/font/user.c', 'src/lib/font/pango/layout.c', 'src/lib/font/pango/object.c']
libntk_headers = ['include/ntk/ntk.h', 'include/ntk/context.h', 'include/ntk/error.h', 'include/ntk/font.h', 'include/ntk/input-dispatcher.h', 'include/ntk/renderer.h', 'include/ntk/utils.h',
  'include/ntk/font/object.h', 'include/ntk/font/user.h', 'include/ntk/font/pango/layout.h', 'include/ntk/font/pango/object.h']

libntk_inc = [include_directories('include')]
libntk_deps = [glib, gobject, pango, nuklear]

libntk_cflags = ['-I', join_paths(meson.project_build_root(), 'include')]
libntk_enums = gnome.mkenums('ntk-enums', sources: libntk_headers)

libntk_shlib = shared_library('ntk', libntk_src, libntk_enums,
  c_args: libntk_cflags,
  version: shortver.split('-')[0],
  include_directories: libntk_inc,
  dependencies: libntk_deps,
  install: true)
libntk_gir = gnome.generate_gir(libntk_shlib,
  extra_args: ['--cflags-begin', libntk_cflags, '--cflags-end'],
  dependencies: [libntk_deps],
  namespace: 'Ntk',
  nsversion: shortver.split('-')[0],
  sources: [libntk_src, libntk_enums, libntk_headers],
  header: 'ntk/ntk.h',
  install: true)
libntk_vapi = gnome.generate_vapi('Ntk-@0@'.format(shortver.split('-')[0]),
  packages: ['nuklear', 'pango', 'gobject-2.0', 'glib-2.0', 'gio-2.0'],
  sources: [libntk_gir[0], join_paths(meson.project_build_root(), 'Ntk-@0@-custom.vala'.format(shortver.split('-')[0]))],
  vapi_dirs: join_paths(meson.project_source_root(), 'vapi'),
  install: true)
libntk = declare_dependency(link_with: libntk_shlib,
  include_directories: libntk_inc,
  dependencies: [libntk_deps])
pkg.generate(libntk_shlib, description: 'GObject based UI toolkit for Nuklear',
  name: 'ntk',
  url: 'https://github.com/ExpidusOS/ntk',
  version: longver)

configure_file(input: 'src/NtkHW.deps.in', output: 'NtkHW-@0@.deps'.format(shortver.split('-')[0]),
  configuration: conf_data)
configure_file(input: 'src/NtkHW.metadata.in', output: 'NtkHW-@0@.metadata'.format(shortver.split('-')[0]),
  configuration: conf_data)
configure_file(input: 'src/NtkHW-custom.vala.in', output: 'NtkHW-@0@-custom.vala'.format(shortver.split('-')[0]),
  configuration: conf_data)

libntk_hw_src = ['src/libhw/device.c', 'src/libhw/device-file.c', 'src/libhw/display.c', 'src/libhw/input.c']
libntk_hw_headers = ['include-hw/ntk/hw.h', 'include-hw/ntk/hw/device.h', 'include-hw/ntk/hw/device-file.h', 'include-hw/ntk/hw/display.h', 'include-hw/ntk/hw/input.h']
libntk_hw_inc = [include_directories('include-hw')]
libntk_hw_deps = [libntk]
libntk_hw_cflags = ['-I @0@'.format(join_paths(meson.project_build_root(), 'include-hw'))]

if libdrm.found()
  libntk_hw_src += ['src/libhw/device/drm.c']
  libntk_hw_headers += ['include-hw/ntk/hw/device/drm.h']
endif

message(libntk_hw_cflags)

libntk_hw_enums = gnome.mkenums('ntk-hw-enums', sources: libntk_hw_headers)

libntk_hw_shlib = shared_library('ntk-hw', libntk_hw_src, libntk_hw_enums,
  c_args: libntk_hw_cflags,
  version: shortver.split('-')[0],
  include_directories: libntk_hw_inc,
  dependencies: libntk_hw_deps,
  install: true)
libntk_hw_gir = gnome.generate_gir(libntk_hw_shlib,
  extra_args: ['--cflags-begin', libntk_hw_cflags, '--cflags-end'],
  dependencies: [libntk_hw_deps],
  namespace: 'NtkHW',
  nsversion: shortver.split('-')[0],
  sources: [libntk_hw_src, libntk_hw_enums, libntk_hw_headers],
  header: 'ntk/hw.h',
  install: true)
libntk_hw_vapi = gnome.generate_vapi('NtkHW-@0@'.format(shortver.split('-')[0]),
  packages: ['Ntk-@0@'.format(shortver.split('-')[0])],
  sources: [libntk_hw_gir[0], join_paths(meson.project_build_root(), 'NtkHW-@0@-custom.vala'.format(shortver.split('-')[0]))],
  vapi_dirs: [join_paths(meson.project_source_root(), 'vapi'), meson.project_build_root()],
  install: true)
libntk_hw = declare_dependency(link_with: libntk_hw_shlib,
  include_directories: libntk_hw_inc,
  dependencies: [libntk_hw_deps])
pkg.generate(libntk_hw_shlib, description: 'Hardware support library for NTK',
  name: 'ntk-hw',
  url: 'https://github.com/ExpidusOS/ntk',
  version: longver)

subdir('backends')
