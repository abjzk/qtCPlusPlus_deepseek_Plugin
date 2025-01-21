#include "AutoStartPlugin.h"
#include "AutoStartPluginWidget.h"
#include <QStandardPaths>

AutoStartPlugin::AutoStartPlugin(Logger * logger,TConfig *config,QObject *parent)
    : QAbstractPlugin(logger,config,parent)
{
}

AutoStartPlugin::~AutoStartPlugin()
{
}

/**
 * @brief 插件组名称
 * @return
 */
QString AutoStartPlugin::group()
{
    return "实用工具";
}

/**
 * @brief  插件名称
 * @return
 */
QString AutoStartPlugin::name()
{
    return "自启动管理器";
}

/**
 * @brief  插件版本
 * @return
 */
QString AutoStartPlugin::version()
{
    return "1.0.0";
}

/**
 * @brief  插件作者
 * @return
 */
QString AutoStartPlugin::author()
{
    return "ljzloser";
}

/**
 * @brief  插件描述
 * @return
 */
QString AutoStartPlugin::description()
{
    return "用于程序的自启动管理";
}

/**
 * @brief  插件图标
 * @return
 */
QIcon AutoStartPlugin::icon()
{
    return QIcon(":res/icon/AutoStartPlugin.png");
}

/**
 * @brief 启动插件
 * @return QWidget
 */
QWidget *AutoStartPlugin::start()
{
    QAbstractPlugin::start();
    return new AutoStartWidget(_logger,_config);
}


/**
 * @brief 停止插件
 */
void AutoStartPlugin::stop()
{
    QAbstractPlugin::stop();
}

/**
 * @brief 在写入配置文件前,在这里可以做一些校验操作,或者因为某些原因不允许修改
 * @param WriteConfigEvent
 * @details 
 */
void AutoStartPlugin::writeConfigBeforeEvent(WriteConfigEvent &event)
{
    QAbstractPlugin::writeConfigBeforeEvent(event);
}

/**
 * @brief  写入配置文件后，在这里可以做一些因为配置文件修改导致的操作
 * 如果event.isValid为false，那么不会触发该函数，界面也会回滚到原来的值
 * @param WriteConfigEvent
 */
void AutoStartPlugin::writeConfigAfterEvent(WriteConfigEvent &event)
{
    QAbstractPlugin::writeConfigAfterEvent(event);
}

/**
 * @brief 读取配置文件前,在这里可以做一些校验操作,比如你可以修改要读取的key
 * @param ReadConfigEvent
 * @details 
 */
void AutoStartPlugin::readConfigBeforeEvent(ReadConfigEvent &event)
{
    QAbstractPlugin::readConfigBeforeEvent(event);
}

/**
 * @brief 读取配置文件后，比如你可以修改要展示的值
 * @param ReadConfigEvent
 * @details 
 */
void AutoStartPlugin::readConfigAfterEvent(ReadConfigEvent &event)
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
void AutoStartPlugin::registerConfig()
{
    QAbstractPlugin::registerConfig();
    Directory_CONFIG_REGISTER("DirPath", "默认选择目录路径", QApplication::applicationDirPath(), true);    
}
