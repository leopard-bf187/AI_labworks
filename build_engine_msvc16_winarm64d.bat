@echo off

cmake --preset vs2019_winarm64
cmake --build --preset build_vs2019_winarm64_debug
cmake --install __projects\vs2019_winarm64 --config Debug
