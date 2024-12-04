#pragma once
#include <Widget.h>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <LWidget>
#include <QDialog>
#include <config.h>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QCloseEvent>
#include <QVBoxLayout>

class ConfigDialog : public Widget
{
    Q_OBJECT

public:
    ConfigDialog(TConfig *config,QWidget* parent = nullptr);
    ~ConfigDialog() override;
    void initUi();
    void loadConfig();
    void initConnect();
    QWidget* createValueWidget(LLabelWidgetFrame* frame,QString type, QString value);
    virtual void closeEvent(QCloseEvent *event) override;
Q_SIGNALS:
    void accepted();
    void rejected();
    void closed();
public slots:
    void accept();
    void reject();
    void exec();
    void saved();
    void valueChanged(QString key, QVariant value);
    void setValue(QWidget* widget, QVariant value);
private:
    TConfig * _config;
    QDialogButtonBox *buttonBox;
    QVBoxLayout *layout;
    bool isRead = false;
};