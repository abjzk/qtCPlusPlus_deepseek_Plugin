#pragma once
#include "Widget.h"
#include <QTableWidget>
#include <QTableWidgetItem>
#include <Logger.h>
class LogDialog : public Widget
{
    Q_OBJECT
private:
    /* data */
public:
    LogDialog(QWidget *parent = nullptr);
    ~LogDialog();
    QTableWidget* getTableWidget() { return qobject_cast<QTableWidget*>(this->getMainWidget()); };
    void addLogItem(const LoggerDetails& details);
};
