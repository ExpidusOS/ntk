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
  'src/nuklear/src/nuklear_combo.c', 'src/nuklear/src/nuklear_menu.c',
  'src/nuklear/src/nuklear_group.c', 'src/nuklear/src/nuklear_progress.c',
  'src/nuklear/src/nuklear_util.c', 'src/nuklear/src/nuklear_slider.c',
  'src/nuklear/src/nuklear_window.c', 'src/nuklear/src/nuklear_math.c',
  'src/nuklear/src/nuklear_property.c', 'src/nuklear/src/nuklear_toggle.c',
  'src/nuklear/src/nuklear_page_element.c', 'src/nuklear/src/nuklear_color.c',
  'src/nuklear/src/nuklear_vertex.c', 'src/nuklear/src/nuklear_tree.c',
  'src/nuklear/src/nuklear_font.c', 'src/nuklear/src/nuklear_chart.c',
  'src/nuklear/src/nuklear_widget.c', 'src/nuklear/src/nuklear_string.c',
  'src/nuklear/src/nuklear_input.c', 'src/nuklear/src/nuklear_pool.c',
  'src/nuklear/src/nuklear_buffer.c', 'src/nuklear/src/nuklear_color_picker.c',
  'src/nuklear/src/nuklear_draw.c', 'src/nuklear/src/nuklear_context.c',
  'src/nuklear/src/nuklear_table.c', 'src/nuklear/src/nuklear_contextual.c',
  'src/nuklear/src/nuklear_edit.c', 'src/nuklear/src/nuklear_utf8.c',
  'src/nuklear/src/nuklear_scrollbar.c', 'src/nuklear/src/nuklear_panel.c',
  'src/nuklear/src/nuklear_list_view.c', 'src/nuklear/src/nuklear_text.c',
  'src/nuklear/src/nuklear_9slice.c', 'src/nuklear/src/nuklear_text_editor.c',
  'src/nuklear/src/nuklear_popup.c', 'src/nuklear/src/nuklear_tooltip.c',
  'src/nuklear/src/nuklear_selectable.c', 'src/nuklear/src/nuklear_button.c',
  'src/nuklear/src/nuklear_image.c', 'src/nuklear/src/nuklear_style.c',
  'src/nuklear/src/nuklear_layout.c', 'src/nk_utils.c'
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
  include_directories: include_directories('src/nuklear'))

nuklear_pc_cflags = nuklear_cflags
nuklear_pc_cflags += ['-I${includedir}/ntk-@0@'.format(shortver)]
pkg.generate(nuklear_shlib, description: 'A minimal state immediate mode graphical user interface toolkit.',
  name: 'nuklear',
  url: 'https://github.com/Immediate-Mode-UI/Nuklear',
  version: nuklear_version,
  extra_cflags: nuklear_pc_cflags)

if use_vala
  nuklear_vapi = meson.get_compiler('vala').find_library('nuklear', dirs: join_paths(meson.current_source_dir(), 'vapi'))
  install_data('vapi/nuklear.vapi', install_dir: join_paths(datadir, 'vala', 'vapi'))
endif

install_data('src/nuklear/nuklear.h', install_dir: join_paths(prefix, 'include', 'ntk-@0@'.format(shortver)))
