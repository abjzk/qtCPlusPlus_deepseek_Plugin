#include "LogDialog.h"
#include <QHeaderView>
#include <QScrollBar>

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
    // 置顶
    // this->setWindowFlags(this->windowFlags() | Qt::WindowType::WindowStaysOnTopHint);
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
