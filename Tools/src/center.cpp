#include "center.h"

Center::Center(QWidget *parent)
    : QWidget(parent), ui(new Ui::Center)
{

    ui->setupUi(this);
}

Center::~Center()
{
    delete ui;
}
