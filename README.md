# ntk

Nuklear Toolkit

## Building

### Dependencies

#### Host

These dependencies must be met in order to build on the host machine.

- `meson`
- `ninja`
- `gobject-introspection-1.0` (disable with `-Dgir=disabled`, default: `auto`)
- `valac` (disable with `-Dvala=disabled`, default: `auto`)

#### Target

These dependencies must be present for the target machine to build Ntk.

- `glib-2.0`
- `gio-2.0`
- `gtk4` (disable with `-Dgtk4=disabled`, default: `auto`)
  - Must also need `libtokyo` and `libadwaita` if building examples
- `libglvnd` (disable with `-Degl=disabled`, default: `auto`)
- `cairo` (disable with `-Dcairo=disabled`, default: `auto`)

### Getting the source code

Ntk is hosted on GitHub at `ExpidusOS/ntk`, it is recommended to use `git` to clone the repository and switching
to the ref your wishing to use. Once you have the repository clones, you must pull the submodules using `git`
in order to build Nuklear for Ntk.

### Configuration & Building

Ntk uses Meson and so the extra configuration options can be found in `meson_options.txt`. Ninja can then
be used to build Ntk and install it. However, it is recommended to use the nix flake to build the package
and use it in other projects.
