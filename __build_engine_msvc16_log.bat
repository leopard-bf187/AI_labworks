@echo off
setlocal
cd /d "%~dp0" || exit /b 1
chcp 65001 >nul
if not exist "__build" mkdir "__build" || exit /b 1

rem Build each configuration, capturing both standard output and errors.
call :BuildAndLog "build_engine_msvc16_win32d.bat" "__build\build_msvc16_win32d.log" || exit /b 1
call :BuildAndLog "build_engine_msvc16_win64d.bat" "__build\build_msvc16_win64d.log" || exit /b 1
call :BuildAndLog "build_engine_msvc16_winarm64d.bat" "__build\build_msvc16_winarm64d.log" || exit /b 1
call :BuildAndLog "build_engine_msvc16_win32c.bat" "__build\build_msvc16_win32c.log" || exit /b 1
call :BuildAndLog "build_engine_msvc16_win64c.bat" "__build\build_msvc16_win64c.log" || exit /b 1
call :BuildAndLog "build_engine_msvc16_winarm64c.bat" "__build\build_msvc16_winarm64c.log" || exit /b 1
call :BuildAndLog "build_engine_msvc16_win32.bat" "__build\build_msvc16_win32.log" || exit /b 1
call :BuildAndLog "build_engine_msvc16_win64.bat" "__build\build_msvc16_win64.log" || exit /b 1
call :BuildAndLog "build_engine_msvc16_winarm64.bat" "__build\build_msvc16_winarm64.log" || exit /b 1

echo All AI_Lab4 builds completed. Logs are in __build.
exit /b 0

:BuildAndLog
echo Building %~1 ...
call "%~dp0%~1" > "%~2" 2>&1
if errorlevel 1 (
    echo FAILED: %~1. Details: %~2
    exit /b 1
)
echo OK: %~1
exit /b 0
