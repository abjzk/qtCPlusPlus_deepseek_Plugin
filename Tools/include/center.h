#include "ui_center.h"
#include <QPoint>
#include <config.h>
#include <QAbstractPlugin.h>
#include "configDialog.h"
#include "logDialog.h"
class Center : public QWidget
{
    Q_OBJECT
public:
    explicit Center(TConfig *config,QWidget *parent = nullptr);
    ~Center();
    void initUi();
    void initConnect();

private:
    void loadPluginTree();
    QAbstractPlugin *currentPlugin = nullptr;
    QAbstractPlugin *findPlugin(QString name);
    QTreeWidgetItem *addPlugin(QString &filename);
    QMap<QString, QAbstractPlugin *> _plugins;
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
    TConfig * _config;
    void reSet();
    void pluginItemClicked(QTreeWidgetItem *item, int column);
};