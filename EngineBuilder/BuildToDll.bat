@echo off
setlocal enabledelayedexpansion

rem === Configuration ===
set "DLL_TARGET=engine"
set "ENGINE_SOURCE=../PulseForge/PulseEngine/"
set "OBJ_DIR=Object"
set "BUILD_DIR=Build"
set "LIB_DIR=Lib"

rem === Création des dossiers ===
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
if not exist "%LIB_DIR%" mkdir "%LIB_DIR%"
if not exist "%OBJ_DIR%" mkdir "%OBJ_DIR%"

rem === List of .cpp files to compile ===
set "SRC_FILES=%ENGINE_SOURCE%src\PulseEngine\core\Math\Frustum\Frustum.cpp %ENGINE_SOURCE%src\PulseEngine\core\PulseObject\PulseObject.cpp %ENGINE_SOURCE%src\PulseEngine\core\GUID\GuidGenerator.cpp"

rem === Compilation conditionnelle ===
set "OBJ_FILES="
for %%f in (%SRC_FILES%) do (
    set "SRC_FILE=%%f"
    set "OBJ_FILE=%OBJ_DIR%\%%~nf.o"
    set "OBJ_FILES=!OBJ_FILES! !OBJ_FILE!"

    if not exist "!OBJ_FILE!" (
        echo [NEW] Compilation: %%~nxf
        g++ -c -g -I%ENGINE_SOURCE%include -I%ENGINE_SOURCE%src "%%f" -o "!OBJ_FILE!" -DBUILDING_DLL
    ) else (
        for %%A in ("%%f") do set "SRC_TIME=%%~tA"
        for %%B in ("!OBJ_FILE!") do set "OBJ_TIME=%%~tB"
        if "!SRC_TIME!" GTR "!OBJ_TIME!" (
            echo [UPDATE] Recompilation: %%~nxf
            g++ -c -g -I%ENGINE_SOURCE%include "%%f" -o "!OBJ_FILE!"
        ) else (
            echo [SKIP] Up to date: %%~nxf
        )
    )
)

rem === Linking ===
echo.
echo === Linking: %DLL_TARGET%.dll ===
g++ -shared -Wl,--out-implib,%LIB_DIR%\lib%DLL_TARGET%.a ^
    -o %BUILD_DIR%\%DLL_TARGET%.dll %OBJ_FILES% ^
    -static-libgcc -static-libstdc++

if %errorlevel% neq 0 (
    echo Build failed!
    exit /b %errorlevel%
)

echo.
echo === Build terminé : %BUILD_DIR%\%DLL_TARGET%.dll ===
