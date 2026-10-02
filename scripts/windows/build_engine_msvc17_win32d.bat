@echo off

cmake --preset vs2022_win32
cmake --build --preset build_vs2022_win32_debug
cmake --install projects\vs2022_win32 --config Debug

