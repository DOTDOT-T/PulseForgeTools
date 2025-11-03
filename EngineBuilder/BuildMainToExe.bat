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
    ../../PulseEngineSource/include/imgui/backends/imgui_impl_glfw.cpp ^
    ../../PulseEngineSource/include/imgui/backends/imgui_impl_opengl3.cpp ^
    ../../PulseEngineSource/include/imgui-node/imgui_node_editor.cpp ^
    ../../PulseEngineSource/include/imgui-node/imgui_node_editor_api.cpp ^
    ../../PulseEngineSource/include/imgui-node/imgui_canvas.cpp ^
    ../../PulseEngineSource/include/imgui-node/crude_json.cpp ^
    -I../../PulseEngineSource/src -I../../PulseEngineSource/include -I../../PulseEngineSource/. -I../../PulseEngineSource/include/imgui -I../../PulseEngineSource/include/imgui-node -I../../PulseEngineSource/imgui/backends ^
    -L../../PulseEngineSource/Build -l../../PulseEngineSource/Build/PulseEngineEditor ^
    -LLib %LIBS_FILES% ^
    -DPULSE_WINDOWS -DENGINE_EDITOR -DPULSE_GRAPHIC_OPENGL -DPULSE_WINDOWS -DIMGUI_IMPL_OPENGL_LOADER_GLAD ^
    -o Build/Game.exe

if %errorlevel% neq 0 (
    echo Build failed!
    exit /b %errorlevel%
) else (
    echo Build succeeded!
)
