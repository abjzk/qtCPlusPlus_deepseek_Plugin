#include "LogDialog.h"
#include <QHeaderView>
#include <QScrollBar>
#include <QMenu>
#include <qdesktopservices.h>

LogTableWidget::LogTableWidget(QWidget *parent)
{
}

LogTableWidget::~LogTableWidget()
{
}

void LogTableWidget::initUi()
{
}

void LogTableWidget::initConnect()
{
}



LogDialog::LogDialog( QWidget *parent)
    :Widget(new QTableWidget(), parent)
{
    auto table = this->getTableWidget();
    table->setRowCount(0);
    table->setColumnCount(3);
    table->setColumnWidth(0, 100);
    table->setColumnWidth(1, 100);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setStretchLastSection(true);
    // 设置表头
    QStringList headerLabels;
    headerLabels << tr("时间") << tr("等级") << tr("消息");
    table->setHorizontalHeaderLabels(headerLabels);
    // 设置表头不可排序
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    // 右键菜单
    table->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(table, &QTableWidget::customContextMenuRequested, this, &LogDialog::showContextMenu);
}

LogDialog::~LogDialog()
{
}

void LogDialog::addLogItem(const LoggerDetails &details)
{
    auto table = this->getTableWidget();
    if (table->rowCount() > 100)
        table->removeRow(0);
    int curRow = table->rowCount();
    table->insertRow(curRow);
    table->setItem(curRow, 0, new QTableWidgetItem(details.date.toString("yyyy-MM-dd hh:mm:ss zzz")));
    table->setItem(curRow, 1, new QTableWidgetItem(details.level));
    table->setItem(curRow, 2, new QTableWidgetItem(details.message));
    spdlog::level::level_enum level = Logger::stringToLevel(details.level);
    // 跟日志等级设置对应的颜色
    if (level == spdlog::level::trace || level == spdlog::level::debug || level == spdlog::level::info)
        table->item(curRow, 1)->setForeground(QColor(0, 0, 0));
    else if (level == spdlog::level::warn)
        table->item(curRow, 1)->setForeground(QColor(255, 0, 0));
    else if (level == spdlog::level::err || level == spdlog::level::critical)
        table->item(curRow, 1)->setForeground(QColor(255, 0, 0));
    // 自动设置列宽
    table->resizeColumnsToContents();
    // 滚动到最后一行
    table->scrollToBottom();
}

void LogDialog::showContextMenu(const QPoint &pos)
{
    auto p = QCursor::pos();
    QMenu menu(this);
    auto openDir = menu.addAction("打开日志目录");
    auto clearDir = menu.addAction("清空");
    connect(openDir, &QAction::triggered, [=]() 
        {
            auto url = QUrl::fromLocalFile(QApplication::applicationDirPath() + "/logs/" + this->objectName());
            QDesktopServices::openUrl(url);
        }
    );
    connect(clearDir, &QAction::triggered,this->getTableWidget(),&QTableWidget::setRowCount);
    menu.exec(p);
}
