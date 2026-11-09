@echo off
setlocal
cd /d "%~dp0" || exit /b 1

rem Build Debug, Checked and Release for Win32, Win64 and WinArm64.
call "%~dp0build_engine_msvc16_win32d.bat" || exit /b 1
call "%~dp0build_engine_msvc16_win64d.bat" || exit /b 1
call "%~dp0build_engine_msvc16_winarm64d.bat" || exit /b 1
call "%~dp0build_engine_msvc16_win32c.bat" || exit /b 1
call "%~dp0build_engine_msvc16_win64c.bat" || exit /b 1
call "%~dp0build_engine_msvc16_winarm64c.bat" || exit /b 1
call "%~dp0build_engine_msvc16_win32.bat" || exit /b 1
call "%~dp0build_engine_msvc16_win64.bat" || exit /b 1
call "%~dp0build_engine_msvc16_winarm64.bat" || exit /b 1

echo All AI_Lab4 MSVC 2019 builds completed.
exit /b 0
