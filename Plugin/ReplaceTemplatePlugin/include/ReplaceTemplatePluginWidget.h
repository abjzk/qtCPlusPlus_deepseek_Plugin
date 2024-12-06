#include "ReplaceTemplatePlugin_global.h"
#include "ui_ReplaceTemplatePluginWidget.h"
#include <config.h>
#include "ReplaceTemplateThread.h"
#include "QAbstractPlugin.h"

class ReplaceTemplatePlugin_EXPORT ReplaceTemplateWidget : public QWidget
{
    Q_OBJECT
public:
    ReplaceTemplateWidget(Logger * logger,TConfig *config,QWidget *parent = nullptr);
    ~ReplaceTemplateWidget();
private:
    void initUi();
    void initConnect();
    void addRow(QMap<QString,QString> map);
    void setWidgetEnabled();
    void startThread();
    void loadFileTree(const QStringList &files, const QString &filter);
    Ui::ReplaceTemplatePluginWidget * ui;
    TConfig * _config;
    Logger * _logger;
    ReplaceTemplateThread * thread = nullptr;
};
