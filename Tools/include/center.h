#include "ui_center.h"
#include <QPoint>
#include <config.h>
#include <AbstractPlugin.h>
#include "configDialog.h"
#include "logDialog.h"
class Center : public QWidget
{
    Q_OBJECT
public:
    explicit Center(TConfig *config, QWidget *parent = nullptr);
    ~Center();
    void initUi();
    void initConnect();

private:
    void loadPluginTree();
    AbstractPlugin *currentPlugin = nullptr;
    AbstractPlugin *findPlugin(QString name);
    QTreeWidgetItem *addPlugin(QString &filename);
    QMap<QString, AbstractPlugin *> _plugins;
    ConfigDialog *dialog = nullptr;
    LogDialog *logDialog = nullptr;
    Ui::Center *ui;
    void showPluginTreeMenu(QPoint pos);
    void writeConfigAfterEvent(WriteConfigEvent &event);
    void writeConfigBeforeEvent(WriteConfigEvent &event);
    void readConfigBeforeEvent(ReadConfigEvent &event);
    void readConfigAfterEvent(ReadConfigEvent &event);
    void showConfigDialog();
    void showLogDialog();
    TConfig *_config;
    void reSet();
    void pluginItemClicked(QTreeWidgetItem *item, int column);
};