@echo off

cmake --preset vs2019_win32
cmake --build --preset build_vs2019_win32_debug
cmake --install projects\vs2019_win32 --config Debug

