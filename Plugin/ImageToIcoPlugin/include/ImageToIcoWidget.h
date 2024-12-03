#include "ImageToIcoPlugin_global.h"
#include "ui_ImageToIcoPlugin.h"
#include <config.h>
class ImageToIcoPlugin_EXPORT ImageToIcoWidget : public QWidget
{
    Q_OBJECT
public:
    ImageToIcoWidget(TConfig *config,QWidget *parent = nullptr);
    ~ImageToIcoWidget();
private:
    void initUi();
    void initConnect();
    void loadTable(const QStringList &files,const QString filter);
    void startConvert();
    Ui::ImageToIcoPlugin * ui;
    TConfig * _config;
};