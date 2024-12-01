#include "QWidget"
#include <QKeySequence>
#include "utility.h"
#include "config.h"
class QAbstractPlugin : public QWidget
{
    Q_OBJECT
public:
    explicit QAbstractPlugin(TConfig *config,QWidget *parent = nullptr);
    virtual ~QAbstractPlugin() = default;
    virtual QString name() { return ""; };
    virtual QString version() = 0;
    virtual QString author() = 0;
    virtual QString description() = 0;
    virtual QIcon icon() = 0;
    bool saveLog(const QString &log, LogLevel level);
    bool registerShortcut(const QString &name,QKeySequence keySequence);

signals:
    void registerShortcutSignal(QKeySequence keySequence);
    void logSignal(const QString &log, LogLevel level);
public slots:
    virtual void registerShortcutSlot(bool isRegister) = 0;
protected:
    TConfig *_config;
};



inline bool QAbstractPlugin::registerShortcut(const QString &name,QKeySequence keySequence)
{
    emit registerShortcutSignal(keySequence);
}
// 插件工厂
class PluginFactory
{
public:
    /**
     * @brief 创建插件
     * @return
     */
    virtual QAbstractPlugin *create() = 0;
    virtual ~PluginFactory() = default;
};

#define BasePlugin_IID "org.Tools.QAbstractPlugin"

Q_DECLARE_INTERFACE(PluginFactory, BasePlugin_IID)