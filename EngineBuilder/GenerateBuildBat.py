import json
import os

with open("BuildConfig.json") as f:
    config = json.load(f)

modules = config["modules"]

for index, module in enumerate(modules, start=1):
    idx_str = f"{index:03}"  # 001, 002, etc.
    name = module["name"]
    dll_target = module["dll_target"]
    src_path = module["source_path"]

    dependency_libs = ""
    if "dependency" in module:
        dependency_libs = " ".join([f"-l:{dep}" for dep in module["dependency"]])

    includes = " ".join([f'-I"%ENGINE_SOURCE%{inc}"' for inc in module["include_dirs"]])
    sources = " ".join(["%ENGINE_SOURCE%" + s for s in module["sources"]])

    content = f"""@echo off
setlocal enabledelayedexpansion
set "DLL_TARGET={dll_target}"
set "ENGINE_SOURCE={src_path}"
set "OBJ_DIR=Object"
set "BUILD_DIR=Build"
set "LIB_DIR=Lib"

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
if not exist "%LIB_DIR%" mkdir "%LIB_DIR%"
if not exist "%OBJ_DIR%" mkdir "%OBJ_DIR%"

:: Compiler flags
set "CFLAGS=-c -g -DBUILDING_DLL -DPULSE_WINDOWS -DPULSE_GRAPHIC_OPENGL -DENGINE_EDITOR"
set "FLAGS=-g -DBUILDING_DLL -DPULSE_WINDOWS -DPULSE_GRAPHIC_OPENGL -DENGINE_EDITOR"
set "INCLUDES={includes}"
set "LIBS=-Llib {dependency_libs} -LC:/msys64/mingw64/bin -lglfw3 -lopengl32 -lm -lassimp -lcomdlg32 -lws2_32 -lwinmm -lmswsock"

set "SRC_FILES={sources}"
set "OBJ_FILES="

for %%f in (%SRC_FILES%) do (
    set "SRC_FILE=%%f"
    set "OBJ_FILE=%OBJ_DIR%\\%%~nf.o"
    set "OBJ_FILES=!OBJ_FILES! !OBJ_FILE!"

    if not exist "!OBJ_FILE!" (
        echo [NEW] Compilation: %%~nxf
        g++ %CFLAGS% %INCLUDES% "%%f" -o "!OBJ_FILE!" %LIBS%
    ) else (
        for %%A in ("%%f") do set "SRC_TIME_RAW=%%~tA"
        for %%B in ("!OBJ_FILE!") do set "OBJ_TIME_RAW=%%~tB"

        :: Convert timestamps to sortable format
        call :convert_time "!SRC_TIME_RAW!" SRC_TIME
        call :convert_time "!OBJ_TIME_RAW!" OBJ_TIME

        if !SRC_TIME! GTR !OBJ_TIME! (
            echo [UPDATE] Recompilation: %%~nxf
            g++ %CFLAGS% %INCLUDES% "%%f" -o "!OBJ_FILE!" %LIBS%
        ) else (
            echo [SKIP] Up to date: %%~nxf
        )
    )
)


echo.
echo === Linking: %DLL_TARGET%.dll ===
g++ -shared -Wl,--out-implib,%LIB_DIR%\\lib%DLL_TARGET%.a -o %BUILD_DIR%\\%DLL_TARGET%.dll %OBJ_FILES% %FLAGS% %LIBS%

if %errorlevel% neq 0 (
    echo Build failed!
    exit /b %errorlevel%
)

echo.
echo === Build terminé : %BUILD_DIR%\\%DLL_TARGET%.dll ===


goto :eof

:convert_time
:: %1 = input date (DD/MM/YYYY HH:MM)
:: %2 = output variable name
setlocal
set "ts=%~1"
for /f "tokens=1,2 delims= " %%i in ("%ts%") do (
    set "date=%%i"
    set "time=%%j"
)
for /f "tokens=1-3 delims=/" %%i in ("!date!") do (
    set "dd=%%i"
    set "mm=%%j"
    set "yyyy=%%k"
)
for /f "tokens=1-2 delims=:" %%i in ("!time!") do (
    set "hh=%%i"
    set "mi=%%j"
)
set "SORT_TIME=!yyyy!!mm!!dd!!hh!!mi!"
endlocal & set "%2=%SORT_TIME%"
goto :eof

"""

    out_file = f"BuildScript/{idx_str}_{name}.bat"
    os.makedirs(os.path.dirname(out_file), exist_ok=True)
    with open(out_file, "w", encoding="utf-8") as out:
        out.write(content)

print("✅ All build scripts generated successfully with fixed flag handling.")
