param (
    [string]$className  # 用户传入的类名
)
$className='AutoStartPlugin'
# 如果用户输入的类名首字母不是大写，自动将其转换为首字母大写
if ($className -and $className[0] -match '[a-z]') {
    $className = $className.Substring(0, 1).ToUpper() + $className.Substring(1)
}

# 校验类名是否合法（必须以大写字母开头，后续可以是字母、数字或下划线）
function IsValidClassName {
    param (
        [string]$name
    )
    
    # 类名必须以字母大写开头，后续可以是字母、数字或下划线
    $regex = '^[A-Z][A-Za-z0-9_]*$'
    
    if ($name -match $regex) {
        return $true
    } else {
        return $false
    }
}

# 检查类名是否合法
if (-not $className) {
    Write-Host "请输入一个类名。"
    exit
}

if (-not (IsValidClassName -name $className)) {
    Write-Host "提供的类名 '$className' 无效。有效的类名必须以大写字母开头，并且只能包含字母、数字和下划线。"
    exit
}

# 获取当前脚本所在的目录
$currentDir = Split-Path -Path $MyInvocation.MyCommand.Path -Parent

# 设置模板文件夹路径
$templateFolder = Join-Path $currentDir "Plugin\TemplatePlugin"

# 设置目标文件夹路径（与模板文件夹同级），并替换其中的 Template
$outputFolder = Join-Path $currentDir ("Plugin\" + "TemplatePlugin".Replace("Template", $className))

# 创建目标文件夹（如果不存在）
if (-not (Test-Path $outputFolder)) {
    Write-Host "创建目标文件夹：$outputFolder"
    New-Item -ItemType Directory -Path $outputFolder
}

# 定义二进制文件的扩展名（如图片、音频、视频等）
$binaryExtensions = @(".jpg", ".jpeg", ".png", ".gif", ".bmp", ".ico", ".exe", ".dll", ".mp3", ".mp4", ".avi", ".zip", ".rar", ".tar", ".tar.gz", ".iso", ".xlsx", ".docx")

# 递归处理目录
function Process-Directory {
    param (
        [string]$sourceDir,
        [string]$destDir,
        [string]$replacement
    )

    Write-Host "正在处理目录：$sourceDir"

    # 获取当前目录下的所有文件和文件夹
    Get-ChildItem -Path $sourceDir | ForEach-Object {
        $item = $_
        $newItemName = $item.Name -replace "Template", $replacement

        # 如果是目录，递归处理
        if ($item.PSIsContainer) {
            $newDestDir = Join-Path $destDir $newItemName
            Write-Host "重命名目录：$($item.FullName) -> $newDestDir"
            if (-not (Test-Path $newDestDir)) {
                New-Item -ItemType Directory -Path $newDestDir
            }
            Process-Directory -sourceDir $item.FullName -destDir $newDestDir -replacement $replacement
        }
        # 如果是文件，判断是否是二进制文件
        else {
            $fileExtension = $item.Extension.ToLower()

            # 如果是二进制文件，复制并只修改文件名
            if ($binaryExtensions -contains $fileExtension) {
                $newDestFile = Join-Path $destDir $newItemName
                Write-Host "重命名二进制文件：$($item.FullName) -> $newDestFile"
                Copy-Item -Path $item.FullName -Destination $newDestFile
            }
            # 如果是文本文件，进行内容替换
            else {
                $newDestFile = Join-Path $destDir $newItemName
                Write-Host "重命名并修改内容：$($item.FullName) -> $newDestFile"
                Copy-Item -Path $item.FullName -Destination $newDestFile

                # 替换文件内容中的Template
                Replace-ContentInFile -file $newDestFile -replacement $replacement
            }
        }
    }
}

# 替换文件内容中的 Template
function Replace-ContentInFile {
    param (
        [string]$file,
        [string]$replacement
    )

    # 读取文件内容
    $content = Get-Content -Path $file -Raw

    # 替换文件内容中的 Template
    $newContent = $content -replace "Template", $replacement

    # 将替换后的内容写回文件
    Set-Content -Path $file -Value $newContent -Force -Encoding UTF8
}

# 开始处理模板文件夹
Write-Host "开始处理模板文件夹：$templateFolder"
Process-Directory -sourceDir $templateFolder -destDir $outputFolder -replacement $className

Write-Host "处理完成！生成的文件位于：$outputFolder"
Pause