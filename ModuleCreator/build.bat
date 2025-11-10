@echo off
setlocal enabledelayedexpansion


set CPP_TARGET="EntityEditor"

g++ -DBUILDING_DLL -DENGINE_EDITOR -DPULSE_WINDOWS -DPULSE_GRAPHIC_OPENGL -DIMGUI_IMPL_OPENGL_LOADER_GLAD  -fvisibility=default ^
-shared -I./src -I. -I./PulseEngineEditor -I./include -Iinclude/asio -Iinclude/websocketpp -Iinclude -Iinclude/imgui -Iinclude/imgui-node ^
SourceCodeModule/%CPP_TARGET%/%CPP_TARGET%.cpp ^
include/imgui/imgui_demo.cpp ^
include/imgui/imgui_draw.cpp ^
include/imgui/imgui_tables.cpp ^
include/imgui/imgui_widgets.cpp ^
include/imgui/imgui.cpp ^
include/imgui/backends/imgui_impl_glfw.cpp ^
include/imgui/backends/imgui_impl_opengl3.cpp ^
include/imgui-node/imgui_node_editor.cpp ^
include/imgui-node/imgui_node_editor_api.cpp ^
include/imgui-node/imgui_canvas.cpp ^
include/imgui-node/crude_json.cpp ^
-Lexternal/assimp/lib ^
-Lexternal/assimp/lib/x64 ^
../EngineBuilder/Lib/libPulseEngine.CoreBackend.a ../EngineBuilder/Lib/libPulseEngine.Renderer.a ../EngineBuilder/Lib/libPulseEngine.FileSystem.a ../EngineBuilder/Lib/libPulseEngine.Registery.a ../EngineBuilder/Lib/libPulseLib.InputSystem.a ^
-lglfw3 -lopengl32 -lassimp -lws2_32 -lwinmm -lmswsock ^
-o Output/%CPP_TARGET%.dll

@REM  if %errorlevel% neq 0 (
@REM      echo Échec de la compilation de la DLL !
@REM      exit /b %errorlevel%
@REM  ) else (
@REM      copy /Y "Output/MaterialEditor.dll" "../../PulseEngineSource/Build/Modules/Interface/MaterialEditor.dll"
@REM  )
