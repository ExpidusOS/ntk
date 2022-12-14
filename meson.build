project('ntk', 'c', license: 'GPL-3.0-only', version: '0.1.0-prealpha')

gnome = import('gnome')
pkg = import('pkgconfig')
fs = import('fs')

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

enable_vala = get_option('vala')
vala = find_program('valac', native: true, required: enable_vala)
use_vala = vala.found()

enable_gir = get_option('gir')
gir = dependency('gobject-introspection-1.0', required: enable_gir)
use_gir = gir.found()

if use_vala
  add_project_arguments(['--vapidir', meson.current_build_dir()], language: 'vala')
  add_languages('vala')
endif

little_endian = target_machine.endian() == 'little'
big_endian = target_machine.endian() == 'big'

if target_machine.system() != 'darwin'
  add_project_arguments([
    '-DLITTLE_ENDIAN=@0@'.format(little_endian.to_int()),
    '-DBIG_EDIAN=@0@'.format(big_endian.to_int())
  ], language: 'c')
endif

glib = [dependency('glib-2.0'), dependency('gio-2.0')]
gobject = dependency('gobject-2.0')
pango = dependency('pango', required: false)
libmath = meson.get_compiler('c').find_library('m', required: false)
libdrm = dependency('libdrm', required: get_option('drm'))
cssparser = dependency('cssparser', required: get_option('styling_css'))
cairo = dependency('cairo', required: get_option('cairo'))
pangoft2 = dependency('pangoft2', required: false)
gles2 = dependency('glesv2', required: get_option('gles2').require(pangoft2.found()))
egl = dependency('egl', required: get_option('egl').require(not get_option('gles2').disabled()))
gtk4 = dependency('gtk4', required: get_option('gtk4'))

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

subdir('nuklear')
subdir('core')
subdir('fonts')
subdir('styling')
subdir('hw')
subdir('backends')
