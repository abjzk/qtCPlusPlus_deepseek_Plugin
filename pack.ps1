param (
    [string]$BuildType  # 用户传入的构建类型（0为Release，1为Debug）
)




# 判断传入参数，如果没有传入则默认为 Release；如果传入的是 0 或 1，则分别设置为 Release 或 Debug
if (-not $BuildType) {
    $BuildType = "Release"
}
elseif ($BuildType -eq "0") {
    $BuildType = "Release"
}
elseif ($BuildType -eq "1") {
    $BuildType = "Debug"
}
else {
    Write-Host "无效的构建类型，默认为 Release."
    $BuildType = "Release"
}
# 输出当前构建类型
Write-Host "开始打包 $BuildType"

# 设置输出目录
$outputDir = "output"
$packDir = Join-Path $outputDir $BuildType

# 如果 output 目录不存在，则创建它
if (-not (Test-Path $outputDir)) {
    New-Item -ItemType Directory -Path $outputDir
}

# 如果存在之前的构建目录，则删除
if (Test-Path $packDir) {
    Write-Host "删除旧的目录 $packDir"
    Remove-Item -Recurse -Force $packDir
}

# 创建新的构建目录
Write-Host "创建新的目录 $packDir"
New-Item -ItemType Directory -Path $packDir

# 将 bin\%BUILD_TYPE% 目录下的 *.dll 和 Tools.exe 复制到输出目录
Write-Host "复制动态链接库和可执行文件到 $packDir"
Copy-Item -Path "bin\$BuildType\*.dll" -Destination $packDir -Force
Copy-Item -Path "bin\$BuildType\Tools.exe" -Destination $packDir -Force
Copy-Item -Path "Tools\res\Tools.ico" -Destination $packDir -Force

# 删除 TemplatePlugin.dll 文件（如果存在）
$templatePluginDllPath = Join-Path $packDir "TemplatePlugin.dll"
if (Test-Path $templatePluginDllPath) {
    Write-Host "删除文件 TemplatePlugin.dll"
    Remove-Item -Path $templatePluginDllPath -Force
}

# 使用 windeployqt 查找 Qt 依赖
Write-Host "使用 windeployqt 工具查找 Qt 依赖"
Set-Location -Path $packDir
& "windeployqt.exe" "Tools.exe" "LCore.dll" "LWidget.dll" "Utility.dll"

# 返回脚本原目录
Set-Location -Path $PSScriptRoot

# 使用 InnoSetup 编译安装包（需要手动配置 InnoSetup）
Write-Host "使用 InnoSetup 创建安装包"
# 取消注释并根据需要配置 InnoSetup
# & "C:\Path\To\InnoSetup\iscc.exe" "Tools.iss"

Write-Host "打包完成，$BuildType 文件夹已生成：$packDir"
Pause