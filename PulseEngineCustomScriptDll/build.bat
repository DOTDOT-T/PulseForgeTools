@echo off
echo Building scripts into a DLL...

REM Collect all .cpp files from Scripts/
setlocal enabledelayedexpansion
set SOURCES=
for %%f in (Scripts\*.cpp) do (
    set SOURCES=!SOURCES! %%f
)

g++ -std=c++17 -shared -o pulseCustomScripts.dll %SOURCES% ^
-IScripts ^
-Idist\src\PulseEngine\CustomScripts ^
-Idist\include ^
-Idist\src ^
-Idist\src\dllexport ^
-Ldist ^
-Wl,--out-implib,libPulseEngineCustomScripts.a ^
-lPulseEngine ^
-Wall -g -mconsole ^
-DBUILDING_DLL -DPULSE_GRAPHIC_OPENGL -DPULSE_WINDOWS


echo Build complete: pulseCustomScripts.dll
endlocal
