@echo off
:: 获取传入的第一个参数，如果没有就定义变量为Debug,0为Release,1为Debug
if "%1"=="" (
  set BUILD_TYPE=Release
) else (
  if "%1"=="0" (
    set BUILD_TYPE=Release
  ) else if "%1"=="1" (
    set BUILD_TYPE=Debug
  ) else (
    echo Invalid build type. Defaulting to Debug.
    set BUILD_TYPE=Release
  )
)

:: 如果不存output文件夹就创建
echo Begin packing %BUILD_TYPE%
set output_dir=output
if not exist "%output_dir%" mkdir "%BUILD_TYPE%"

set pack_dir="%output_dir%\%BUILD_TYPE%"

:: 如果存在output/pack_type文件夹就删除
echo Delete old "%pack_dir%" directory
if exist "%pack_dir%" rd /s /q "%pack_dir%"

:: 创建output/pack_type文件夹
echo Create new "%pack_dir%" directory
mkdir "%pack_dir%"

:: 将bin\pack_type目录下的*.dll 和 Tools.exe复制到pack_type文件夹
echo Begin copying dynamic libraries and executable files from "bin\%BUILD_TYPE%" directory to "%pack_dir%"
copy /y bin\%BUILD_TYPE%\*.dll "%pack_dir%"
copy /y bin\%BUILD_TYPE%\Tools.exe "%pack_dir%"

:: 查找Tools.exe的依赖使用qt的windeployqt工具
echo Using windeployqt tool to find Qt's dependencies
cd "%pack_dir%"
windeployqt Tools.exe LCore.dll LWidget.dll Utility.dll


:: 返回到脚本目录
cd ..
cd ..

:: 使用InnoSetup编译安装包
echo Using InnoSetup to create installer
::iscc -c Tools.iss


echo Done packing %BUILD_TYPE% to "%pack_dir%"
