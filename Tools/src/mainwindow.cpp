#include "mainwindow.h"
#include <QHBoxLayout>
#include <QTimer>
TitleBar::TitleBar(QWidget *parent)
    :LBaseTitleBar(parent)
{
}

QWidget *TitleBar::getMaxButton()
{
    return nullptr;
}
void TitleBar::updateIcon()
{

}
void TitleBar::closeButtonClick()
{
    
}

void TitleBar::minButtonClick()
{
}

MainWindow::MainWindow(QWidget *mainWidget, QWidget *parent)
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
    connect(this,&MainWindow::systemSettingsChanged,[=]()
    {
        QTimer::singleShot(100, this, [=]() {this->systemSettingsChangedSlot();});
    });
}

void MainWindow::systemSettingsChangedSlot()
{
    QMap<LTitleBar::ButtonIcon, QIcon> iconMap{
        {LTitleBar::ButtonIcon::CloseButtonIcon, setIconColor(QIcon(":/res/icon/close.png"),oppositeColor(this->palette().window().color()))},
        {LTitleBar::ButtonIcon::MinButtonIcon, setIconColor(QIcon(":/res/icon/minimize.png"),oppositeColor(this->palette().window().color()))},
        {LTitleBar::ButtonIcon::MaxButtonIcon, setIconColor(QIcon(":/res/icon/maximize.png"),oppositeColor(this->palette().window().color()))},
        {LTitleBar::ButtonIcon::RestoreButtonIcon, setIconColor(QIcon(":/res/icon/restore.png"),oppositeColor(this->palette().window().color()))},
    };
    LTitleBar *titleBar = qobject_cast<LTitleBar*>(this->getTitleBar());
    titleBar->setStandardIconMap(iconMap);

}

QIcon MainWindow::setIconColor(const QIcon &icon, const QColor &color)
{
    QPixmap pixmap = icon.pixmap(QSize(64,64));
    QPainter painter(&pixmap);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(pixmap.rect(), color);
    QIcon colorIcon = QIcon(pixmap);
    return colorIcon;
}

QColor MainWindow::oppositeColor(const QColor &color)
{
    return QColor(255 - color.red(), 255 - color.green(), 255 - color.blue());
}