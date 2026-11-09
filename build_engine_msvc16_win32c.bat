@echo off
setlocal
cd /d "%~dp0" || exit /b 1

rem Configure, build and install the standalone AI_Lab4 application (win32 Checked).
cmake --preset vs2019_win32 || exit /b 1
cmake --build --preset build_vs2019_win32_checked --target AI_Lab4 || exit /b 1
cmake --install "__projects\vs2019_win32" --config Checked || exit /b 1

exit /b 0
