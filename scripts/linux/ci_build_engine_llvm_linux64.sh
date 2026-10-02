#!/usr/bin/env bash
set -e
cmake --preset llvm_ninja_linux64
cmake --build --preset build_llvm_ninja_linux64
cmake --install projects/llvm_ninja_linux64
