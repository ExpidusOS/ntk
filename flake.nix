{
  description = "Nuklear Toolkit";

  inputs.cssparser = {
    url = github:ExpidusOS/cssparser/feat/nix;
    inputs.nixpkgs.follows = "nixpkgs";
  };

  inputs.expidus-sdk = {
    url = github:ExpidusOS/sdk;
    inputs.nixpkgs.follows = "nixpkgs";
  };

  outputs = { self, nixpkgs, cssparser, expidus-sdk, ... }:
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
      src = self // { submodules = true; };
    in
    {
      packages = forAllSystems (system:
        let
          pkgs = nixpkgsFor.${system};
          cssparser-pkg = cssparser.packages.${system}.default;
          expidus-sdk-pkg = expidus-sdk.packages.${system}.default;

          linuxPackages = {
            default = with pkgs; [
              libdrm
            ];
            cairo = [];
            gtk4 = [];
            gl = with pkgs; [
              libdrm
            ];
          };

          darwinPackages = {
            default = with pkgs; [
              mesa
              mesa.drivers
            ];
            cairo = [];
            gtk4 = [];
            gl = with pkgs; [
              mesa
              mesa.drivers
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

          linuxFlags = {
            default = ["-Ddrm=enabled"];
            cairo = ["-Ddrm=disabled"];
            gtk4 = ["-Ddrm=disabled"];
            gl = ["-Ddrm=enabled"];
          };

          darwinFlags = {
            default = ["-Ddrm=disabled"];
            cairo = ["-Ddrm=disabled"];
            gtk4 = ["-Ddrm=disabled"];
            gl = ["-Ddrm=disabled"];
          };

          systemFlags = {
            aarch64-linux = linuxFlags;
            i686-linux = linuxFlags;
            riscv64-linux = linuxFlags;
            x86_64-linux = linuxFlags;
            x86_64-darwin = darwinFlags;
          };

          flagsForSystem = systemFlags.${system};

          mkDerivation = ({ name, buildInputs, mesonFlags ? [] }: pkgs.stdenv.mkDerivation {
            inherit name buildInputs src;

            mesonFlags = mesonFlags ++ ["-Dexamples=false" "-Dcairo_examples=disabled" "-Dgtk4_examples=disabled" "-Degl_examples=disabled"];
            enableParallelBuilding = true;
            nativeBuildInputs = with pkgs; [ meson vala ninja pkg-config gobject-introspection expidus-sdk-pkg ];

            meta = with pkgs.lib; {
              homepage = "https://github.com/ExpidusOS/ntk";
              license = with licenses; [ gpl3Only ];
              maintainers = with expidus-sdk.lib.maintainers; [ TheComputerGuy ];
            };
          });
        in {
          default = mkDerivation {
            name = "ntk";
            mesonFlags = flagsForSystem.default;
            buildInputs = with pkgs; [ cairo gtk4 libglvnd cssparser-pkg ] ++ packagesForSystem.default;
          };

          cairo = mkDerivation {
            name = "ntk-cairo";
            mesonFlags = flagsForSystem.cairo;
            buildInputs = with pkgs; [ cairo cssparser-pkg ] ++ packagesForSystem.cairo;
          };

          gtk4 = mkDerivation {
            name = "ntk-gtk4";
            mesonFlags = flagsForSystem.gtk4;
            buildInputs = with pkgs; [ gtk4 cssparser-pkg ] ++ packagesForSystem.gtk4;
          };

          gl = mkDerivation {
            name = "ntk-gl";
            mesonFlags = flagsForSystem.gl;
            buildInputs = with pkgs; [ libglvnd cssparser-pkg ] ++ packagesForSystem.gl;
          };
        });

      mkDerivation = ({ name ? "ntk", buildInputs, system ? builtins.currentSystem }:
        let
          pkgs = nixpkgsFor.${system};
          cssparser-pkg = cssparser.packages.${system}.default;
        in
        pkgs.stdenv.mkDerivation rec {
          inherit name buildInputs src;

          enableParallelBuilding = true;
          nativeBuildInputs = with pkgs; [ meson vala ninja pkg-config gobject-introspection cssparser-pkg ];

          meta = with pkgs.lib; {
            homepage = "https://github.com/ExpidusOS/ntk";
            license = with licenses; [ gpl3Only ];
            maintainers = [ "Tristan Ross" ];
          };
        });

      devShells = forAllSystems (system:
        let
          pkgs = nixpkgsFor.${system};
          cssparser-pkg = cssparser.packages.${system}.default;
          expidus-sdk-pkg = expidus-sdk.packages.${system}.default;

          linuxPackages = {
            default = with pkgs; [
              libdrm
            ];
            minimal = with pkgs; [
              libdrm
            ];
          };

          darwinPackages = {
            default = with pkgs; [
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
              gdb
              gobject-introspection
              meson
              ninja
              pkg-config
              glib
              gtk4
              vala
              libadwaita
              libglvnd
              cssparser-pkg
              expidus-sdk-pkg
            ] ++ packagesForSystem.default;
          };
          minimal = pkgs.mkShell {
            buildInputs = with pkgs; [
              pango
              gdb
              gobject-introspection
              meson
              ninja
              pkg-config
              glib
              vala
              cssparser-pkg
              expidus-sdk-pkg
            ] ++ packagesForSystem.default;
          };
        });

      submodules = true;
    };
}
