@echo off
setlocal

set "rebuildExe=%1"

rem === Création des dossiers ===
if not exist "Object" mkdir "Object"
if not exist "Build" mkdir "Build"
if not exist "Lib" mkdir "Lib"
if not exist "BuildScriptEditor" mkdir "BuildScriptEditor"

python BuildToEditor/GenerateBuildBat.py

for %%f in (BuildScriptEditor\*.bat) do (
    echo Running %%~nxf...
    call "%%f"
)

if "%rebuildExe%"=="rebuildExe" (
    call BuildToEditor/BuildMainToExe.bat
)

echo.
echo === Build engine dlls finished ===
