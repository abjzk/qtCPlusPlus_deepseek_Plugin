@echo off
echo Starting CMake process...

:: 检查 CMake 是否安装并在 PATH 中
cmake --version
if %errorlevel% neq 0 (
    echo CMake is not installed or not in PATH.
    pause
    exit /b %errorlevel%
)

:: 获取当前脚本的路径
set script_dir=%cd%

:: 设置项目路径（假设项目根目录和脚本在同一目录层级）
set project_path=%script_dir%

:: 检查项目路径是否存在
if not exist "%project_path%\Tools" (
    echo The directory "%project_path%\Tools" does not exist.
    pause
    exit /b 1
)

:: 执行 CMake 命令，使用当前工作目录作为项目根目录
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE --no-warn-unused-cli -S "%project_path%" -B "%project_path%\build_VS2022" -G "Visual Studio 17 2022" -T host=x64 -A x64

if %errorlevel% neq 0 (
    echo CMake command failed with error level %errorlevel%.
    pause
    exit /b %errorlevel%
)

echo CMake command finished successfully.
pause
