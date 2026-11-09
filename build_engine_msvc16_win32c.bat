@echo off

cmake --preset vs2019_win32
cmake --build --preset build_vs2019_win32_checked
cmake --install __projects\vs2019_win32 --config Checked
