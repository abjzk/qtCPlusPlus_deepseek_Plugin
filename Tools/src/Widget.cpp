#include "Widget.h"
#include <QTimer>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QLineEdit>
#include <LCore>

Widget::Widget(QWidget *mainWidget, QWidget *parent)
    :LWidget(new LTitleBar(), mainWidget, parent)
{
    auto info = LWidget::info();
    info.edgeSize = 8;
    info.splitLineColor = Qt::GlobalColor::transparent;
    LWidget::setInfo(info);
    LTitleBar *titleBar = qobject_cast<LTitleBar*>(this->getTitleBar());
    titleBar->setTitleIcon(QIcon(":res/Tools.ico"));
    titleBar->setTitleText("");
    this->systemSettingsChangedSlot();
    connect(this,&Widget::systemSettingsChanged,[=]()
    {
        QTimer::singleShot(100, this, [=]() {this->systemSettingsChangedSlot();});
    });
}

void Widget::systemSettingsChangedSlot()
{
    QMap<LTitleBar::ButtonIcon, QIcon> iconMap{
        {LTitleBar::ButtonIcon::CloseButtonIcon, LFunc::getoppositeColorIcon(QIcon(":/res/icon/close.png"),this->palette().window().color())},
        {LTitleBar::ButtonIcon::MinButtonIcon, LFunc::getoppositeColorIcon(QIcon(":/res/icon/minimize.png"),this->palette().window().color())},
        {LTitleBar::ButtonIcon::MaxButtonIcon, LFunc::getoppositeColorIcon(QIcon(":/res/icon/maximize.png"),this->palette().window().color())},
        {LTitleBar::ButtonIcon::RestoreButtonIcon, LFunc::getoppositeColorIcon(QIcon(":/res/icon/restore.png"),this->palette().window().color())},
    };
    LTitleBar *titleBar = qobject_cast<LTitleBar*>(this->getTitleBar());
    titleBar->setStandardIconMap(iconMap);
    QList<QPushButton*> list = this->findChildren<QPushButton*>();
    for (QPushButton *button : list)
    {
      button->setIcon(LFunc::getoppositeColorIcon(button->icon(),this->palette().window().color()));
    }
    QList<QTreeWidget*> list2 = this->findChildren<QTreeWidget*>();
    for (QTreeWidget *item : list2)
    {
        for (int i = 0; i < item->topLevelItemCount(); i++)
        {
            QTreeWidgetItem *topItem = item->topLevelItem(i);
            topItem->setIcon(0, LFunc::getoppositeColorIcon(topItem->icon(0),(this->palette().window().color())));
            for (int j = 0; j < topItem->childCount(); j++)
            {
                QTreeWidgetItem *childItem = topItem->child(j);
                childItem->setIcon(0, LFunc::getoppositeColorIcon(childItem->icon(0),(this->palette().window().color())));
            }
        }
    }
    QList<QAction*> list3 = this->findChildren<QAction*>();
    for (QAction *action : list3)
    {
        action->setIcon(LFunc::getoppositeColorIcon(action->icon(),(this->palette().window().color())));
    }
    QList<QLineEdit*> list4 = this->findChildren<QLineEdit*>();
    for (QLineEdit *edit : list4)
    {
        // 获取所有的QAction对象
        QList<QAction*> actions = edit->actions();
        // 遍历所有的QAction对象
        for (QAction* action : actions) {
            // 设置图标
            action->setIcon(LFunc::getoppositeColorIcon(action->icon(),(this->palette().window().color())));
        }
    }
}

LTitleBar *Widget::getTitleBar()
{
    return qobject_cast<LTitleBar*>(LWidget::getTitleBar());
}
