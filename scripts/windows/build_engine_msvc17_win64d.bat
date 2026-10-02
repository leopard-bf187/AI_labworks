@echo off

cmake --preset vs2022_win64
cmake --build --preset build_vs2022_win64_debug
cmake --install projects\vs2022_win64 --config Debug