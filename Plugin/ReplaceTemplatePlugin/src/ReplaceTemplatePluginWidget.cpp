#include "ReplaceTemplatePluginWidget.h"
#include <QMessageBox>
#include <QImage>
#include <QIcon>
#include <qdesktopservices.h>

ReplaceTemplateWidget::ReplaceTemplateWidget(Logger * logger,TConfig *config, QWidget *parent)
    : QWidget(parent), ui(new Ui::ReplaceTemplatePluginWidget()), _config(config),_logger(logger)
{
    ui->setupUi(this);
    thread = new ReplaceTemplateThread(this);
    this->initUi();
    this->initConnect();
}

ReplaceTemplateWidget::~ReplaceTemplateWidget()
{
    delete ui;
}
/**
 * @brief 初始化UI界面最好在这里完成
 */
void ReplaceTemplateWidget::initUi()
{
    ui->binTypeEdit->setText(_config->read("BinaryFileTypes").value.toString());
    auto info = ui->templateEdit->info();
    info.mode = QFileDialog::FileMode::Directory;
    ui->templateEdit->setInfo(info);
    ui->templateEdit->setText(this->_config->read("lastInputPath").value.toString());
    info = ui->outputEdit->info();
    info.mode = QFileDialog::FileMode::Directory;
    ui->outputEdit->setInfo(info);
    ui->outputEdit->setText(this->_config->read("lastOutputPath").value.toString());
    ui->topButton->setChecked(_config->read("isGenterTopFloder").value.toBool());
    ui->tempNameEdit->setText(this->_config->read("lastTemplateName").value.toString());
    ui->outputNameEdit->setText(this->_config->read("lastOutName").value.toString());
    ui->reButton->setChecked(_config->read("isRe").value.toBool());
}
/**
 * @brief 初始化各种信号和槽最好在这里完成
 */
void ReplaceTemplateWidget::initConnect()
{
    connect(ui->startButton, &QPushButton::clicked, this, &ReplaceTemplateWidget::startThread);
    connect(thread, &QThread::started, this, &ReplaceTemplateWidget::setWidgetEnabled);
    connect(thread, &QThread::finished, this, &ReplaceTemplateWidget::setWidgetEnabled);
    connect(ui->templateEdit, &LFileLineEdit::fileSelected, this, &ReplaceTemplateWidget::loadFileTree);
    connect(thread, &ReplaceTemplateThread::message, _logger,&Logger::write);
    connect(_logger, &Logger::sendLogger, this,&ReplaceTemplateWidget::addRow,Qt::QueuedConnection);
}

void ReplaceTemplateWidget::addRow(QMap<QString,QString> map)
{
    ui->textEdit->append(map["time"]+ "    " + map["level"] + "    " + map["message"]);
}

void ReplaceTemplateWidget::setWidgetEnabled()
{
    ui->binTypeEdit->setEnabled(thread->isFinished());
    ui->startButton->setEnabled(thread->isFinished());
    ui->outputEdit->setEnabled(thread->isFinished());
    ui->templateEdit->setEnabled(thread->isFinished());
    ui->tempNameEdit->setEnabled(thread->isFinished());
    ui->outputNameEdit->setEnabled(thread->isFinished());
}

void ReplaceTemplateWidget::startThread()
{
    QString templatePath = ui->templateEdit->text();
    QString outPath = ui->outputEdit->text();
    if(templatePath.isEmpty() || outPath.isEmpty())
    {
        _logger->warn("模板路径或输出路径为空");
        return;
    }
    QString templateName = ui->tempNameEdit->text();
    QString outName = ui->outputNameEdit->text();
    if(templateName.isEmpty() || outName.isEmpty())
    {
        _logger->warn("文本不能为空");
        return;
    }
    ui->textEdit->clear();
    QStringList binType = ui->binTypeEdit->text().split(";");
    bool isre = ui->reButton->isChecked();
    bool isGenerateTopFloder = ui->topButton->isChecked();
    thread->templatePath = templatePath;
    thread->outPath = outPath;
    thread->templateName = templateName;
    thread->outName = outName;
    thread->binType = binType;
    thread->isre = isre;
    thread->isRecursive = this->_config->read("isRecursive").value.toBool();
    thread->isGenerateTopFloder = isGenerateTopFloder;
    QVariantMap map{
        {"lastInputPath", templatePath},
        {"lastOutputPath", outPath},
        {"lastTemplateName", templateName},
        {"lastOutName", outName},
        {"isGenterTopFloder", isGenerateTopFloder},
        {"isRe", isre},
    };
    this->_config->write(map);
    thread->start();
}

void ReplaceTemplateWidget::loadFileTree(const QStringList &files, const QString &filter)
{
    if (files.isEmpty()) return;
}
