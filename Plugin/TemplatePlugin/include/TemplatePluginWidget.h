#include "TemplatePlugin_global.h"
#include "ui_TemplatePluginWidget.h"
#include <config.h>
class TemplatePlugin_EXPORT TemplateWidget : public QWidget
{
    Q_OBJECT
public:
    TemplateWidget(TConfig *config,QWidget *parent = nullptr);
    ~TemplateWidget();
private:
    void initUi();
    void initConnect();
    Ui::TemplatePluginWidget * ui;
    TConfig * _config;
};