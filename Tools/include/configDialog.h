#include <Widget.h>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <LWidget>
#include <QDialog>
#include <config.h>

class ConfigDialog : public Widget
{
    Q_OBJECT

public:
    ConfigDialog(QString name,QWidget* parent = nullptr);
    ~ConfigDialog() override;
    void initUi();
    void initConnect();
Q_SIGNALS:
    void accepted();
    void rejected();
public slots:
    void accept();
    void reject();
    void exec();
    void saved(bool init = false);
private:
    TConfig * _config;
};