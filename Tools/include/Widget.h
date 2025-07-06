#pragma once
#include <JZKWidget.h>
#include <QPushButton>
using namespace jzk;


class Widget : public LWidget
{
    Q_OBJECT
public:
    Widget(QWidget* mainWidget, QWidget* parent = nullptr);
    ~Widget() override = default;
    LTitleBar *getTitleBar();
public slots:
    virtual void systemSettingsChangedSlot();
};
