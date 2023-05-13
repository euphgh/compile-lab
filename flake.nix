{
  description = "A basic flake with a shell";
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  inputs.flake-utils.url = "github:numtide/flake-utils";

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system: let
      pkgs = nixpkgs.legacyPackages.${system};
    in {
      devShells.default = pkgs.mkShell {
        packages = with pkgs; [ 
        flex 
        bison 
        fmt 
        python38
        xorg.libxcb
        # (python38.withPackages(ps: with ps; [
        # pyqt5
        # sip
        # ]))
        # python38.withPackages.pyqt5
        # python38Packages.pyqt5_sip
        ];
      };
    });
}
