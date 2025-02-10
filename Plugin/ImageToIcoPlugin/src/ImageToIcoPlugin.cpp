#include "ImageToIcoPlugin.h"
#include "ImageToIcoWidget.h"
#include <QStandardPaths>

ImageToIcoPlugin::ImageToIcoPlugin(Logger* logger,TConfig *config,QObject *parent)
    : AbstractPlugin(logger,config,parent)
{
    this->registerConfig();
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

QWidget *ImageToIcoPlugin::start()
{
    AbstractPlugin::start();
    return new ImageToIcoWidget(_config);
}


void ImageToIcoPlugin::stop()
{
    AbstractPlugin::stop();
}

void ImageToIcoPlugin::writeConfigBeforeEvent(WriteConfigEvent &event)
{
    AbstractPlugin::writeConfigBeforeEvent(event);
    if(event.key == "currentOutputPath" || event.key == "currentInputPath")
    {
        QVariant path = event.newItem.value.toString();
        if (path.toString().isEmpty())
        {
            event.isValid = false;
            event.message = "路径不能为空";
        }
    }
}

void ImageToIcoPlugin::writeConfigAfterEvent(WriteConfigEvent &event)
{
    AbstractPlugin::writeConfigAfterEvent(event);
}

void ImageToIcoPlugin::readConfigBeforeEvent(ReadConfigEvent &event)
{
    AbstractPlugin::readConfigBeforeEvent(event);
}

void ImageToIcoPlugin::readConfigAfterEvent(ReadConfigEvent &event)
{
    AbstractPlugin::readConfigAfterEvent(event);
}

void ImageToIcoPlugin::registerConfig()
{
    AbstractPlugin::registerConfig();
    Directory_CONFIG_REGISTER("currentInputPath", "默认输入路径", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), true);
    Directory_CONFIG_REGISTER("currentOutputPath", "默认输出路径", QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)+"/ImageToIco" , true);
    Bool_CONFIG_REGISTER("overOpenOutputPath", "转换后打开文件夹", false, true);
    Bool_CONFIG_REGISTER("addSuffix", "文件名添加像素大小", false, true);
}
