install_headers([
    'device.h',
    'display.h',
    'drm.h',
    'error.h',
    'input.h'
  ],
  install_dir: libntk_hw_inc_installdir)

configure_file(input: 'build.h.in', output: 'build.h',
  configuration: conf_data,
  install: true,
  install_dir: libntk_hw_inc_installdir)

subdir('drm', if_found: [libdrm])
