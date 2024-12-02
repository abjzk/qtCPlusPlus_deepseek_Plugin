#include "Widget.h"
#include <QTimer>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QLineEdit>

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
        {LTitleBar::ButtonIcon::CloseButtonIcon, setIconColor(QIcon(":/res/icon/close.png"),oppositeColor(this->palette().window().color()))},
        {LTitleBar::ButtonIcon::MinButtonIcon, setIconColor(QIcon(":/res/icon/minimize.png"),oppositeColor(this->palette().window().color()))},
        {LTitleBar::ButtonIcon::MaxButtonIcon, setIconColor(QIcon(":/res/icon/maximize.png"),oppositeColor(this->palette().window().color()))},
        {LTitleBar::ButtonIcon::RestoreButtonIcon, setIconColor(QIcon(":/res/icon/restore.png"),oppositeColor(this->palette().window().color()))},
    };
    LTitleBar *titleBar = qobject_cast<LTitleBar*>(this->getTitleBar());
    titleBar->setStandardIconMap(iconMap);
    QList<QPushButton*> list = this->findChildren<QPushButton*>();
    for (QPushButton *button : list)
    {
      button->setIcon(setIconColor(button->icon(),oppositeColor(this->palette().window().color())));
    }
    QList<QTreeWidget*> list2 = this->findChildren<QTreeWidget*>();
    for (QTreeWidget *item : list2)
    {
        for (int i = 0; i < item->topLevelItemCount(); i++)
        {
            QTreeWidgetItem *topItem = item->topLevelItem(i);
            topItem->setIcon(0, setIconColor(topItem->icon(0),oppositeColor(this->palette().window().color())));
            for (int j = 0; j < topItem->childCount(); j++)
            {
                QTreeWidgetItem *childItem = topItem->child(j);
                childItem->setIcon(0, setIconColor(childItem->icon(0),oppositeColor(this->palette().window().color())));
            }
        }
    }
    QList<QAction*> list3 = this->findChildren<QAction*>();
    for (QAction *action : list3)
    {
        action->setIcon(setIconColor(action->icon(),oppositeColor(this->palette().window().color())));
    }
    QList<QLineEdit*> list4 = this->findChildren<QLineEdit*>();
    for (QLineEdit *edit : list4)
    {
        // 获取所有的QAction对象
        QList<QAction*> actions = edit->actions();
        // 遍历所有的QAction对象
        for (QAction* action : actions) {
            // 设置图标
            action->setIcon(setIconColor(action->icon(),oppositeColor(this->palette().window().color())));
        }
    }
}

QIcon Widget::setIconColor(const QIcon &icon, const QColor &color)
{
    QPixmap pixmap = icon.pixmap(QSize(64,64));
    QPainter painter(&pixmap);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(pixmap.rect(), color);
    QIcon colorIcon = QIcon(pixmap);
    return colorIcon;
}

QColor Widget::oppositeColor(const QColor &color)
{
    return QColor(255 - color.red(), 255 - color.green(), 255 - color.blue());
}

LTitleBar *Widget::getTitleBar()
{
    return qobject_cast<LTitleBar*>(LWidget::getTitleBar());
}
