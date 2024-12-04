@echo off
setlocal enabledelayedexpansion

:: 检查是否传入了替换单词
if "%~1"=="" (
    echo Please provide a word to replace "Template".
    exit /b
)

:: 获取传入的替换单词
set "replacement=%~1"

:: 获取当前脚本所在目录
set "current_dir=%~dp0"

:: 设置模板文件夹路径
set "template_folder=%current_dir%Plugin\TemplatePlugin"

:: 设置生成文件夹的父目录（与模板文件夹同级）
set "output_folder=%current_dir%Plugin\%replacement%Plugin"

:: 创建目标文件夹（如果不存在）
if not exist "%output_folder%" mkdir "%output_folder%"

:: 递归处理文件夹和文件
call :process_dir "%template_folder%" "%output_folder%" "%replacement%"

:: 脚本结束
echo Done!
exit /b

:: 递归处理目录的函数
:process_dir
set "source_dir=%~1"
set "dest_dir=%~2"
set "replacement=%~3"

:: 打印当前处理的目录
echo Processing directory: %source_dir%

:: 遍历当前目录下的所有内容
for /f "delims=" %%i in ('dir "%source_dir%" /a /b') do (
    set "item=%%i"
    
    :: 判断是否是目录
    if exist "%source_dir%\%%i\" (
        :: 替换目录名称中的Template
        set "new_dir_name=!item:Template=%replacement%!"

        :: 打印原目录名和新目录名
        echo Renaming directory: "%source_dir%\%%i" to "%dest_dir%\!new_dir_name!"

        :: 创建新的目录
        if not exist "%dest_dir%\!new_dir_name!" (
            mkdir "%dest_dir%\!new_dir_name!"
        )

        :: 递归处理该子目录
        call :process_dir "%source_dir%\%%i" "%dest_dir%\!new_dir_name!" "%replacement%"
    ) else (
        :: 判断是否是文件
        if not "%%i"=="" (
            :: 替换文件名中的Template
            set "new_file_name=!item:Template=%replacement%!"

            :: 打印原文件名和新文件名
            echo Renaming file: "%source_dir%\%%i" to "%dest_dir%\!new_file_name!"

            :: 复制并重命名文件
            copy "%source_dir%\%%i" "%dest_dir%\!new_file_name!" >nul

            :: 替换文件内容中的Template
            call :replace_in_file "%dest_dir%\!new_file_name!" "%replacement%"
        )
    )
)

:: 结束递归
exit /b

:: 替换文件内容中的Template函数
:replace_in_file
set "file=%~1"
set "replacement=%~2"

:: 创建临时文件用于保存修改后的内容
set "temp_file=%file%.tmp"

:: 初始化临时文件
echo. > "%temp_file%"

:: 逐行读取并替换文件内容
for /f "delims=" %%a in (%file%) do (
    set "line=%%a"
    set "line=!line:Template=%replacement%!"
    echo !line! >> "%temp_file%"
)

:: 用临时文件替换原文件
move /y "%temp_file%" "%file%" >nul

:: 完成替换
exit /b
