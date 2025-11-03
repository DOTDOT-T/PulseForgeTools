@echo off
setlocal enabledelayedexpansion
set "DLL_TARGET=PulseEngine.CoreBackend"
set "ENGINE_SOURCE=../../PulseEngineSource/"
set "OBJ_DIR=Object"
set "BUILD_DIR=Build"
set "LIB_DIR=Lib"

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
if not exist "%LIB_DIR%" mkdir "%LIB_DIR%"
if not exist "%OBJ_DIR%" mkdir "%OBJ_DIR%"

set "SRC_FILES=%ENGINE_SOURCE%src/glad.c %ENGINE_SOURCE%src/PulseEngine/core/GUID/GuidGenerator.cpp %ENGINE_SOURCE%src/shader.cpp %ENGINE_SOURCE%src/camera.cpp %ENGINE_SOURCE%src/PulseEngine/core/PulseEngineBackend.cpp %ENGINE_SOURCE%src/PulseEngine/core/Inputs/Mouse.cpp %ENGINE_SOURCE%src/PulseEngine/core/Entity/Entity.cpp %ENGINE_SOURCE%src/PulseEngine/core/Meshes/mesh.cpp %ENGINE_SOURCE%src/PulseEngine/core/WindowContext/WindowContext.cpp %ENGINE_SOURCE%src/PulseEngine/core/Material/Material.cpp %ENGINE_SOURCE%src/PulseEngine/core/Material/MaterialManager.cpp %ENGINE_SOURCE%src/PulseEngine/core/Meshes/primitive/Primitive.cpp %ENGINE_SOURCE%src/PulseEngine/core/GUID/GuidReader.cpp %ENGINE_SOURCE%src/PulseEngine/core/SceneLoader/SceneLoader.cpp %ENGINE_SOURCE%src/PulseEngine/core/FileManager/FileManager.cpp %ENGINE_SOURCE%src/PulseEngine/core/Lights/DirectionalLight/DirectionalLight.cpp %ENGINE_SOURCE%src/PulseEngine/CustomScripts/ScriptsLoader.cpp %ENGINE_SOURCE%src/PulseEngine/core/Graphics/OpenGLAPI/OpenGLApi.cpp %ENGINE_SOURCE%src/PulseEngine/core/Physics/Collider/BoxCollider.cpp %ENGINE_SOURCE%src/PulseEngine/core/Meshes/SkeletalMesh.cpp %ENGINE_SOURCE%PulseEngineEditor/InterfaceEditor/InterfaceEditor.cpp %ENGINE_SOURCE%PulseEngineEditor/InterfaceEditor/TopBar.cpp %ENGINE_SOURCE%PulseEngineEditor/InterfaceEditor/InterfaceAPI/PulseInterfaceAPI.cpp %ENGINE_SOURCE%src/PulseEngine/core/Lights/PointLight/PointLight.cpp %ENGINE_SOURCE%src/PulseEngine/core/Material/Texture.cpp %ENGINE_SOURCE%src/PulseEngine/core/Lights/LightManager.cpp %ENGINE_SOURCE%src/PulseEngine/core/Physics/CollisionManager.cpp %ENGINE_SOURCE%src/PulseEngine/core/coroutine/CoroutineManager.cpp %ENGINE_SOURCE%PulseEngineEditor/InterfaceEditor/BuildGameCoroutine.cpp %ENGINE_SOURCE%src/PulseEngine/ModuleLoader/ModuleLoader.cpp %ENGINE_SOURCE%src/PulseEngine/API/EngineApi.cpp %ENGINE_SOURCE%src/PulseEngine/API/GameEntity.cpp %ENGINE_SOURCE%src/PulseEngine/core/Math/Transform/Transform.cpp %ENGINE_SOURCE%PulseEngineEditor/InterfaceEditor/InterfaceEditorFileExplorer.cpp %ENGINE_SOURCE%PulseEngineEditor/InterfaceEditor/Synapse/Synapse.cpp %ENGINE_SOURCE%PulseEngineEditor/InterfaceEditor/NewFileCreator/NewFileManager.cpp %ENGINE_SOURCE%src/PulseEngine/core/GUID/GuidCollection.cpp %ENGINE_SOURCE%src/PulseEngine/core/ExecutableManager/ExecutableLauncher.cpp %ENGINE_SOURCE%src/PulseEngine/core/ExecutableManager/PulseExecutable.cpp %ENGINE_SOURCE%src/PulseEngine/core/ExecutableManager/ExecutableCommunication.cpp %ENGINE_SOURCE%src/PulseEngine/API/EntityAPI/EntityApi.cpp %ENGINE_SOURCE%include/imgui/backends/imgui_impl_glfw.cpp %ENGINE_SOURCE%include/imgui/backends/imgui_impl_opengl3.cpp %ENGINE_SOURCE%include/imgui/ImGuizmo.cpp %ENGINE_SOURCE%include/imgui-node/imgui_node_editor.cpp %ENGINE_SOURCE%include/imgui-node/imgui_node_editor_api.cpp %ENGINE_SOURCE%include/imgui-node/imgui_canvas.cpp %ENGINE_SOURCE%include/imgui-node/crude_json.cpp %ENGINE_SOURCE%src/PulseEngine/core/Network/Request/Request.cpp %ENGINE_SOURCE%src/PulseEngine/core/FileManager/FileReader/FileReader.cpp %ENGINE_SOURCE%PulseEngineEditor/InterfaceEditor/Account/Account.cpp %ENGINE_SOURCE%src/PulseEngine/core/Input/InputSystem.cpp %ENGINE_SOURCE%src/PulseEngine/API/MaterialAPI/MaterialApi.cpp %ENGINE_SOURCE%src/PulseEngine/core/Meshes/RenderableMesh.cpp %ENGINE_SOURCE%src/PulseEngine/core/Meshes/StaticMesh.cpp %ENGINE_SOURCE%src/PulseEngine/core/Profiler/Profiler.cpp %ENGINE_SOURCE%src/PulseEngine/core/Profiler/ProfileTimer.cpp %ENGINE_SOURCE%src/PulseEngine/core/PulseObject/PulseObject.cpp %ENGINE_SOURCE%src/PulseEngine/core/Lights/Lights.cpp %ENGINE_SOURCE%src/PulseEngine/core/SceneManager/SceneManager.cpp %ENGINE_SOURCE%src/PulseEngine/core/SceneManager/SpatialPartition/SpatialPartition.cpp %ENGINE_SOURCE%src/PulseEngine/core/Math/Frustum/Frustum.cpp %ENGINE_SOURCE%src/PulseEngine/core/SceneManager/SpatialPartition/SimpleSpatial/SimpleSpatial.cpp %ENGINE_SOURCE%include/imgui/imgui_demo.cpp %ENGINE_SOURCE%include/imgui/imgui_draw.cpp %ENGINE_SOURCE%include/imgui/imgui_tables.cpp %ENGINE_SOURCE%include/imgui/imgui_widgets.cpp %ENGINE_SOURCE%include/imgui/imgui.cpp"

set "OBJ_FILES="
for %%f in (%SRC_FILES%) do (
    set "SRC_FILE=%%f"
    set "OBJ_FILE=%OBJ_DIR%\%%~nf.o"
    set "OBJ_FILES=!OBJ_FILES! !OBJ_FILE!"

    if not exist "!OBJ_FILE!" (
        echo [NEW] Compilation: %%~nxf
        g++ -c -g -I"%ENGINE_SOURCE%include" -I"%ENGINE_SOURCE%/" -I"%ENGINE_SOURCE%src" -I"%ENGINE_SOURCE%include/websocketpp" -I"%ENGINE_SOURCE%include/asio" -I"%ENGINE_SOURCE%include/imgui" -I"%ENGINE_SOURCE%include/imgui-node" -I"%ENGINE_SOURCE%external/assimp/include" "%%f" -o "!OBJ_FILE!" -DBUILDING_DLL -DPULSE_WINDOWS -DPULSE_GRAPHIC_OPENGL -DENGINE_EDITOR ^
        -Llib -l:libPulseLib.InputSystem.a -l:libPulseEngine.Registery.a -l:libPulseEngine.FileSystem.a -LC:/msys64/mingw64/bin ^
        -lglfw3 -lopengl32 -lm -lassimp -lcomdlg32 -lws2_32 -lwinmm -lmswsock ^
        -static-libgcc -static-libstdc++
    ) else (
        for %%A in ("%%f") do set "SRC_TIME=%%~tA"
        for %%B in ("!OBJ_FILE!") do set "OBJ_TIME=%%~tB"
        if "!SRC_TIME!" GTR "!OBJ_TIME!" (
            echo [UPDATE] Recompilation: %%~nxf
            g++ -c -g -I"%ENGINE_SOURCE%include" -I"%ENGINE_SOURCE%/" -I"%ENGINE_SOURCE%src" -I"%ENGINE_SOURCE%include/websocketpp" -I"%ENGINE_SOURCE%include/asio" -I"%ENGINE_SOURCE%include/imgui" -I"%ENGINE_SOURCE%include/imgui-node" -I"%ENGINE_SOURCE%external/assimp/include" "%%f" -o "!OBJ_FILE!" -DBUILDING_DLL -DPULSE_WINDOWS -DPULSE_GRAPHIC_OPENGL -DENGINE_EDITOR ^
            -Llib -l:libPulseLib.InputSystem.a -l:libPulseEngine.Registery.a -l:libPulseEngine.FileSystem.a -LC:/msys64/mingw64/bin ^
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
    -Llib -l:libPulseLib.InputSystem.a -l:libPulseEngine.Registery.a -l:libPulseEngine.FileSystem.a -LC:/msys64/mingw64/bin ^
    -lglfw3 -lopengl32 -lm -lassimp -lcomdlg32 -lws2_32 -lwinmm -lmswsock ^
    -static-libgcc -static-libstdc++

if %errorlevel% neq 0 (
    echo Build failed!
    exit /b %errorlevel%
)

echo.
echo === Build terminé : %BUILD_DIR%\%DLL_TARGET%.dll ===
