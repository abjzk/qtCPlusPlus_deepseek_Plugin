#pragma once
#include <LJZWidget.h>
#include <QPushButton>
using namespace ljz;


class Widget : public LWidget
{
    Q_OBJECT
public:
    Widget(QWidget* mainWidget, QWidget* parent = nullptr);
    ~Widget() override = default;
    LTitleBar *getTitleBar();
public slots:
    virtual void systemSettingsChangedSlot();
public:
    static QIcon setIconColor(const QIcon& icon, const QColor& color);
    static QColor oppositeColor(const QColor& color);

};