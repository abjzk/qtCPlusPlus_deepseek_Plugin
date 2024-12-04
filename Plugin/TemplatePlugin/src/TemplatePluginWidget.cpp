#include "TemplatePluginWidget.h"
#include <QMessageBox>
#include <QImage>
#include <QIcon>
#include <qdesktopservices.h>

TemplateWidget::TemplateWidget(TConfig *config, QWidget *parent)
    : QWidget(parent), ui(new Ui::TemplatePluginWidget()), _config(config)
{
    ui->setupUi(this);
    this->initUi();
    this->initConnect();
}

TemplateWidget::~TemplateWidget()
{
    delete ui;
}
/**
 * @brief 初始化UI界面最好在这里完成
 */
void TemplateWidget::initUi()
{
}
/**
 * @brief 初始化各种信号和槽最好在这里完成
 */
void TemplateWidget::initConnect()
{
}
