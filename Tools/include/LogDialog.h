#pragma once
#include "Widget.h"
#include <QTableWidget>
#include <QTableWidgetItem>
#include <Logger.h>
#include <qlayout.h>
#include <LWidget>


class LogTableWidget:public QWidget
{
    Q_OBJECT
private:
    QVBoxLayout* layout;
    QHBoxLayout* buttonLayout;
    QPushButton* clearButton;
    QPushButton* autoScrollButton;
public:
    LogTableWidget(QWidget *parent = nullptr);
    ~LogTableWidget();
private:
    void initUi();
    void initConnect();
};




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
