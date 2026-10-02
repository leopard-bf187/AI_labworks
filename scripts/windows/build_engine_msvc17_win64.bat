@echo off

cmake --preset vs2022_win64
cmake --build --preset build_vs2022_win64_release
cmake --install projects\vs2022_win64 --config Release