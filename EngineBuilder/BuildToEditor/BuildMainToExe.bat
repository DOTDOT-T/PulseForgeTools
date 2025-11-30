@echo off
setlocal enabledelayedexpansion

set "LIBS_FILES="
for %%f in (Lib\*.a) do (
    set "LIBS_FILES=!LIBS_FILES! -l:%%~nxf"
)

echo "used libs in the project : %LIBS_FILES%"

rem === Compilation du test ===
g++ ^
    main.cpp ^
    ../../PulseEngineSource/include/imgui/imgui_demo.cpp ^
    ../../PulseEngineSource/include/imgui/imgui_draw.cpp ^
    ../../PulseEngineSource/include/imgui/imgui_tables.cpp ^
    ../../PulseEngineSource/include/imgui/imgui_widgets.cpp ^
    ../../PulseEngineSource/include/imgui/imgui.cpp ^
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
    -lglfw3 -lopengl32 -lm -lassimp -lcomdlg32 -lws2_32 -lwinmm -lmswsock ^
    -o Build/Game.exe

if %errorlevel% neq 0 (
    echo Build failed!
    exit /b %errorlevel%
) else (
    echo Build succeeded!
)