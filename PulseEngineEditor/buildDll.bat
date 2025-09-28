@echo off
setlocal enabledelayedexpansion

REM === Configuration ===
set COMPILER=g++
set OUTPUT=CustomScripts.dll
set STD=-std=c++17
set DEFINES=-DBUILDING_DLL
set FLAGS=-shared -Wall -g -mconsole %DEFINES%
set INCLUDE_DIRS=-Icustom -Isrc\PulseEngine\CustomScripts -Iinclude -Isrc -Isrc/dllexport  -L.
set LIBS=-lPulseEngine

REM === Fichiers sources ===
set SOURCES=
for %%f in (custom\*.cpp) do (
    set SOURCES=!SOURCES! %%f
)

REM === Compilation en DLL ===
echo Compilation de la DLL personnalisée...
%COMPILER% %STD% -o %OUTPUT% %SOURCES% %INCLUDE_DIRS% %LIBS% %FLAGS%
if errorlevel 1 (
    echo Échec de la compilation.
    exit /b 1
)

echo DLL générée : %OUTPUT%
exit /b 0
