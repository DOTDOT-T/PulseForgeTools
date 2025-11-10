@echo off
setlocal

set "rebuildExe=%1"

rem === Création des dossiers ===
if not exist "ObjectGame" mkdir "ObjectGame"
if not exist "BuildGame" mkdir "BuildGame"
if not exist "LibGame" mkdir "LibGame"
if not exist "BuildScriptGame" mkdir "BuildScriptGame"

python BuildToGame/GenerateBuildBat.py

for %%f in (BuildScriptGame\*.bat) do (
    echo Running %%~nxf...
    call "%%f"
)

if "%rebuildExe%"=="rebuildExe" (
    call BuildToGame/BuildMainToExe.bat
)

echo.
echo === Build engine dlls finished ===
