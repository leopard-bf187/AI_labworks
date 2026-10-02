@echo off

call scripts\windows\build_engine_msvc17_win32d.bat
call scripts\windows\build_engine_msvc17_win64d.bat
call scripts\windows\build_engine_msvc17_winarm64d.bat

call scripts\windows\build_engine_msvc17_win32.bat
call scripts\windows\build_engine_msvc17_win64.bat
call scripts\windows\build_engine_msvc17_winarm64.bat
