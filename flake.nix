{
	description = "Nuklear Toolkit";

	outputs = { self, nixpkgs, ... }:
		let
			supportedSystems = [
				"aarch64-linux"
        "i686-linux"
        "riscv64-linux"
        "x86_64-linux"
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
						buildInputs = with pkgs; [ cairo gtk4 ];
					};
				});

			devShells = forAllSystems (system:
				let
					pkgs = nixpkgsFor.${system};
				in {
					default = pkgs.mkShell {
						buildInputs = with pkgs; [
							gobject-introspection
							meson
							ninja
							pkg-config
							glib
							gtk4
							vala
						];
					};
				});
		};
}
