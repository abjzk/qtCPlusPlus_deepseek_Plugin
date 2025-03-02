#include "ReplaceTemplatePlugin_global.h"
#include "ui_ReplaceTemplatePluginWidget.h"
#include <config.h>
#include "ReplaceTemplateThread.h"
#include "AbstractPlugin.h"

class ReplaceTemplatePlugin_EXPORT ReplaceTemplateWidget : public QWidget
{
    Q_OBJECT
public:
    ReplaceTemplateWidget(Logger * logger,TConfig *config,QWidget *parent = nullptr);
    ~ReplaceTemplateWidget();
private slots:
    void on_outputEdit_cursorPositionChanged(int arg1, int arg2);

private:
    void initUi();
    void initConnect();
    void addRow(const LoggerDetails& details);
    void setWidgetEnabled();
    void startThread();
    void loadFileTree(const QStringList &files, const QString &filter);
    Ui::ReplaceTemplatePluginWidget * ui;
    TConfig * _config;
    Logger * _logger;
    ReplaceTemplateThread * thread = nullptr;
};
