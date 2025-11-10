@echo off
setlocal enabledelayedexpansion

set "LIBS_FILES="
for %%f in (LibGame\*.a) do (
    set "LIBS_FILES=!LIBS_FILES! -l:%%~nxf"
)

echo "used libs in the project : %LIBS_FILES%"

rem === Compilation du test ===
g++ ^
    main.cpp ^
    -I../../PulseEngineSource/src -I../../PulseEngineSource/include -I../../PulseEngineSource/. ^
    -L../../PulseEngineSource/Build -l../../PulseEngineSource/Build/PulseEngineEditor ^
    -LLib %LIBS_FILES% ^
    -DPULSE_WINDOWS -DPULSE_GRAPHIC_OPENGL -DPULSE_WINDOWS -DIMGUI_IMPL_OPENGL_LOADER_GLAD ^
    -lglfw3 -lopengl32 -lm -lassimp -lcomdlg32 -lws2_32 -lwinmm -lmswsock ^
    -o BuildGame/Game.exe

if %errorlevel% neq 0 (
    echo Build failed!
    exit /b %errorlevel%
) else (
    echo Build succeeded!
)
