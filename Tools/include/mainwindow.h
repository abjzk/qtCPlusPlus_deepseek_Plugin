#include <JZKWidget.h>
#include "Widget.h"
#include "config.h"
#include <AbstractPlugin.h>
#include <QCloseEvent>
#include "configDialog.h"
using namespace jzk;

class TitleBar : public LBaseTitleBar
{
    Q_OBJECT
public:
    TitleBar(QWidget *parent = nullptr);
    ~TitleBar() override = default;

private:
    QWidget *getMaxButton() override;
public slots:
    virtual void updateIcon();
    virtual void closeButtonClick();
    virtual void minButtonClick();
};


class MainWindow : public Widget
{
    Q_OBJECT
public:
    MainWindow(TConfig *config,QWidget* mainWidget, QWidget* parent = nullptr);
    ~MainWindow() override;
    virtual void systemSettingsChangedSlot() override;
    void initUi();
    void initConnect();
    void showConfigDialog();
    virtual void closeEvent(QCloseEvent *event) override;
    TConfig * _config;
private:
    QPushButton * _setButton = new QPushButton(this);
    ConfigDialog * dialog = nullptr;

    void reSet();
};
