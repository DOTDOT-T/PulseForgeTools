@echo off
echo === Checking libPulseEngineEditor.a for Shader symbols ===
nm ..\ModuleCreator\lib\libPulseEngineEditor.a | findstr Shader

echo.
echo === Compiling EntityEditor.cpp to object file (no linking) ===
g++ -DBUILDING_DLL -DPULSE_WINDOWS -DPULSE_GRAPHIC_OPENGL -fvisibility=default ^
-I./src -I. -I./PulseEngineEditor -I./include -Iimgui ^
-c SourceCodeModule/EntityEditor/EntityEditor.cpp -o EntityEditor.o

echo.
echo === Checking EntityEditor.o for Shader symbols ===
nm EntityEditor.o | findstr Shader

echo.
echo === DONE ===
pause
