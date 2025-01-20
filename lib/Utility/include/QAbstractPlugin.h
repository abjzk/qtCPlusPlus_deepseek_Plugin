#pragma once
#include "QObject"
#include <QKeySequence>
#include "utility.h"
#include "config.h"
#include "Utility_global.h"
#include <QWidget>
#include "Logger.h"
/**
 * @brief 注册配置
 * @param key 键
 * @param description 描述
 * @param type 类型
 * @param defaultValue 默认值
 * @param isShow 是否显示
 */
#define CONFIG_REGISTER(key, description, type, defaultValue, isShow) _config->registerConfig(key, description, TConfig::Type::type, defaultValue, isShow)
// 注册Int类型的配置 key, description, defaultValue, isShow
#define Int_CONFIG_REGISTER(key, description, defaultValue, isShow) CONFIG_REGISTER(key, description, Int, defaultValue, isShow)
// 注册String类型的配置, key, description, defaultValue, isShow
#define String_CONFIG_REGISTER(key, description, defaultValue, isShow) CONFIG_REGISTER(key, description, String, defaultValue, isShow)
// 注册Bool类型的配置, key, description, defaultValue, isShow
#define Bool_CONFIG_REGISTER(key, description, defaultValue, isShow) CONFIG_REGISTER(key, description, Bool, defaultValue, isShow)
// 注册Float类型的配置, key, description, defaultValue, isShow
#define FLOAT_CONFIG_REGISTER(key, description, defaultValue, isShow) CONFIG_REGISTER(key, description, Float, defaultValue, isShow)
// 注册Double类型的配置, key, description, defaultValue, isShow
#define Double_CONFIG_REGISTER(key, description, defaultValue, isShow) CONFIG_REGISTER(key, description, Double, defaultValue, isShow)
// 注册DateTime类型的配置, key, description, defaultValue, isShow
#define DateTime_CONFIG_REGISTER(key, description, defaultValue, isShow) CONFIG_REGISTER(key, description, DateTime, defaultValue, isShow)
// 注册Date类型的配置, key, description, defaultValue, isShow
#define Date_CONFIG_REGISTER(key, description, defaultValue, isShow) CONFIG_REGISTER(key, description, Date, defaultValue, isShow)
// 注册Time类型的配置, key, description, defaultValue, isShow
#define Time_CONFIG_REGISTER(key, description, defaultValue, isShow) CONFIG_REGISTER(key, description, Time, defaultValue, isShow)
// 注册File类型的配置, key, description, defaultValue, isShow
#define File_CONFIG_REGISTER(key, description, defaultValue, isShow) CONFIG_REGISTER(key, description, File, defaultValue, isShow)
// 注册Directory类型的配置, key, description, defaultValue, isShow
#define Directory_CONFIG_REGISTER(key, description, defaultValue, isShow) CONFIG_REGISTER(key, description, Directory, defaultValue, isShow)
// 注册Color类型的配置, key, description, defaultValue, isShow
#define Color_CONFIG_REGISTER(key, description, defaultValue, isShow) CONFIG_REGISTER(key, description, Color, defaultValue, isShow)
// 注册ComBox类型的配置, key, description, defaultValue, isShow
#define ComBox_CONFIG_REGISTER(key, description, defaultValue, isShow) CONFIG_REGISTER(key, description, Combox, defaultValue, isShow)


class UTILITY_EXPORT QAbstractPlugin : public QObject
{
    Q_OBJECT
public:
    explicit QAbstractPlugin(Logger* logger,TConfig *config,QObject *parent = nullptr);
    virtual ~QAbstractPlugin() = default;
    // 插件组名称
    virtual QString group() = 0;
    // 插件名称
    virtual QString name() = 0;
    // 插件版本
    virtual QString version() = 0;
    // 插件作者
    virtual QString author() = 0;
    // 插件描述
    virtual QString description() = 0;
    // 插件图标
    virtual QIcon icon() = 0;
    /**
     * @brief 启动插件
     * @param config 配置
     * @return QWidget
     */
    virtual QWidget * start();
    /**
     * @brief 停止插件
     */
    virtual void stop();
    /**
     * @brief 获取配置
     * @return TConfig *
     */
    TConfig *config() { return _config; }
    Logger *logger() { return _logger; }
    /**
     * @brief 注册配置
     */
    virtual void registerConfig();
    /**
     * @brief 写入配置文件前,在这里可以做一些校验操作,或者因为某些原因不允许修改。
     * @param WriteEvent
     */
    virtual void writeConfigBeforeEvent(WriteConfigEvent &event);
    /**
     * @brief 写入配置文件后，在这里可以做一些因为配置文件修改导致的操作
     * 如果event.isValid为false，那么不会触发该函数，界面也会回滚到原来的值
     * @param WriteEvent
     */
    virtual void writeConfigAfterEvent(WriteConfigEvent &event);
    /**
     * @brief 读取配置文件前,比如说你可以修改要读取的key
     * @param ReadEvent
     */
    virtual void readConfigBeforeEvent(ReadConfigEvent &event);
    /**
     * @brief 读取配置文件后，比如说你可以修改要展示的值
     * @param ReadEvent
     */
    virtual void readConfigAfterEvent(ReadConfigEvent &event);
protected:
    TConfig *_config;
    Logger* _logger;
};

// 插件工厂
class UTILITY_EXPORT PluginFactory
{
public:
    /**
     * @brief 创建插件
     * @return
     */
    virtual QAbstractPlugin *create(Logger *logger,TConfig *config) = 0;
    virtual ~PluginFactory() = default;
};

#define QAbstractPlugin_IID "org.Tools.QAbstractPlugin"

Q_DECLARE_INTERFACE(PluginFactory, QAbstractPlugin_IID)