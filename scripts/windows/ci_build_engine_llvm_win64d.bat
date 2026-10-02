@echo off

cmake --preset llvm_ninja_win64d
cmake --build --preset build_llvm_ninja_win64d
cmake --install projects\llvm_ninja_win64d