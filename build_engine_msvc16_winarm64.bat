@echo off
setlocal
cd /d "%~dp0" || exit /b 1

rem Configure, build and install the standalone AI_Lab4 application (winarm64 Release).
cmake --preset vs2019_winarm64 || exit /b 1
cmake --build --preset build_vs2019_winarm64_release --target AI_Lab4 || exit /b 1
cmake --install "__projects\vs2019_winarm64" --config Release || exit /b 1

exit /b 0
