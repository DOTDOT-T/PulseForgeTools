@echo off
setlocal enabledelayedexpansion
set "DLL_TARGET=PulseLib.InputSystem"
set "ENGINE_SOURCE=../../PulseEngineSource/"
set "OBJ_DIR=Object"
set "BUILD_DIR=Build"
set "LIB_DIR=Lib"

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
if not exist "%LIB_DIR%" mkdir "%LIB_DIR%"
if not exist "%OBJ_DIR%" mkdir "%OBJ_DIR%"

set "SRC_FILES=%ENGINE_SOURCE%src/PulseEngine/core/Input/InputSystem.cpp"

set "OBJ_FILES="
for %%f in (%SRC_FILES%) do (
    set "SRC_FILE=%%f"
    set "OBJ_FILE=%OBJ_DIR%\%%~nf.o"
    set "OBJ_FILES=!OBJ_FILES! !OBJ_FILE!"

    if not exist "!OBJ_FILE!" (
        echo [NEW] Compilation: %%~nxf
        g++ -c -g -I"%ENGINE_SOURCE%include" -I"%ENGINE_SOURCE%src" "%%f" -o "!OBJ_FILE!" -DBUILDING_DLL -DPULSE_WINDOWS -DPULSE_GRAPHIC_OPENGL -DENGINE_EDITOR ^
        -Llib -l:libPulseEngine.FileSystem.a -LC:/msys64/mingw64/bin ^
        -lglfw3 -lopengl32 -lm -lassimp -lcomdlg32 -lws2_32 -lwinmm -lmswsock ^
        -static-libgcc -static-libstdc++
    ) else (
        for %%A in ("%%f") do set "SRC_TIME=%%~tA"
        for %%B in ("!OBJ_FILE!") do set "OBJ_TIME=%%~tB"
        if "!SRC_TIME!" GTR "!OBJ_TIME!" (
            echo [UPDATE] Recompilation: %%~nxf
            g++ -c -g -I"%ENGINE_SOURCE%include" -I"%ENGINE_SOURCE%src" "%%f" -o "!OBJ_FILE!" -DBUILDING_DLL -DPULSE_WINDOWS -DPULSE_GRAPHIC_OPENGL -DENGINE_EDITOR ^
            -Llib -l:libPulseEngine.FileSystem.a -LC:/msys64/mingw64/bin ^
            -lglfw3 -lopengl32 -lm -lassimp -lcomdlg32 -lws2_32 -lwinmm -lmswsock ^
            -static-libgcc -static-libstdc++
        ) else (
            echo [SKIP] Up to date: %%~nxf
        )
    )
)

echo.
echo === Linking: %DLL_TARGET%.dll ===
g++ -shared -Wl,--out-implib,%LIB_DIR%\lib%DLL_TARGET%.a ^
    -o %BUILD_DIR%\%DLL_TARGET%.dll %OBJ_FILES% -DBUILDING_DLL -DPULSE_WINDOWS -DPULSE_GRAPHIC_OPENGL -DENGINE_EDITOR ^
    -Llib -l:libPulseEngine.FileSystem.a -LC:/msys64/mingw64/bin ^
    -lglfw3 -lopengl32 -lm -lassimp -lcomdlg32 -lws2_32 -lwinmm -lmswsock ^
    -static-libgcc -static-libstdc++

if %errorlevel% neq 0 (
    echo Build failed!
    exit /b %errorlevel%
)

echo.
echo === Build terminé : %BUILD_DIR%\%DLL_TARGET%.dll ===
