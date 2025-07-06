#include "ImageToIcoWidget.h"
#include <QMessageBox>
#include <QImage>
#include <QIcon>
#include <qdesktopservices.h>

ImageToIcoWidget::ImageToIcoWidget(TConfig *config, QWidget *parent)
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
    info.path = _config->read("currentInputPath").value.toString();
    ui->inLineEdit->setInfo(info);
    info = ui->outLineEdit->info();
    info.title = "选择输出图标文件夹";
    info.mode = QFileDialog::Directory;
    info.path = _config->read("currentOutputPath").value.toString();
    ui->outLineEdit->setInfo(info);
    ui->outLineEdit->setText(_config->read("currentOutputPath").value.toString());
    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(0);
}

void ImageToIcoWidget::initConnect()
{
    connect(ui->inLineEdit, &LFileLineEdit::fileSelected, this, &ImageToIcoWidget::loadTable);
    connect(ui->startButton, &QPushButton::clicked, this, &ImageToIcoWidget::startConvert);
    connect(ui->comboBox, &QComboBox::currentTextChanged, [=]()
            {
            this->loadTable(ui->inLineEdit->text().split(";"),
             ui->comboBox->currentText()); });
    connect(ui->openOutputButton, &QPushButton::clicked, [=]()
            {
            QString path = ui->outLineEdit->text();
            QDesktopServices::openUrl(QUrl::fromLocalFile(path)); });
}

void ImageToIcoWidget::loadTable(const QStringList &files, const QString filter)
{
    if (files.isEmpty())
        return;
    QDir dir(files.at(0));
    bool addSuffix = _config->read("addSuffix").value.toBool();
    // 后缀
    QString suffix = addSuffix ? "-" + ui->comboBox->currentText() : "";
    QStringList list = dir.entryList({"*.jpg", "*.jpeg", "*.png", "*.bmp"}, QDir::Files);
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setRowCount(list.size());
    for (int i = 0; i < list.size(); i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem(dir.path() + "/" + list.at(i));
        item->setIcon(QIcon(dir.path() + "/" + list.at(i)));
        ui->tableWidget->setItem(i, 0, item);
        QTableWidgetItem *item2 = new QTableWidgetItem(list.at(i).split(".")[0] + suffix + ".ico");
        ui->tableWidget->setItem(i, 1, item2);
        QTableWidgetItem *item3 = new QTableWidgetItem("未完成");
        ui->tableWidget->setItem(i, 2, item3);
    }
    ui->tableWidget->resizeColumnsToContents();
    ui->progressBar->setMaximum(list.size());
}

void ImageToIcoWidget::startConvert()
{
    int size = ui->comboBox->currentText().toInt();
    ui->startButton->setEnabled(false);
    ui->inLineEdit->setEnabled(false);
    ui->outLineEdit->setEnabled(false);
    QString dir = ui->inLineEdit->text();
    QString outdir = ui->outLineEdit->text();
    // 判断output文件夹是否存在
    if (!QDir(outdir).exists())
    {
        QDir().mkdir(outdir);
    }
    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        QString input = ui->tableWidget->item(i, 0)->text();
        QString output = outdir + "/" + ui->tableWidget->item(i, 1)->text();
        // 判断input文件是否存在
        if (!QFile(input).exists())
        {
            QMessageBox::warning(this, "警告", QString("%1不存在").arg(input));
            continue;
        }
        // 开始将图片转为ico
        QImage image(input);
        QIcon icon(QPixmap::fromImage(image));
        QPixmap pixmap = icon.pixmap(size, size);
        bool ret = pixmap.save(output, "ICO");
        if (!ret)
        {
            QMessageBox::warning(this, "警告", QString("%1保存失败").arg(output));
            continue;
        }
        ui->tableWidget->item(i, 2)->setText("已完成");
        // 滚动到当前行
        ui->tableWidget->scrollToItem(ui->tableWidget->item(i, 0));
        ui->progressBar->setValue(i + 1);
    }
    ui->inLineEdit->setEnabled(true);
    ui->outLineEdit->setEnabled(true);
    ui->startButton->setEnabled(true);
    if (_config->read("overOpenOutputPath").value.toBool())
        QDesktopServices::openUrl(QUrl::fromLocalFile(outdir));
}





