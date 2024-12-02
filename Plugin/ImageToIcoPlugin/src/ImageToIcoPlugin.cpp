#include "ImageToIcoPlugin.h"

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
    return QIcon();
}
