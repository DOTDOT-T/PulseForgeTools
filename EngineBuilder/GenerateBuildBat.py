import json, os

with open("BuildConfig.json") as f:
    config = json.load(f)

# Simple manual ordering (already sorted for dependencies)
modules = config["modules"]

for index, module in enumerate(modules, start=1):
    # zero-padded index for ordering
    idx_str = f"{index:03}"  # 001, 002, 003 ...

    name = module["name"]
    dll_target = module["dll_target"]
    src_path = module["source_path"]
    dependency_libs = ""
    if "dependency" in module:
        dependency_libs = " ".join([f'-l:{dep}' for dep in module["dependency"]])
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

set "SRC_FILES={sources}"

set "OBJ_FILES="
for %%f in (%SRC_FILES%) do (
    set "SRC_FILE=%%f"
    set "OBJ_FILE=%OBJ_DIR%\\%%~nf.o"
    set "OBJ_FILES=!OBJ_FILES! !OBJ_FILE!"

    if not exist "!OBJ_FILE!" (
        echo [NEW] Compilation: %%~nxf
        g++ -c -g {includes} "%%f" -o "!OBJ_FILE!" -DBUILDING_DLL -DPULSE_WINDOWS -DPULSE_GRAPHIC_OPENGL -DENGINE_EDITOR ^
        -Llib {dependency_libs} -LC:/msys64/mingw64/bin ^
        -lglfw3 -lopengl32 -lm -lassimp -lcomdlg32 -lws2_32 -lwinmm -lmswsock ^
        -static-libgcc -static-libstdc++
    ) else (
        for %%A in ("%%f") do set "SRC_TIME=%%~tA"
        for %%B in ("!OBJ_FILE!") do set "OBJ_TIME=%%~tB"
        if "!SRC_TIME!" GTR "!OBJ_TIME!" (
            echo [UPDATE] Recompilation: %%~nxf
            g++ -c -g {includes} "%%f" -o "!OBJ_FILE!" -DBUILDING_DLL -DPULSE_WINDOWS -DPULSE_GRAPHIC_OPENGL -DENGINE_EDITOR ^
            -Llib {dependency_libs} -LC:/msys64/mingw64/bin ^
            -lglfw3 -lopengl32 -lm -lassimp -lcomdlg32 -lws2_32 -lwinmm -lmswsock ^
            -static-libgcc -static-libstdc++
        ) else (
            echo [SKIP] Up to date: %%~nxf
        )
    )
)

echo.
echo === Linking: %DLL_TARGET%.dll ===
g++ -shared -Wl,--out-implib,%LIB_DIR%\\lib%DLL_TARGET%.a ^
    -o %BUILD_DIR%\\%DLL_TARGET%.dll %OBJ_FILES% -DBUILDING_DLL -DPULSE_WINDOWS -DPULSE_GRAPHIC_OPENGL -DENGINE_EDITOR ^
    -Llib {dependency_libs} -LC:/msys64/mingw64/bin ^
    -lglfw3 -lopengl32 -lm -lassimp -lcomdlg32 -lws2_32 -lwinmm -lmswsock ^
    -static-libgcc -static-libstdc++

if %errorlevel% neq 0 (
    echo Build failed!
    exit /b %errorlevel%
)

echo.
echo === Build terminé : %BUILD_DIR%\\%DLL_TARGET%.dll ===
"""

    # Add numeric prefix to enforce order
    out_file = f"BuildScript/{idx_str}_{name}.bat"
    with open(out_file, "w") as out:
        out.write(content)

print("All build scripts generated with numeric ordering.")
