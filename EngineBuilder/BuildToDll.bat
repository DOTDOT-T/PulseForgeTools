@echo off
setlocal enabledelayedexpansion

rem === Création des dossiers ===
if not exist "Object" mkdir "Object"
if not exist "Build" mkdir "Build"
if not exist "Lib" mkdir "Lib"
if not exist "BuildScript" mkdir "BuildScript"

python GenerateBuildBat.py

for %%f in (BuildScript\*.bat) do (
    echo Running %%~nxf...
    call "%%f"
)

call BuildMainToExe.bat

echo.
echo === Build engine dlls finished ===
