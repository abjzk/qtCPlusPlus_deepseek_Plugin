#include "ImageToIcoPlugin.h"
#include "ImageToIcoWidget.h"
#include <QStandardPaths>

ImageToIcoPlugin::ImageToIcoPlugin(QObject *parent)
    : QAbstractPlugin(parent)
{
}

ImageToIcoPlugin::~ImageToIcoPlugin()
{
}

QString ImageToIcoPlugin::group()
{
    return "图像处理";
}

QString ImageToIcoPlugin::name()
{
    return "图片转ico";
}

QString ImageToIcoPlugin::version()
{
    return "1.0.0";
}

QString ImageToIcoPlugin::author()
{
    return "ljzloser";
}

QString ImageToIcoPlugin::description()
{
    return "这是一个图像转ico的插件";
}

QIcon ImageToIcoPlugin::icon()
{
    return QIcon(":res/icon/ImageToIco.png");
}

QWidget *ImageToIcoPlugin::start(TConfig *config)
{
    QAbstractPlugin::start(config);
    config->registerConfig("currentInputPath", "默认输入路径", TConfig::Type::Directory, QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), true);
    config->registerConfig("currentOutputPath", "默认输出路径", TConfig::Type::Directory,QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)+"/ImageToIco" , true);
    config->registerConfig("overOpenOutputPath", "转换后打开文件夹", TConfig::Type::Bool, false, true);
    config->registerConfig("addSuffix", "文件名添加像素大小", TConfig::Type::Bool, false, true);
    return new ImageToIcoWidget(config);
}

void ImageToIcoPlugin::stop()
{
    QAbstractPlugin::stop();
}
