#pragma once
#include <QDialog>
#include <QJsonObject>
#include <QLayout>
#include <QLabel>
#include <LWidget>
#include <QLineEdit>
#include <QFormLayout>
#include <QDialogButtonBox>

class Program
{
public:
    Program() = default;
    Program(QString icon, QString name, QString path, QString arguments) : icon(icon), name(name), path(path), arguments(arguments) {}
    Program(QJsonObject jsonObject) : icon(jsonObject.value("icon").toString()), name(jsonObject.value("name").toString()), path(jsonObject.value("path").toString()), arguments(jsonObject.value("arguments").toString()) {};
    QString icon = "";
    QString name = "";
    QString path = "";
    QString arguments = "";
    QString toString()
    {
        return QString(R"({"icon": "%1","name": "%2","path": "%3","arguments": "%4"}})")
            .arg(icon, name, path, arguments);
    }
};

class ProgramDialog : public QDialog
{
    Q_OBJECT
public:
    ProgramDialog(QWidget *parent = nullptr,QString path = "",Program * program = nullptr,bool isEdit = true);
    ~ProgramDialog();

private:
    void initUi();
    void initConnect();
    Program *_program = nullptr;
    bool _isEdit = true;
    QLineEdit *_nameEdit = new QLineEdit();
    LFileLineEdit *_pathEdit = new LFileLineEdit();
    QLineEdit *_argumentsEdit = new QLineEdit();
    QDialogButtonBox *_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    QString _path = "";
};