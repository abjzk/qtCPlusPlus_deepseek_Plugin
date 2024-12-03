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
    ConfigDialog(QString name,QWidget* parent = nullptr);
    ~ConfigDialog() override;
    void initUi();
    void initConnect();
    QWidget* createValueWidget(QString type, QString value);
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
private:
    TConfig * _config;
    QDialogButtonBox *buttonBox;
    QVBoxLayout *layout;
};