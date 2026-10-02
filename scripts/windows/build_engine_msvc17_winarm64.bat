@echo off

cmake --preset vs2022_winarm64
cmake --build --preset build_vs2022_winarm64_release
cmake --install projects\vs2022_winarm64 --config Release