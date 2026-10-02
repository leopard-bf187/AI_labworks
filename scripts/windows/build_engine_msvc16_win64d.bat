@echo off

cmake --preset vs2019_win64
cmake --build --preset build_vs2019_win64_debug
cmake --install projects\vs2019_win64 --config Debug