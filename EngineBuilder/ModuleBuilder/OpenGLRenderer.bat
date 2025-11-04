@echo off
setlocal enabledelayedexpansion

set "FLAGS=-g -DBUILDING_DLL -DPULSE_WINDOWS -DPULSE_GRAPHIC_OPENGL -DENGINE_EDITOR"

g++ -shared -Wl,--out-implib,Lib\libOpenGlRenderer.a -o Build\Modules\Renderer.dll %FLAGS% ^
    ../../PulseEngineSource/src/shader.cpp ^
    ../../PulseEngineSource/src/PulseEngine/core/Graphics/OpenGLAPI/OpenGLApi.cpp ^
    ../../PulseEngineSource/src/glad.c ^
    -I../../PulseEngineSource/src ^
    -I../../PulseEngineSource/include ^
    -I../../PulseEngineSource/include/glad ^
    -I../../PulseEngineSource/include/imgui ^
    -I../../PulseEngineSource/include/imgui-node ^
    -I../../PulseEngineSource/imgui/backends ^
    -lglfw3 -lopengl32 -lm -lassimp -lcomdlg32 -lws2_32 -lwinmm -lmswsock
