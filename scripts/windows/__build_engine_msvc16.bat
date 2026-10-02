@echo off
call scripts\windows\build_engine_msvc16_win32d.bat
call scripts\windows\build_engine_msvc16_win64d.bat
call scripts\windows\build_engine_msvc16_winarm64d.bat

call scripts\windows\build_engine_msvc16_win32.bat
call scripts\windows\build_engine_msvc16_win64.bat
call scripts\windows\build_engine_msvc16_winarm64.bat