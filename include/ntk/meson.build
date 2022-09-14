libntk_inc_installdir = join_paths(prefix, 'include', 'ntk-@0@'.format(shortver), 'ntk')

install_headers([
    'context.h',
    'error.h',
    'font.h',
    'input-dispatcher.h',
    'ntk.h',
    'renderer.h',
    'utils.h'
  ],
  install_dir: libntk_inc_installdir)

configure_file(input: 'build.h.in', output: 'build.h',
  configuration: conf_data,
  install: true,
  install_dir: libntk_inc_installdir)

subdir('font')
