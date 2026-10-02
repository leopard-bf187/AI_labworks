{
  description = "MSU Krystallic Windows clang-cl/MSVC cross-build shell";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = {
    self,
    nixpkgs,
    flake-utils,
    ...
  }:
    flake-utils.lib.eachDefaultSystem (system: let
      pkgs = import nixpkgs {
        inherit system;
        config.allowUnsupportedSystem = true;
      };

      lib = pkgs.lib;
      repoRoot = ../..;

      # Vendored MSVC 14.29.30133 + Windows SDK 10.0.17763.0, matching what
      # devs use locally via localdevenv_*.bat + llvm_msvc16_winsdk17763_*.cmake.
      # Not fetched by Nix: Microsoft doesn't allow public redistribution of the
      # SDK/MSVC libs, so this lives host-local, outside git and outside any
      # nix store shared via a binary cache. Same pattern as ANDROID_NDK_ROOT.
      #
      # Imported via builtins.path (requires --impure) so the resulting store
      # path is a tracked dependency and is visible inside the sandboxed build.
      msvcDevEnvRoot = builtins.path {
        path = /home/yorunikakeru/toolchains/msvc16_winsdk17763;
        name = "msvc16-winsdk17763-devenv";
      };
      msvcVersion = "14.29.30133";
      sdkVersion = "10.0.17763.0";

      # clang-cl / lld-link / llvm-mt / llvm-rc from nixpkgs, combined into one
      # LLVM_ROOT-shaped tree so the shared cmake/toolchains/llvm_msvc16_*.cmake
      # files (also used for native Windows builds) work unmodified.
      llvmMsvcToolchain = pkgs.symlinkJoin {
        name = "llvm-msvc-cross-toolchain";
        paths = [
          pkgs.llvmPackages.clang-unwrapped
          pkgs.llvmPackages.lld
          pkgs.llvmPackages.llvm
        ];
      };
      sourceFilter = path: type: let
        root = toString repoRoot;
        pathString = toString path;
        relPath = lib.removePrefix "${root}/" pathString;
        name = baseNameOf pathString;
      in
        !(
          relPath
          == ".git"
          || lib.hasPrefix ".git/" relPath
          || relPath == "build"
          || lib.hasPrefix "build/" relPath
          || relPath == "projects"
          || lib.hasPrefix "projects/" relPath
          || relPath == "__buildData"
          || lib.hasPrefix "__buildData/" relPath
          || relPath == "docs"
          || lib.hasPrefix "docs/" relPath
          || relPath == ".worktrees"
          || lib.hasPrefix ".worktrees/" relPath
          || relPath == "worktrees"
          || lib.hasPrefix "worktrees/" relPath
          || name == "CMakeCache.txt"
          || name == "CMakeFiles"
          || name == "cmake_install.cmake"
          || name == "install_manifest.txt"
          || name == "compile_commands.json"
        );
      repoSource = lib.cleanSourceWith {
        src = repoRoot;
        filter = sourceFilter;
        name = "msu-krystallic-source";
      };

      mkWindowsCross = {
        name,
        toolchain,
        buildType ? "Release",
        archDir, # "x64" or "x86", matches msvc/Tools/<ver>/lib/<archDir> and winsdk/Lib/<ver>/{ucrt,um}/<archDir>
      }:
        pkgs.stdenv.mkDerivation {
          pname = "msu-krystallic-engine-${name}";
          version = "0.1.0";

          src = repoSource;

          nativeBuildInputs = [
            pkgs.cmake
            pkgs.ninja
            pkgs.pkg-config
            llvmMsvcToolchain
          ];

          # Host-local, unfetchable (Microsoft doesn't allow public
          # redistribution of MSVC/WinSDK), so this needs --impure.
          LLVM_ROOT = llvmMsvcToolchain;
          MSVC_ROOT = "${msvcDevEnvRoot}/msvc/Tools/${msvcVersion}";
          SDK_ROOT = "${msvcDevEnvRoot}/winsdk";
          SDK_VER = sdkVersion;

          postPatch = ''
            substituteInPlace cmake/engine_target_layout.cmake \
              --replace-fail \
                '"''${SRC_ROOT_DIR}/build/v''${SRC_ENG_VER}/''${SRC_PLATFORM}"' \
                '"build/v''${SRC_ENG_VER}/''${SRC_PLATFORM}"'
          '';

          configurePhase = ''
            runHook preConfigure
            cmake -B build -S . \
              -G Ninja \
              -DCMAKE_TOOLCHAIN_FILE="$PWD/cmake/toolchains/${toolchain}" \
              -DCMAKE_BUILD_TYPE=${buildType} \
              -DCMAKE_INSTALL_PREFIX="$out"
            runHook postConfigure
          '';

          buildPhase = ''
            runHook preBuild
            cmake --build build
            runHook postBuild
          '';

          installPhase = ''
            runHook preInstall
            cmake --install build --prefix "$out"
            runHook postInstall
          '';
        };

      windowsPackages = {
        msvc-win64-release = mkWindowsCross {
          name = "msvc-win64-release";
          toolchain = "llvm_msvc16_winsdk17763_win64.cmake";
          buildType = "Release";
          archDir = "x64";
        };
        msvc-win64-debug = mkWindowsCross {
          name = "msvc-win64-debug";
          toolchain = "llvm_msvc16_winsdk17763_win64.cmake";
          buildType = "Debug";
          archDir = "x64";
        };
        msvc-win32-release = mkWindowsCross {
          name = "msvc-win32-release";
          toolchain = "llvm_msvc16_winsdk17763_win32.cmake";
          buildType = "Release";
          archDir = "x86";
        };
        msvc-win32-debug = mkWindowsCross {
          name = "msvc-win32-debug";
          toolchain = "llvm_msvc16_winsdk17763_win32.cmake";
          buildType = "Debug";
          archDir = "x86";
        };
      };

      checkWin64 = pkgs.writeShellApplication {
        name = "check-msvc-win64";
        runtimeInputs = [
          pkgs.nix
        ];
        text = ''
          set -euo pipefail
          nix build --impure '${self}#msvc-win64-release'
          nix build --impure '${self}#msvc-win64-debug'
        '';
      };

      checkWin32 = pkgs.writeShellApplication {
        name = "check-msvc-win32";
        runtimeInputs = [
          pkgs.nix
        ];
        text = ''
          set -euo pipefail
          nix build --impure '${self}#msvc-win32-release'
          nix build --impure '${self}#msvc-win32-debug'
        '';
      };
    in {
      packages =
        windowsPackages
        // {
          default = windowsPackages.msvc-win64-release;
          check-msvc-win64 = checkWin64;
          check-msvc-win32 = checkWin32;
        };

      apps = {
        check-msvc-win64 = {
          type = "app";
          program = "${checkWin64}/bin/check-msvc-win64";
        };
        check-msvc-win32 = {
          type = "app";
          program = "${checkWin32}/bin/check-msvc-win32";
        };
      };

      checks =
        windowsPackages
        // {
          default = windowsPackages.msvc-win64-release;
        };

      devShells.default = pkgs.mkShell {
        packages = [
          pkgs.cmake
          pkgs.ninja
          pkgs.pkg-config
          llvmMsvcToolchain
        ];

        shellHook = ''
          echo "MSU Krystallic Windows clang-cl/MSVC cross-compilation shell"
          echo "Requires LLVM_ROOT / MSVC_ROOT / SDK_ROOT / SDK_VER env vars (see cmake/toolchains/llvm_msvc16_winsdk17763_*.cmake)"
          echo "Example: cmake -B build -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/llvm_msvc16_winsdk17763_win64.cmake && cmake --build build"
        '';
      };

      formatter = pkgs.alejandra;
    });
}
