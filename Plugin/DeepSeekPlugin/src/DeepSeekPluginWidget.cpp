#include "DeepSeekPluginWidget.h"
#include <QMessageBox>
#include <QImage>
#include <QIcon>
#include <qdesktopservices.h>

DeepSeekWidget::DeepSeekWidget(Logger *logger, TConfig *config, QWidget *parent)
    : QWidget(parent), ui(new Ui::DeepSeekPluginWidget()), _config(config), _logger(logger)
{
    ui->setupUi(this);
    this->initUi();
    this->initConnect();
}

DeepSeekWidget::~DeepSeekWidget()
{
    delete ui;
}
/**
 * @brief 初始化UI界面最好在这里完成
 */
void DeepSeekWidget::initUi()
{
#pragma region 初始化分割器的尺寸
    ui->hSplitter->setStretchFactor(0, 1);
    ui->hSplitter->setStretchFactor(1, 5);
    ui->vSplitter->setSizes(QList<int>() << 800 << 200);
    ui->vSplitter->setStretchFactor(0, 5);
    ui->vSplitter->setStretchFactor(1, 1);
#pragma endregion 初始化分割器的尺寸
}
/**
 * @brief 初始化各种信号和槽最好在这里完成
 */
void DeepSeekWidget::initConnect()
{
}
