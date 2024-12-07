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
    explicit Center(QWidget *parent = nullptr);
    ~Center();
    void initUi();
    void initConnect();

private:
    void loadPluginTree();
    QAbstractPlugin *currentPlugin = nullptr;
    QAbstractPlugin *findPlugin(QString name);
    QTreeWidgetItem *addPlugin(QString &filename);
    TConfig *_config = new TConfig("SYSTEM", this);
    QMap<QString, QAbstractPlugin *> _plugins;
    ConfigDialog *dialog = nullptr;
    LogDialog *logDialog = nullptr;
    Ui::Center *ui;
    void showPluginTreeMenu(QPoint pos);

    void showConfigDialog();
    void showLogDialog();

    void reSet();
    void pluginItemClicked(QTreeWidgetItem *item, int column);
};