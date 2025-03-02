/********************************************************************************
** Form generated from reading UI file 'center.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CENTER_H
#define UI_CENTER_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Center
{
public:
    QGridLayout *gridLayout_2;
    QSplitter *splitter;
    QTreeWidget *pluginTree;
    QWidget *widget;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer_3;
    QLabel *authorlabel;
    QPushButton *pushButton;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QLabel *nameLabel;
    QLabel *versionlabel;
    QPushButton *logButton;

    void setupUi(QWidget *Center)
    {
        if (Center->objectName().isEmpty())
            Center->setObjectName("Center");
        Center->resize(891, 568);
        gridLayout_2 = new QGridLayout(Center);
        gridLayout_2->setObjectName("gridLayout_2");
        splitter = new QSplitter(Center);
        splitter->setObjectName("splitter");
        splitter->setOrientation(Qt::Orientation::Horizontal);
        pluginTree = new QTreeWidget(splitter);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("\346\217\222\344\273\266"));
        pluginTree->setHeaderItem(__qtreewidgetitem);
        pluginTree->setObjectName("pluginTree");
        pluginTree->setMaximumSize(QSize(200, 16777215));
        splitter->addWidget(pluginTree);
        widget = new QWidget(splitter);
        widget->setObjectName("widget");
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 0, 4, 1, 1);

        authorlabel = new QLabel(widget);
        authorlabel->setObjectName("authorlabel");
        authorlabel->setFrameShape(QFrame::Shape::WinPanel);

        gridLayout->addWidget(authorlabel, 0, 2, 1, 1);

        pushButton = new QPushButton(widget);
        pushButton->setObjectName("pushButton");
        pushButton->setMaximumSize(QSize(25, 25));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/res/icon/set.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton->setIcon(icon);

        gridLayout->addWidget(pushButton, 0, 6, 1, 1);

        scrollArea = new QScrollArea(widget);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 666, 518));
        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout->addWidget(scrollArea, 1, 0, 1, 7);

        nameLabel = new QLabel(widget);
        nameLabel->setObjectName("nameLabel");
        nameLabel->setFrameShape(QFrame::Shape::WinPanel);

        gridLayout->addWidget(nameLabel, 0, 1, 1, 1);

        versionlabel = new QLabel(widget);
        versionlabel->setObjectName("versionlabel");
        versionlabel->setFrameShape(QFrame::Shape::WinPanel);

        gridLayout->addWidget(versionlabel, 0, 3, 1, 1);

        logButton = new QPushButton(widget);
        logButton->setObjectName("logButton");
        logButton->setMaximumSize(QSize(25, 25));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/res/icon/log.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        logButton->setIcon(icon1);

        gridLayout->addWidget(logButton, 0, 5, 1, 1);

        splitter->addWidget(widget);

        gridLayout_2->addWidget(splitter, 0, 0, 1, 1);


        retranslateUi(Center);

        QMetaObject::connectSlotsByName(Center);
    } // setupUi

    void retranslateUi(QWidget *Center)
    {
        Center->setWindowTitle(QCoreApplication::translate("Center", "Form", nullptr));
#if QT_CONFIG(tooltip)
        widget->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        authorlabel->setToolTip(QCoreApplication::translate("Center", "\346\217\222\344\273\266\344\275\234\350\200\205", nullptr));
#endif // QT_CONFIG(tooltip)
        authorlabel->setText(QCoreApplication::translate("Center", "\344\275\234\350\200\205", nullptr));
#if QT_CONFIG(tooltip)
        pushButton->setToolTip(QCoreApplication::translate("Center", "\346\217\222\344\273\266\350\256\276\347\275\256", nullptr));
#endif // QT_CONFIG(tooltip)
        pushButton->setText(QString());
#if QT_CONFIG(tooltip)
        nameLabel->setToolTip(QCoreApplication::translate("Center", "\346\217\222\344\273\266\345\220\215\347\247\260", nullptr));
#endif // QT_CONFIG(tooltip)
        nameLabel->setText(QCoreApplication::translate("Center", "\345\220\215\347\247\260", nullptr));
#if QT_CONFIG(tooltip)
        versionlabel->setToolTip(QCoreApplication::translate("Center", "\346\217\222\344\273\266\347\211\210\346\234\254", nullptr));
#endif // QT_CONFIG(tooltip)
        versionlabel->setText(QCoreApplication::translate("Center", "\347\211\210\346\234\254", nullptr));
#if QT_CONFIG(tooltip)
        logButton->setToolTip(QCoreApplication::translate("Center", "\346\217\222\344\273\266\346\227\245\345\277\227", nullptr));
#endif // QT_CONFIG(tooltip)
        logButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Center: public Ui_Center {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CENTER_H
