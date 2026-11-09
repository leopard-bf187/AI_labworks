@echo off

cmake --preset vs2019_win64
cmake --build --preset build_vs2019_win64_release
cmake --install __projects\vs2019_win64 --config Release
