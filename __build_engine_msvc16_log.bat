@echo off

chcp 65001

if not exist __build mkdir __build


echo Building MSVC16 Win32 Debug:
call scripts\windows\build_engine_msvc16_win32d.bat > __build\build_msvc16_win32d.log

echo Building MSVC16 Win32 Checked:
call scripts\windows\build_engine_msvc16_win32c.bat > __build\build_msvc16_win32c.log

echo Building MSVC16 Win32 Release:
call scripts\windows\build_engine_msvc16_win32.bat > __build\build_msvc16_win32.log


echo Building MSVC16 Win64 Debug:
call scripts\windows\build_engine_msvc16_win64d.bat > __build\build_msvc16_win64d.log

echo Building MSVC16 Win64 Checked:
call scripts\windows\build_engine_msvc16_win64c.bat > __build\build_msvc16_win64c.log

echo Building MSVC16 Win64 Release:
call scripts\windows\build_engine_msvc16_win64.bat > __build\build_msvc16_win64.log


echo Building MSVC16 WinArm64 Debug:
call scripts\windows\build_engine_msvc16_winarm64d.bat > __build\build_msvc16_winarm64d.log

echo Building MSVC16 WinArm64 Checked:
call scripts\windows\build_engine_msvc16_winarm64c.bat > __build\build_msvc16_winarm64c.log

echo Building MSVC16 WinArm64 Release:
call scripts\windows\build_engine_msvc16_winarm64.bat > __build\build_msvc16_winarm64.log
