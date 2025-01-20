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

        programs.append(program);
    }
    return programs;
}

void AutoStartWidget::showTableMenu(const QPoint &pos)
{
    QPoint globalPos = QCursor::pos();
    QMenu menu(this);
    QAction *addAction = menu.addAction("添加");
    QAction *removeAction = menu.addAction("删除");
    QAction *editAction = menu.addAction("编辑");
    QAction *backAction = menu.addAction("备份");
    // connect(addAction, &QAction::triggered, this, &AutoStartWidget::addProgram);
    // connect(removeAction, &QAction::triggered, this, &AutoStartWidget::removeProgram);
    // connect(editAction, &QAction::triggered, this, &AutoStartWidget::editProgram);
    menu.exec(globalPos);

}
