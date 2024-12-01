@echo off
echo Starting CMake process...
cmake --version
if %errorlevel% neq 0 (
    echo CMake is not installed or not in PATH.
    pause
    exit /b %errorlevel%
)
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE --no-warn-unused-cli -S E:/Project/C++/Tools -B E:/Project/C++/Tools/build_VS2022 -G "Visual Studio 17 2022" -T host=x64 -A x64
if %errorlevel% neq 0 (
    echo CMake command failed with error level %errorlevel%.
    pause
    exit /b %errorlevel%
)
echo CMake command finished successfully.
pause