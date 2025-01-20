#include "AutoStartPlugin_global.h"
#include "ui_AutoStartPluginWidget.h"
#include <config.h>
#include <QLayout>
#include <QTabWidget>
#include <QTableWidgetItem>
#include <QAbstractPlugin.h>

struct Program
{
    QString icon = "";
    QString name = "";
    QString path = "";
    QString arguments = "";
};


class AutoStartPlugin_EXPORT AutoStartWidget : public QWidget
{
    Q_OBJECT
public:
    AutoStartWidget(Logger * logger,TConfig *config,QWidget *parent = nullptr);
    ~AutoStartWidget();
private:
    void initUi();
    void initConnect();
    void loadTable();
    QList<Program> getPrograms();
    Ui::AutoStartPluginWidget * ui;
    TConfig * _config;
    Logger * _logger;
    QVBoxLayout * _mainLayout = new QVBoxLayout();
    QTableWidget * _tableWidget = new QTableWidget();
    void showTableMenu(const QPoint &pos);
};