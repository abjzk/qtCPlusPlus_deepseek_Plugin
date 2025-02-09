#include "DeepSeekPlugin_global.h"
#include "ui_DeepSeekPluginWidget.h"
#include <config.h>
#include <AbstractPlugin.h>
#include <DeepSeek.h>
class DeepSeekPlugin_EXPORT DeepSeekWidget : public QWidget
{

    Q_OBJECT
public:
    DeepSeekWidget(Logger *logger, TConfig *config, QWidget *parent = nullptr);
    ~DeepSeekWidget();

private:
    void initUi();
    void initConnect();
    Ui::DeepSeekPluginWidget *ui;
    TConfig *_config;
    Logger *_logger;
};