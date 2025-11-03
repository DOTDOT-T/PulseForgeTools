@echo off
setlocal enabledelayedexpansion

if not exist tests mkdir tests

set "LIBS_FILES="
for %%f in (Lib\*.a) do (
    set "LIBS_FILES=!LIBS_FILES! -l:%%~nxf"
)

echo %LIBS_FILES%

rem === Compilation du test ===
g++ ^
    main.cpp ^
    -LLib %LIBS_FILES% -I../../PulseEngineSource/src -I../../PulseEngineSource/include ^
    -DPULSE_WINDOWS ^
    -o Build/Game.exe

if %errorlevel% neq 0 (
    echo Build failed!
    exit /b %errorlevel%
) else (
    echo Build succeeded!
)
