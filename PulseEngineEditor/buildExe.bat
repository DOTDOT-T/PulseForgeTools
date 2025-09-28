@echo off
setlocal enabledelayedexpansion

echo Compilation de l'exécutable...

REM === CONFIGURATION ===
set COMPILER=g++
set STD=-std=c++17
set OUTPUT=game.exe
set MAIN=main.cpp

REM === Dossiers d'inclusion ===
set INCLUDE_DIRS=-Iinclude -Isrc -Icustom -Iimgui

REM === Librairies à lier ===
set LIB_PATHS=-L.
set LIBS=-lPulseEngine -lCustomScripts

REM === Flags de compilation ===
set FLAGS=-Wall -g

REM === Compilation ===
%COMPILER% %STD% -o %OUTPUT% %MAIN% %INCLUDE_DIRS% %LIB_PATHS% %LIBS% %FLAGS%

if %errorlevel% neq 0 (
    echo.
    echo ❌ Erreur de compilation.
) else (
    echo.
    echo ✅ Compilation réussie : %OUTPUT%
)

pause
