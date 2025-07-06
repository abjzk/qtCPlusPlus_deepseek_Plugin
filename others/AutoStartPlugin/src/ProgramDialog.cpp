#include "ProgramDialog.h"

ProgramDialog::ProgramDialog(QWidget *parent,QString path, Program *program,bool isEdit)
    :QDialog(parent),_program(program),_isEdit(isEdit),_path(path)
{
    this->initUi();
    this->initConnect();
}

ProgramDialog::~ProgramDialog()
{
}

void ProgramDialog::initUi()
{
    QVBoxLayout *_layout = new QVBoxLayout();
    QFormLayout *layout = new QFormLayout();
    layout->addRow("名称",_nameEdit);
    layout->addRow("路径",_pathEdit);
    auto info = _pathEdit->info();
    info.mode = QFileDialog::FileMode::AnyFile;
    info.path = _path;
    info.filters = {"可执行文件(*.exe)"};
    _pathEdit->setInfo(info);
    layout->addRow("启动参数",_argumentsEdit);
    _layout->addLayout(layout);
    _layout->addWidget(_buttonBox);
    this->setLayout(_layout);
    if (_isEdit)
    {
        _nameEdit->setText(_program->name);
        _pathEdit->setText(_program->path);
        _argumentsEdit->setText(_program->arguments);
    }
}

void ProgramDialog::initConnect()
{
    connect(_buttonBox, &QDialogButtonBox::accepted, [=]()
    {
        _program->name = _nameEdit->text();
        _program->path =  _pathEdit->text().replace("/", "\\");
        _program->arguments = _argumentsEdit->text();
        this->accept();
    });
    connect(_buttonBox, &QDialogButtonBox::rejected, this, &ProgramDialog::reject);
    connect(_pathEdit, &LFileLineEdit::fileSelected, [=]()
    {
        auto path = _pathEdit->text();
        _nameEdit->setText(QFileInfo(path).fileName().split(".")[0]);
    });
}
