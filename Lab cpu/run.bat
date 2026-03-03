@echo off
chcp 65001 >nul
echo Compiling...
g++ -std=c++17 -o lab.exe main.cpp

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
lab.exe
