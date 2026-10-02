@echo off

cmake --preset llvm_ninja_win32d
cmake --build --preset build_llvm_ninja_win32d
cmake --install projects\llvm_ninja_win32d