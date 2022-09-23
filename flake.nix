{
  description = "Nuklear Toolkit";

  outputs = { self, nixpkgs, ... }:
    let
      supportedSystems = [
        "aarch64-linux"
        "i686-linux"
        "riscv64-linux"
        "x86_64-linux"
        "x86_64-darwin"
      ];
      forAllSystems = nixpkgs.lib.genAttrs supportedSystems;
      nixpkgsFor = forAllSystems (system: import nixpkgs { inherit system; });
    in
    {
      packages = forAllSystems (system:
        let
          pkgs = nixpkgsFor.${system};
          mkDerivation = ({ name, buildInputs }: pkgs.stdenv.mkDerivation rec {
            inherit name buildInputs;

            src = self;
            enableParallelBuilding = true;
            nativeBuildInputs = with pkgs; [ meson vala ninja pkg-config gobject-introspection ];

            meta = with pkgs.lib; {
              homepage = "https://github.com/ExpidusOS/ntk";
              license = with licenses; [ gpl3Only ];
              maintainers = [ "Tristan Ross" ];
            };
          });
        in {
          default = mkDerivation {
            name = "ntk";
            buildInputs = with pkgs; [ cairo gtk4 libglvnd ];
          };

          cairo = mkDerivation {
            name = "ntk-cairo";
            buildInputs = with pkgs; [ cairo ];
          };

          gtk4 = mkDerivation {
            name = "ntk-gtk4";
            buildInputs = with pkgs; [ gtk4 ];
          };

          gl = mkDerivation {
            name = "ntk-gl";
            buildInputs = with pkgs; [ libglvnd ];
          };
        });

      mkDerivation = ({ name ? "ntk", buildInputs, system ? builtins.currentSystem }:
        let
          pkgs = nixpkgsFor.${system};
        in
        pkgs.stdenv.mkDerivation rec {
          inherit name buildInputs;

          src = self;
          enableParallelBuilding = true;
          nativeBuildInputs = with pkgs; [ meson vala ninja pkg-config gobject-introspection ];

          meta = with pkgs.lib; {
            homepage = "https://github.com/ExpidusOS/ntk";
            license = with licenses; [ gpl3Only ];
            maintainers = [ "Tristan Ross" ];
          };
        });

      devShells = forAllSystems (system:
        let
          pkgs = nixpkgsFor.${system};

          linuxPackages = {
            default = with pkgs; [
              libdrm
              libglvnd
            ];
            minimal = with pkgs; [
              libdrm
            ];
          };

          darwinPackages = {
            default = with pkgs; [
              libglvnd
              mesa
              mesa.drivers
            ];
            minimal = with pkgs; [
              mesa
            ];
          };

          systemPackages = {
            aarch64-linux = linuxPackages;
            i686-linux = linuxPackages;
            riscv64-linux = linuxPackages;
            x86_64-linux = linuxPackages;
            x86_64-darwin = darwinPackages;
          };

          packagesForSystem = systemPackages.${system};
        in {
          default = pkgs.mkShell {
            buildInputs = with pkgs; [
              clang_14
              gdb
              gobject-introspection
              meson
              ninja
              pkg-config
              glib
              gtk4
              vala
              libadwaita
            ] ++ packagesForSystem.default;
          };
          minimal = pkgs.mkShell {
            buildInputs = with pkgs; [
              pango
              clang_14
              gdb
              gobject-introspection
              meson
              ninja
              pkg-config
              glib
              vala
            ] ++ packagesForSystem.default;
          };
        });
    };
}
