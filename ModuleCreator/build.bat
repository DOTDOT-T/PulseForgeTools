g++ -DBUILDING_DLL -DENGINE_EDITOR -DPULSE_WINDOWS -DPULSE_GRAPHIC_OPENGL -fvisibility=default ^
-shared -I./src -I. -I./PulseEngineEditor -I./include -Iinclude/asio -Iinclude/websocketpp -Iinclude -Iimgui ^
SourceCodeModule/EntityEditor/EntityEditor.cpp ^
-Lexternal/assimp/lib ^
-Lexternal/assimp/lib/x64 ^
lib/libPulseEngineEditor.a ^
-lassimp -lws2_32 -lwinmm -lmswsock ^
-o Output/EntityEditor.dll

@REM if %errorlevel% neq 0 (
@REM     echo Échec de la compilation de la DLL !
@REM     exit /b %errorlevel%
@REM ) else (
@REM     copy /Y "Output/EntityEditor.dll" "../engine/PulseEngineBuild/Modules/Interface/EntityEditor.dll"
@REM )
