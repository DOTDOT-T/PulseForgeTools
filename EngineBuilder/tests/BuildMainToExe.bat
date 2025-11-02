@echo off
setlocal enabledelayedexpansion

if not exist tests mkdir tests

rem === Compilation du test ===
g++ ^
    tests/main.cpp ^
    -LLib -lengine -I../PulseForge/PulseEngine/src ^
    -o tests/Game.exe

if %errorlevel% neq 0 (
    echo Build failed!
    exit /b %errorlevel%
) else (
    echo Build succeeded!
)
