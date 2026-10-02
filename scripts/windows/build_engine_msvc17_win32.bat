@echo off

cmake --preset vs2022_win32
cmake --build --preset build_vs2022_win32_release
cmake --install projects\vs2022_win32 --config Release