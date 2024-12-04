#include "TemplatePlugin.h"
#include "TemplatePluginWidget.h"
#include <QStandardPaths>

TemplatePlugin::TemplatePlugin(TConfig *config,QObject *parent)
    : QAbstractPlugin(config,parent)
{
}

TemplatePlugin::~TemplatePlugin()
{
}

/**
 * @brief 插件组名称
 * @return
 */
QString TemplatePlugin::group()
{
    return "group";
}

/**
 * @brief  插件名称
 * @return
 */
QString TemplatePlugin::name()
{
    return "name";
}

/**
 * @brief  插件版本
 * @return
 */
QString TemplatePlugin::version()
{
    return "version";
}

/**
 * @brief  插件作者
 * @return
 */
QString TemplatePlugin::author()
{
    return "author";
}

/**
 * @brief  插件描述
 * @return
 */
QString TemplatePlugin::description()
{
    return "description";
}

/**
 * @brief  插件图标
 * @return
 */
QIcon TemplatePlugin::icon()
{
    return QIcon(":res/icon/TemplatePlugin.png");
}

/**
 * @brief 启动插件
 * @return QWidget
 */
QWidget *TemplatePlugin::start()
{
    QAbstractPlugin::start();
    return new TemplateWidget(_config);
}


/**
 * @brief 停止插件
 */
void TemplatePlugin::stop()
{
    QAbstractPlugin::stop();
}

/**
 * @brief 在写入配置文件前,在这里可以做一些校验操作,或者因为某些原因不允许修改
 * @param WriteConfigEvent
 * @details 
 */
void TemplatePlugin::writeConfigBeforeEvent(WriteConfigEvent &event)
{
    QAbstractPlugin::writeConfigBeforeEvent(event);
}

/**
 * @brief  写入配置文件后，在这里可以做一些因为配置文件修改导致的操作
 * 如果event.isValid为false，那么不会触发该函数，界面也会回滚到原来的值
 * @param WriteConfigEvent
 */
void TemplatePlugin::writeConfigAfterEvent(WriteConfigEvent &event)
{
    QAbstractPlugin::writeConfigAfterEvent(event);
}

/**
 * @brief 读取配置文件前,在这里可以做一些校验操作,比如你可以修改要读取的key
 * @param ReadConfigEvent
 * @details 
 */
void TemplatePlugin::readConfigBeforeEvent(ReadConfigEvent &event)
{
    QAbstractPlugin::readConfigBeforeEvent(event);
}

/**
 * @brief 读取配置文件后，比如你可以修改要展示的值
 * @param ReadConfigEvent
 * @details 
 */
void TemplatePlugin::readConfigAfterEvent(ReadConfigEvent &event)
{
    QAbstractPlugin::readConfigAfterEvent(event);
}

/**
 * @brief 该函数用于注册插件的配置项
 * @details  在该函数中，需要使用CONFIG_REGISTER宏来注册插件的配置项
 *          CONFIG_REGISTER的第一个参数是配置项的Key，第二个参数是配置项的描述，
 *          第三个参数是配置项的类型，第四个参数是配置项的默认值，第五个参数
 *          是一个bool值，表示该配置项是否可以在界面上显示
 *          或使用指定类型的宏来注册 示例 
 *          Int_CONFIG_REGISTER("Int", "大小", 1, true);
 */
void TemplatePlugin::registerConfig()
{
    QAbstractPlugin::registerConfig();
}
