project('ntk', 'c', license: 'GPL-3.0-only', version: '0.1.0-prealpha')

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

gobject = dependency('gobject-2.0')
pango = dependency('pango')

conf_data = configuration_data()
conf_data.set('PREFIX', prefix)
conf_data.set('BINDIR', bindir)
conf_data.set('DATADIR', datadir)
conf_data.set('LIBDIR', libdir)
conf_data.set('LIBEXECDIR', libexecdir)
conf_data.set('VERSION', longver)
conf_data.set('BUILDDIR', meson.current_build_dir())
conf_data.set('SOURCEDIR', meson.current_source_dir())

nuklear_cargs = [
  '-DNK_INCLUDE_DEFAULT_ALLOCATOR=1',
  '-DNK_INCLUDE_FIXED_TYPES=1',
  '-DNK_INCLUDE_STANDARD_IO=1',
  '-DNK_INCLUDE_STANDARD_VARARGS=1',
  '-DNK_MEMSET=memset',
  '-DNK_MEMCPY=memcpy',
  '-DNK_DTOA=nk_dtoa'
]
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
nuklear_deps = [meson.get_compiler('c').find_library('m')]
nuklear_shlib = shared_library('nuklear', nuklear_src,
  dependencies: nuklear_deps,
  c_args: nuklear_cargs,
  install: true)
nuklear = declare_dependency(link_with: nuklear_shlib,
  dependencies: nuklear_deps,
  compile_args: nuklear_cargs,
  include_directories: include_directories('deps/nuklear'))

libntk_src = ['src/lib/context.c', 'src/lib/error.c', 'src/lib/font.c', 'src/lib/renderer.c']
libntk_headers = ['include/ntk/ntk.h', 'include/ntk/context.h', 'include/ntk/error.h', 'include/ntk/font.h', 'include/ntk/renderer.h']

libntk_inc = include_directories('include')
libntk_deps = [gobject, pango, nuklear]

libntk_enums = gnome.mkenums('ntk-enums', sources: libntk_headers)

libntk_shlib = shared_library('ntk', libntk_src, libntk_enums,
  version: shortver.split('-')[0],
  include_directories: libntk_inc,
  dependencies: libntk_deps,
  install: true)
libntk = declare_dependency(link_with: libntk_shlib,
  include_directories: libntk_inc,
  dependencies: libntk_deps)
libntk_gir = gnome.generate_gir(libntk_shlib,
  dependencies: [libntk_deps],
  namespace: 'Ntk',
  nsversion: shortver.split('-')[0],
  sources: [libntk_src, libntk_enums, libntk_headers],
  header: 'ntk/ntk.h',
  install: true)
libntk_vapi = gnome.generate_vapi('Ntk-@0@'.format(shortver.split('-')[0]),
  packages: ['pango'],
  sources: libntk_gir[0],
  install: true)

subdir('renderers')
