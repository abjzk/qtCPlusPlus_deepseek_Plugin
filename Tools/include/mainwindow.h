#include <LJZWidget.h>
using namespace ljz;

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


class MainWindow : public LWidget
{
    Q_OBJECT
public:
    MainWindow(QWidget* mainWidget, QWidget* parent = nullptr);
    ~MainWindow() override = default;
private:
    void systemSettingsChangedSlot();
    static QIcon setIconColor(const QIcon& icon, const QColor& color);
    static QColor oppositeColor(const QColor& color);
};