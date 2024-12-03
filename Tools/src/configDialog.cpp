#include "configDialog.h"
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QColorDialog>
#include <QDateTimeEdit>
#include <QDateEdit>
#include <QTimeEdit>
#ifdef WIN32
#include <Windows.h>
#pragma comment (lib,"user32.lib")
#endif // WIN32
#include <windowsx.h>
#include <LWidget>

ConfigDialog::ConfigDialog(QString name, QWidget *parent)
    : Widget(new QWidget(), parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint| Qt::Dialog | Qt::WindowStaysOnTopHint);
    // this->setFixedWidth(200);
#ifdef Q_OS_WIN
	HWND hwnd = reinterpret_cast<HWND>(this->winId());
	DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);
	::SetWindowLong(hwnd, GWL_STYLE, style | WS_CAPTION);
#endif
    
    this->getTitleBar()->getMinButton()->setVisible(false);
    this->getTitleBar()->getMaxButton()->setVisible(false);
    this->_config = new TConfig(name, this);
    this->initUi();
    this->initConnect();
    systemSettingsChangedSlot();
}

ConfigDialog::~ConfigDialog()
{
}

void ConfigDialog::initUi()
{
    QVBoxLayout *hlayout = new QVBoxLayout(this);
    layout = new QVBoxLayout(this);
    // 整体右对齐
    layout->setContentsMargins(0, 0, 0, 0);
    QList<QVariantMap> maps = this->_config->readAllAndDescription();
    int count = maps.count();
    this->setFixedHeight(count * 30 + 100);
    for (const auto &map : maps)
    {
        QWidget * valueWidget = this->createValueWidget(map["type"].toString(), map["value"].toString());
        LLabelWidgetFrame *labelWidget = new LLabelWidgetFrame(map["description"].toString(), valueWidget, this);
        layout->addWidget(labelWidget);
    }
    hlayout->addLayout(layout);
    hlayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    hlayout->setContentsMargins(0, 10, 0, 0);
    // 添加QDialog的按钮
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    hlayout->addWidget(buttonBox);
    this->_mainWidget->setLayout(hlayout);
}

void ConfigDialog::initConnect()
{
    connect(buttonBox, &QDialogButtonBox::accepted, this, &ConfigDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ConfigDialog::reject);
}

QWidget* ConfigDialog::createValueWidget(QString type, QString value)
{
    TConfig::Type typeEnum = TConfig::getTypeFromString(type);
    switch (typeEnum)
    {
    case TConfig::Type::Int:
    {
        QSpinBox *spinBox = new QSpinBox(this);
        spinBox->setValue(TConfig::stringToValue(value, type).toInt());
        return spinBox;
    }
    case TConfig::Type::String:
    {
        QLineEdit *lineEdit = new QLineEdit(this);
        lineEdit->setText(TConfig::stringToValue(value, type).toString());
        return lineEdit;
    }
    case TConfig::Type::Bool:
    {
        LSwitchButton *switchButton = new LSwitchButton(this);
        switchButton->setChecked(TConfig::stringToValue(value, type).toBool());
        switchButton->setFixedWidth(60);
        return switchButton;
    }
    case TConfig::Type::Float:
    {
        QLineEdit *lineEdit = new QLineEdit(this);
        lineEdit->setText(TConfig::stringToValue(value, type).toString());
        return lineEdit;
    }
    case TConfig::Type::Double:
    {
        QLineEdit *lineEdit = new QLineEdit(this);
        lineEdit->setText(TConfig::stringToValue(value, type).toString());
        return lineEdit;
    }
    case TConfig::Type::DateTime:
    {
        QDateTimeEdit *dateTimeEdit = new QDateTimeEdit(this);
        dateTimeEdit->setDateTime(TConfig::stringToValue(value, type).toDateTime());
        return dateTimeEdit;
    }
    case TConfig::Type::Date:
    {
        QDateEdit *dateEdit = new QDateEdit(this);
        dateEdit->setDate(TConfig::stringToValue(value, type).toDate());
        return dateEdit;
    }
    case TConfig::Type::Time:
    {
        QTimeEdit *timeEdit = new QTimeEdit(this);
        timeEdit->setTime(TConfig::stringToValue(value, type).toTime());
        return timeEdit;
    }
    case TConfig::Type::File:
    {
        LFileLineEdit *fileLineEdit = new LFileLineEdit(this);
        fileLineEdit->setText(TConfig::stringToValue(value, type).toString());
        return fileLineEdit;
    }
    case TConfig::Type::Directory:
    {
        LFileLineEdit *directoryLineEdit = new LFileLineEdit(this);
        auto info = directoryLineEdit->info();
        info.mode = QFileDialog::Directory;
        directoryLineEdit->setInfo(info);
        directoryLineEdit->setText(TConfig::stringToValue(value, type).toString());
        return directoryLineEdit;
    }
    default:
        return new QWidget();
    }

}

void ConfigDialog::closeEvent(QCloseEvent *event)
{
    emit this->closed();
    Widget::closeEvent(event);
}

void ConfigDialog::accept()
{
    this->saved();
    emit this->accepted();
    this->close();
}

void ConfigDialog::reject()
{
    emit this->rejected();
    this->close();
}

void ConfigDialog::exec()
{
    // 移动到屏幕中央
    this->move(QGuiApplication::primaryScreen()->geometry().center() - this->rect().center());
    this->show();
}

void ConfigDialog::saved()
{
    QMap<QString, QVariant> map;
    // 读取所有的行
    for (int i = 0; i < layout->count(); i++)
    {
        QLayoutItem * item = layout->itemAt(i);
        LLabelWidgetFrame * labelWidget = qobject_cast<LLabelWidgetFrame*>(item->widget());
        if (!labelWidget) continue;
        QWidget * widget = labelWidget->valueWidget();
        if (!widget) continue;
        QString className = widget->metaObject()->className();
        QString key = labelWidget->labelName();
        QVariant value;
        if (className.contains("QLineEdit"))
            value =  qobject_cast<QLineEdit*>(widget)->text();
        else if (className.contains("QSpinBox"))
            value = qobject_cast<QSpinBox*>(widget)->value();
        else if (className.contains("QDoubleSpinBox"))
			value = qobject_cast<QDoubleSpinBox*>(widget)->value();
        else if (className.contains("QDateTimeEdit"))
			value = qobject_cast<QDateTimeEdit*>(widget)->dateTime();
        else if (className.contains("QDateEdit"))
			value = qobject_cast<QDateEdit*>(widget)->date();
		else if (className.contains("QTimeEdit"))
			value = qobject_cast<QTimeEdit*>(widget)->time();
		else if (className.contains("LSwitchButton"))
			value = qobject_cast<LSwitchButton*>(widget)->isChecked();
		else if (className.contains("LFileLineEdit"))
			value = qobject_cast<LFileLineEdit*>(widget)->text();
		else
			continue;      
        map.insert(key, value);
    }
    if (map.isEmpty()) return;
    _config->write(map);
}
