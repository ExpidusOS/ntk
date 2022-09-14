{
  description = "Nuklear Toolkit";

  inputs.libtokyo = {
    url = github:ExpidusOS/libtokyo;
    inputs.nixpkgs.follows = "nixpkgs";
  };

  outputs = { self, libtokyo, nixpkgs, ... }:
    let
      supportedSystems = builtins.attrNames libtokyo.packages;
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
            buildInputs = with pkgs; [ cairo gtk4 ];
          };
        });

      devShells = forAllSystems (system:
        let
          pkgs = nixpkgsFor.${system};
          tokyo = libtokyo.packages.${system}.gtk4;

          linuxPackages = with pkgs; [
            libdrm
            libglvnd
          ];

          darwinPackages = with pkgs; [
            libglvnd
            mesa
            mesa.drivers
          ];

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
              gobject-introspection
              meson
              ninja
              pkg-config
              glib
              gtk4
              tokyo
              vala
              libadwaita
            ] ++ packagesForSystem;
          };
        });
    };
}
