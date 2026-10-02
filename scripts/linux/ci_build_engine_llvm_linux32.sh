#!/usr/bin/env bash
set -e
cmake --preset llvm_ninja_linux32
cmake --build --preset build_llvm_ninja_linux32
cmake --install projects/llvm_ninja_linux32
