#include "ui_center.h"
#include <QPoint>
#include <config.h>
#include <QAbstractPlugin.h>
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
    QAbstractPlugin *findPlugin(QString name);
    void addPlugin(QString &filename, int &retFlag);
    void showPluginTreeMenu(QPoint pos);
    TConfig * _config = new TConfig("SYSTEM", this);
    QMap<QString, QAbstractPlugin *> _plugins;
    
    Ui::Center *ui;
};