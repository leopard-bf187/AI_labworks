@echo off

cmake --preset llvm_ninja_winarm64
cmake --build --preset build_llvm_ninja_winarm64
cmake --install projects\llvm_ninja_winarm64