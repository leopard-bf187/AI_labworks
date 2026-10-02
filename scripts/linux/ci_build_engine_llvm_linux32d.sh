#!/usr/bin/env bash
set -e
cmake --preset llvm_ninja_linux32d
cmake --build --preset build_llvm_ninja_linux32d
cmake --install projects/llvm_ninja_linux32d
