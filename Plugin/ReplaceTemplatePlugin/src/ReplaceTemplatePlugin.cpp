#include "ReplaceTemplatePlugin.h"
#include "ReplaceTemplatePluginWidget.h"
#include <QStandardPaths>

ReplaceTemplatePlugin::ReplaceTemplatePlugin(Logger * logger,TConfig *config,QObject *parent)
    : AbstractPlugin(logger,config,parent)
{
}

ReplaceTemplatePlugin::~ReplaceTemplatePlugin()
{
}

/**
 * @brief 插件组名称
 * @return
 */
QString ReplaceTemplatePlugin::group()
{
    return "文件处理";
}

/**
 * @brief  插件名称
 * @return
 */
QString ReplaceTemplatePlugin::name()
{
    return "替换模板文件夹";
}

/**
 * @brief  插件版本
 * @return
 */
QString ReplaceTemplatePlugin::version()
{
    return "0.0.0";
}

/**
 * @brief  插件作者
 * @return
 */
QString ReplaceTemplatePlugin::author()
{
    return "jzk";
}

/**
 * @brief  插件描述
 * @return
 */
QString ReplaceTemplatePlugin::description()
{
    return "根据模板生成文件";
}

/**
 * @brief  插件图标
 * @return
 */
QIcon ReplaceTemplatePlugin::icon()
{
    return QIcon(":res/icon/ReplaceTemplatePlugin.png");
}

/**
 * @brief 启动插件
 * @return QWidget
 */
QWidget *ReplaceTemplatePlugin::start()
{
    AbstractPlugin::start();
    return new ReplaceTemplateWidget(_logger,_config);
}


/**
 * @brief 停止插件
 */
void ReplaceTemplatePlugin::stop()
{
    AbstractPlugin::stop();
}

/**
 * @brief 在写入配置文件前,在这里可以做一些校验操作,或者因为某些原因不允许修改
 * @param WriteConfigEvent
 * @details 
 */
void ReplaceTemplatePlugin::writeConfigBeforeEvent(WriteConfigEvent &event)
{
    AbstractPlugin::writeConfigBeforeEvent(event);
}

/**
 * @brief  写入配置文件后，在这里可以做一些因为配置文件修改导致的操作
 * 如果event.isValid为false，那么不会触发该函数，界面也会回滚到原来的值
 * @param WriteConfigEvent
 */
void ReplaceTemplatePlugin::writeConfigAfterEvent(WriteConfigEvent &event)
{
    AbstractPlugin::writeConfigAfterEvent(event);
}

/**
 * @brief 读取配置文件前,在这里可以做一些校验操作,比如你可以修改要读取的key
 * @param ReadConfigEvent
 * @details 
 */
void ReplaceTemplatePlugin::readConfigBeforeEvent(ReadConfigEvent &event)
{
    AbstractPlugin::readConfigBeforeEvent(event);
}

/**
 * @brief 读取配置文件后，比如你可以修改要展示的值
 * @param ReadConfigEvent
 * @details 
 */
void ReplaceTemplatePlugin::readConfigAfterEvent(ReadConfigEvent &event)
{
    AbstractPlugin::readConfigAfterEvent(event);
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
void ReplaceTemplatePlugin::registerConfig()
{
    AbstractPlugin::registerConfig();
    // QStringList types = {"jpg", "jpeg", "png", "gif", "bmp", "ico", "exe", "dll", "mp3", "mp4", "avi", "zip", "rar", "tar", "tar.gz", "iso", "xlsx", "docx"};
    Bool_CONFIG_REGISTER("isRecursive", "是否递归替换文件", true, true);
    // String_CONFIG_REGISTER("BinaryFileTypes","二进制文件类型", types.join(";"), true);
    Bool_CONFIG_REGISTER("isGenterTopFloder","是否生成顶层文件夹", true, true);
    String_CONFIG_REGISTER("lastInputPath", "上次输入路径", "", false);
    String_CONFIG_REGISTER("lastOutputPath", "上次输出路径", "", false);
    String_CONFIG_REGISTER("lastTemplateName", "上次模板名称", "", false);
    String_CONFIG_REGISTER("lastOutName", "上次输出名称", "", false);
    Bool_CONFIG_REGISTER("isRe", "是否正则表达式", true, true);
}