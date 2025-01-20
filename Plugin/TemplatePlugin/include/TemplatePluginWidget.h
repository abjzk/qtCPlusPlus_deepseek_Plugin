#include "TemplatePlugin_global.h"
#include "ui_TemplatePluginWidget.h"
#include <config.h>
#include <QAbstractPlugin.h>
class TemplatePlugin_EXPORT TemplateWidget : public QWidget
{

    Q_OBJECT
public:
    TemplateWidget(Logger * logger,TConfig *config,QWidget *parent = nullptr);
    ~TemplateWidget();
private:
    void initUi();
    void initConnect();
    Ui::TemplatePluginWidget * ui;
    TConfig * _config;
    Logger * _logger;
};