#include "ImageToIcoWidget.h"

ImageToIcoWidget::ImageToIcoWidget(TConfig *config,QWidget *parent)
    : QWidget(parent), ui(new Ui::ImageToIcoPlugin()), _config(config)
{
    ui->setupUi(this);
    this->initUi();
    this->initConnect();
}

ImageToIcoWidget::~ImageToIcoWidget()
{
    delete ui;
}

void ImageToIcoWidget::initUi()
{
    auto info = ui->inLineEdit->info();
    info.title = "选择输入图像文件夹";
    info.mode = QFileDialog::Directory;
    info.path = _config->read("currentInputPath").toString();
    ui->inLineEdit->setInfo(info);
    info = ui->outLineEdit->info();
    info.title = "选择输出图标文件夹";
    info.mode = QFileDialog::Directory;
    info.path = _config->read("currentOutputPath").toString();
    ui->outLineEdit->setInfo(info);
    ui->outLineEdit->setText(_config->read("currentOutputPath").toString());
}

void ImageToIcoWidget::initConnect()
{
    connect(ui->inLineEdit, &LFileLineEdit::fileSelected, this, [=]()
    {
        qDebug() << ui->inLineEdit->text();
    });
}
