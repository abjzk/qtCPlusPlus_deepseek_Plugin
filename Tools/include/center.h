#include "ui_center.h"

class Center : public QWidget
{
    Q_OBJECT
public:
    explicit Center(QWidget *parent = nullptr);
    ~Center();

private:
    Ui::Center *ui;
};