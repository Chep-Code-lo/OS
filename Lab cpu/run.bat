@echo off
chcp 65001 >nul
echo.
echo ========================================
echo   CPU SCHEDULING SIMULATOR (C++)
echo ========================================
echo.
echo Compiling...
g++ -std=c++17 -o OperatingSystem.exe main.cpp

if %errorlevel% neq 0 (
    echo.
    echo [ERROR] Compilation failed!
    pause
    exit /b 1
)

echo.
echo [SUCCESS] Compilation successful!
echo.
echo Starting program...
echo.
OperatingSystem.exe
