g++ -DBUILDING_DLL -DPULSE_WINDOWS -DPULSE_GRAPHIC_OPENGL -fvisibility=default ^
-shared -I./src -I. -I./PulseEngineEditor -I./include -Iinclude/asio -Iinclude/websocketpp -Iinclude -Iimgui ^
SourceCodeModule/InputBinding/InputBinding.cpp ^
-Lexternal/assimp/lib ^
-Lexternal/assimp/lib/x64 ^
lib/libPulseEngineEditor.a ^
-lassimp -lws2_32 -lwinmm -lmswsock ^
-o Output/InputBinding.dll

@REM if %errorlevel% neq 0 (
@REM     echo Échec de la compilation de la DLL !
@REM     exit /b %errorlevel%
@REM ) else (
@REM     copy /Y "Output/InputBinding.dll" "../engine/PulseEngineBuild/Modules/Interface/InputBinding.dll"
@REM )
