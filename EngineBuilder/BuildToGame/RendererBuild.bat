@echo off
setlocal enabledelayedexpansion

echo.
echo === Linking: Renderer.dll ===
g++ -shared -Wl,--out-implib,../EngineBuilder/Build/Modules\libRenderer.a -o ../EngineBuilder/Build/Modules\Renderer.dll ^
 -DBUILDING_DLL -DPULSE_WINDOWS -DPULSE_GRAPHIC_OPENGL -DENGINE_EDITOR ^
 ../../PulseEngineSource/src/PulseEngine/core/Graphics/OpenGLAPI/OpenGLApi.cpp ^
 ../../PulseEngineSource/src/glad.c ^
 -I"../../PulseEngineSource/include" -I"../../PulseEngineSource/" -I"../../PulseEngineSource/src" -I"../../PulseEngineSource/include/websocketpp" -I"../../PulseEngineSource/include/asio" -I"../../PulseEngineSource/external/assimp/include" ^
-lglfw3 -lopengl32 -lm -lassimp -lcomdlg32 -lws2_32 -lwinmm -lmswsock

if %errorlevel% neq 0 (
    echo Build failed!
    exit /b %errorlevel%
)

echo.
echo === Build terminé  ===
