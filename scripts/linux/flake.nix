{
  description = "MSU Krystallic Engine";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
    systems.url = "github:nix-systems/default";
    treefmt-nix.url = "github:numtide/treefmt-nix";
  };

  outputs = {
    self,
    nixpkgs,
    flake-utils,
    systems,
    treefmt-nix,
    ...
  }:
    flake-utils.lib.eachSystem (import systems) (system: let
      pkgs = import nixpkgs {inherit system;};
      lib = pkgs.lib;

      packageName = "msu-krystallic-engine";
      repoRoot = ../..;

      treefmtEval = treefmt-nix.lib.evalModule pkgs {
        projectRootFile = "CMakeLists.txt";

        programs.alejandra.enable = true;
        programs.clang-format.enable = true;
        programs.cmake-format.enable = true;

        settings.excludes = [
          "public/freetype/**"
          "src_engine/freetype/**"
          "third_party/**"
        ];
      };

      engine = pkgs.stdenv.mkDerivation {
        pname = packageName;
        version = "0.1.0";

        src = lib.cleanSource repoRoot;

        nativeBuildInputs = with pkgs; [
          cmake
          lua
          ninja
          pkg-config
        ];

        buildInputs = with pkgs; [
          wayland
        ];

        postPatch = ''
          substituteInPlace cmake/engine_target_layout.cmake \
            --replace-fail \
              '"''${SRC_ROOT_DIR}/build/v''${SRC_ENG_VER}/''${SRC_PLATFORM}"' \
              '"build/v''${SRC_ENG_VER}/''${SRC_PLATFORM}"'
        '';

        cmakeFlags = [
          "-DCMAKE_BUILD_TYPE=Release"
          "-DKRYSTALLIC_ENGINE_INSTALL_LAYOUT=OFF"
        ];
      };

      tests = pkgs.stdenv.mkDerivation {
        pname = "${packageName}-tests";
        version = "0.1.0";

        src = lib.cleanSource repoRoot;

        nativeBuildInputs = with pkgs; [
          cmake
          lua
          ninja
          pkg-config
        ];

        buildInputs = with pkgs; [
          wayland
        ];

        postPatch = ''
          substituteInPlace cmake/engine_target_layout.cmake \
            --replace-fail \
              '"''${SRC_ROOT_DIR}/build/v''${SRC_ENG_VER}/''${SRC_PLATFORM}"' \
              '"build/v''${SRC_ENG_VER}/''${SRC_PLATFORM}"'
        '';

        cmakeFlags = [
          "-DCMAKE_BUILD_TYPE=Debug"
          "-DKRYSTALLIC_ENGINE_INSTALL_LAYOUT=OFF"
          "-DKRYSTALLIC_BUILD_TESTS=ON"
        ];

        checkPhase = ''
          runHook preCheck
          ctest --test-dir . --output-on-failure
          runHook postCheck
        '';

        doCheck = true;

        # Skip install — tests only run, outputs not installed
        installPhase = "mkdir -p $out";
      };
    in {
      packages = {
        default = engine;
        engine = engine;
      };

      checks = {
        default = tests;
        tests = tests;
      };

      formatter = treefmtEval.config.build.wrapper;

      devShells.default = pkgs.mkShell {
        inputsFrom = [engine];

        packages = with pkgs;
          [
            clang-tools
            cmake-language-server
            ccache
            lua
            treefmtEval.config.build.wrapper
          ]
          ++ lib.optionals stdenv.isLinux [
            gdb
          ];
      };
    });
}
