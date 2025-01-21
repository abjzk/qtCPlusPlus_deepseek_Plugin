#include "AutoStartPluginWidget.h"
#include <QMessageBox>
#include <QImage>
#include <QIcon>
#include <qdesktopservices.h>
#include <QSettings>
#include <Windows.h>
#include <QPixmap>
#include <QHeaderView>
#include <QMenu>
#include <QUrl>
#include <QFileInfo>
#include <QProcess>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <LCore>
AutoStartWidget::AutoStartWidget(Logger * logger,TConfig *config, QWidget *parent)
    : QWidget(parent), ui(new Ui::AutoStartPluginWidget()), _config(config),_logger(logger)
{
    ui->setupUi(this);
    this->initUi();
    this->initConnect();
    this->getPrograms();
}

AutoStartWidget::~AutoStartWidget()
{
    delete ui;
}
/**
 * @brief 初始化UI界面最好在这里完成
 */
void AutoStartWidget::initUi()
{
    _mainLayout->addWidget(_tableWidget);
    this->setLayout(_mainLayout);
    QStringList headerTexts = {"名称","路径","启动参数"};
    // 设置表头
    _tableWidget->setColumnCount(headerTexts.size());
    _tableWidget->setRowCount(0);
    _tableWidget->setHorizontalHeaderLabels(headerTexts);
    // 设置表格不可编辑
    _tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // 设置最后一列挤满
    _tableWidget->horizontalHeader()->setStretchLastSection(true);
    // 右键菜单
    _tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    this->loadTable();
}
/**
 * @brief 初始化各种信号和槽最好在这里完成
 */
void AutoStartWidget::initConnect()
{
    connect(_tableWidget, &QTableWidget::customContextMenuRequested, this, &AutoStartWidget::showTableMenu);
}

void AutoStartWidget::loadTable()
{
    QList<Program> programs = this->getPrograms();
    _tableWidget->setRowCount(programs.size());
    for (int i = 0; i < programs.size(); i++)
    {
        _tableWidget->setItem(i, 0, new QTableWidgetItem(programs.at(i).name));
        _tableWidget->setItem(i, 1, new QTableWidgetItem(programs.at(i).path));
        _tableWidget->setItem(i, 2, new QTableWidgetItem(programs.at(i).arguments));
    }
    _tableWidget->resizeColumnsToContents();
    // 设置首列宽度为100
    _tableWidget->setColumnWidth(0, 100);
}

QList<Program> AutoStartWidget::getPrograms()
{
    // 读取注册表中开机自启动的程序
    QList<Program> programs;

    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    for (QString key : settings.allKeys()) 
    {
        Program program;
        program.name = key;
        // program.icon = QDesktopServices::storageLocation(QDesktopServices::DesktopLocation) + "/windows.ico";
        QString value = settings.value(key).toString();
        int pathStart = value.indexOf("\"") + 1;
        int pathEnd = value.lastIndexOf("\"");
        if (pathStart != -1 && pathEnd != -1)
        {
            program.path = value.mid(pathStart, pathEnd - pathStart);
            program.arguments = value.mid(pathEnd + 1);
        }
        else
        {
            QStringList values = value.split("exe");
            program.path = values.at(0) + "exe";
            program.arguments = values.at(1).trimmed();
        }
        _logger->info(QString("\{\"name\":\"%1\",\"path\":\"%2\",\"arguments\":\"%3\"\}").arg(program.name).arg(program.path).arg(program.arguments));
        programs.append(program);
    }
    _logger->info("读取注册表中开机自启动的程序完成 size:" + QString::number(programs.size()));
    return programs;
}

void AutoStartWidget::showTableMenu(const QPoint &pos)
{
    QPoint globalPos = QCursor::pos();
    QMenu menu(this);
    QAction * addAction = menu.addAction("添加");
    QAction * removeAction = menu.addAction("删除");
    QAction * editAction = menu.addAction("编辑");
    QAction * refreshAction = menu.addAction("刷新");
    QAction * backAction = menu.addAction("备份");
    QAction * openInDirAction = menu.addAction("打开文件所在路径");
    QAction * openAction = menu.addAction("打开应用程序");

    connect(addAction, &QAction::triggered, this, &AutoStartWidget::addProgram);
    connect(removeAction, &QAction::triggered, this, &AutoStartWidget::removeProgram);
    connect(editAction, &QAction::triggered, this, &AutoStartWidget::editProgram);
    connect(backAction, &QAction::triggered, this, &AutoStartWidget::backUp);
    connect(refreshAction, &QAction::triggered, this, &AutoStartWidget::loadTable);
    connect(openInDirAction, &QAction::triggered, this, &AutoStartWidget::openProgramDir);
    connect(openAction, &QAction::triggered, this,&AutoStartWidget::runProgram);
    menu.exec(globalPos);
}
void AutoStartWidget::openProgramDir()
{
    QString path = _tableWidget->item(_tableWidget->currentRow(), 1)->text();
    QString command = QString("explorer /select,\"%1\"").arg(path);
    _logger->info("打开文件所在路径:" + command);
    system(command.toLocal8Bit().data());
}
void AutoStartWidget::runProgram()
{
    QString path = _tableWidget->item(_tableWidget->currentRow(), 1)->text();
    QUrl url = QUrl::fromLocalFile(path);
    _logger->info("打开应用程序:" + url.toString());
    QDesktopServices::openUrl(url);
}

void AutoStartWidget::addProgram()
{
    Program program;
    ProgramDialog *dialog = new ProgramDialog(this,_config->read("DirPath").value.toString(), &program, false);
    if (dialog->exec() == QDialog::Accepted)
    {
        _logger->info(program.toString());
        // 判断是否已经存在
        for (int i = 0; i < _tableWidget->rowCount(); i++)
        {
            if (_tableWidget->item(i, 0)->text() == program.name)
            {
                QMessageBox::warning(this, "警告", "程序名称重复!");
                _logger->warn("程序名称重复!" + program.name);
                return;
            }
        }
        ljz::LFunc::autoRunPath(1,program.name, program.path, program.arguments);
        _tableWidget->setRowCount(_tableWidget->rowCount() + 1);
        _tableWidget->setItem(_tableWidget->rowCount() - 1, 0, new QTableWidgetItem(program.name));
        _tableWidget->setItem(_tableWidget->rowCount() - 1, 1, new QTableWidgetItem(program.path));
        _tableWidget->setItem(_tableWidget->rowCount() - 1, 2, new QTableWidgetItem(program.arguments));
    }
}

void AutoStartWidget::removeProgram()
{
    int row = _tableWidget->currentRow();
    if (row == -1)
    {
        return;
    }
    if(QMessageBox::question(this, "警告", "确定删除"+_tableWidget->item(row, 0)->text()+"吗?\n请尽量备份后再删除!", QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
    {
        return;
    }
    ljz::LFunc::autoRunPath(0, _tableWidget->item(row, 0)->text(), _tableWidget->item(row, 1)->text(), _tableWidget->item(row, 2)->text());
    _logger->info(QString("\{\"name\":\"%1\",\"path\":\"%2\",\"arguments\":\"%3\"\}").arg(_tableWidget->item(row, 0)->text()).arg(_tableWidget->item(row, 1)->text()).arg(_tableWidget->item(row, 2)->text()));
    _tableWidget->removeRow(row);
}

void AutoStartWidget::editProgram()
{
    Program program;
    program.name = _tableWidget->item(_tableWidget->currentRow(), 0)->text();
    program.path = _tableWidget->item(_tableWidget->currentRow(), 1)->text();
    program.arguments = _tableWidget->item(_tableWidget->currentRow(), 2)->text();
    ProgramDialog *dialog = new ProgramDialog(this,_config->read("DirPath").value.toString(), &program, true);
    if (dialog->exec() == QDialog::Accepted)
    {
        _logger->info(program.toString());
        ljz::LFunc::autoRunPath(1,program.name, program.path, program.arguments);
        _tableWidget->setItem(_tableWidget->currentRow(), 0, new QTableWidgetItem(program.name));
        _tableWidget->setItem(_tableWidget->currentRow(), 1, new QTableWidgetItem(program.path));
        _tableWidget->setItem(_tableWidget->currentRow(), 2, new QTableWidgetItem(program.arguments));
    }
}

void AutoStartWidget::backUp()
{
    QString filePath = QFileDialog::getSaveFileName(this, "选择备份文件路径", QApplication::applicationDirPath(), "*.json");
    if (filePath.isEmpty())
    {
        return;
    }
    filePath = filePath.split(".")[0] += ".json";
    QFile file(filePath);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::critical(this, "错误", "备份配置文件失败");
        _logger->warn("备份失败");
        return;
    }
    QJsonArray array;
    for(int i = 0; i < _tableWidget->rowCount(); i++)
    {
        array.append(QJsonObject({
            {"name", _tableWidget->item(i, 0)->text()},
            {"path", _tableWidget->item(i, 1)->text()},
            {"arguments", _tableWidget->item(i, 2)->text()}
        }));
    }
    QJsonDocument doc(array);
    file.write(doc.toJson());
    file.close();
    _logger->info("备份到:" + filePath);
}
