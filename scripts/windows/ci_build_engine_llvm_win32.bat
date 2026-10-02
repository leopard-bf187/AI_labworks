@echo off

cmake --preset llvm_ninja_win32
cmake --build --preset build_llvm_ninja_win32
cmake --install projects\llvm_ninja_win32