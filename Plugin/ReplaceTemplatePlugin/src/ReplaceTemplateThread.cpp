#include "ReplaceTemplateThread.h"
#include "QDateTime"
#include "QDir"
#include "QFile"
#include "QRegularExpression"
#include <QStringDecoder>

ReplaceTemplateThread::ReplaceTemplateThread(QObject *parent)
    :QThread(parent)
{
}

ReplaceTemplateThread::~ReplaceTemplateThread()
{
}

void ReplaceTemplateThread::run()
{
    this->addMessage("开始执行");
    this->addMessage(QString("模板文件路径:%1").arg(this->templatePath));
    this->addMessage(QString("输出文件路径:%1").arg(this->outPath));
    this->addMessage(QString("待替换文本:%1").arg(this->templateName));
    this->addMessage(QString("替换后文本:%1").arg(this->outName));
    this->addMessage(QString("二进制文件类型:%1").arg(this->binType.join(";")));
    this->addMessage(QString("是否使用正则:%1").arg(this->isre ? "是" : "否"));
    this->addMessage(QString("是否递归:%1").arg(this->isRecursive ? "是" : "否"));
    this->addMessage(QString("是否生成顶层文件夹:%1").arg(this->isGenerateTopFloder ? "是" : "否"));
    this->convertFile(this->templatePath,this->outPath);
    this->addMessage("执行完成");
}

void ReplaceTemplateThread::addMessage(const QString &message,const QString& type)
{
    QString msg = QString("[%1]     [%2]     [%3]。").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(type).arg(message);
    emit this->message(msg);
}

void ReplaceTemplateThread::convertFile(const QString &path, const QString& newPath) 
{
    // 判断该目录或文件是否存在
    if (!QFile::exists(path))
    {
        this->addMessage(QString("文件或目录不存在：%1").arg(path),"ERROR");
        return;
    }
    // 获取文件信息
    QFileInfo info(path);
    // 文件名
    QString fileName = info.fileName();
    // 文件类型
    QString fileType = info.completeSuffix();
    // 判断文件中是否包含待替换文本
    QString newFileName = "";
    QString oldFilePath = info.path() + "/" + fileName;
    if (this->isre)
        newFileName = fileName.replace(QRegularExpression(this->templateName), this->outName);
    else
        newFileName = fileName.replace(this->templateName, this->outName);
    QString newFilePath = (this->isGenerateTopFloder ? newPath : this->outPath)  + "/" + newFileName;
    // 如果是文件
    if (info.isFile())
    {
        this->copyFile(oldFilePath, newFilePath);
    }
    else if (info.isDir())
    {
        // 如果是文件夹
        QDir dir(oldFilePath);
        QDir dir1(newFilePath);
        if (!dir1.exists())
        {
            dir1.mkpath(newFilePath);
        }
        addMessage(QString("复制目录:%1 -> %2").arg(oldFilePath).arg(newFilePath));
        if(!isRecursive) return;
        QStringList list = dir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);
        for (int i = 0; i < list.size(); i++)
        {
            this->convertFile(dir.path() + "/" + list.at(i), this->isGenerateTopFloder ? dir1.path() : this->outPath);
        }
    }
}

void ReplaceTemplateThread::copyFile(const QString &srcPath, const QString &destPath)
{
    this->addMessage(QString("复制文件:%1 -> %2").arg(srcPath).arg(destPath));
    QFile file(srcPath);
    if (!file.exists())
    {
        this->addMessage("文件不存在", "ERROR");
        return;
    }
    if (!file.open(QFile::ReadOnly))
    {
        this->addMessage("文件打开失败", "ERROR");
        return;
    }
    QByteArray array = file.readAll();
    file.close();
    QString encodeing = "UTF-8";
    QStringDecoder decoder(encodeing.toStdString().c_str());
    QString data = decoder.decode(array);

    if(decoder.hasError())
    {
        encodeing = "GBK";
        decoder = QStringDecoder(encodeing.toStdString().c_str());
        data = decoder.decode(array);
        if(decoder.hasError())
        {
            encodeing = "";
            this->addMessage("未识别的文件编码,视为二进制文件，不做内容替换处理", "WARNING");
            // 直接将文件复制过去
            QFile::copy(srcPath, destPath);
            return;
        }
    }
    if(isre)
    {
        data = data.replace(QRegularExpression(this->templateName), this->outName);
    }
    else
    {
        data = data.replace(this->templateName, this->outName);
    }

    if(encodeing != "")
    {
        this->addMessage(QString("替换 %1 文件内容").arg(srcPath));
        this->addMessage(QString("文件编码为:%1").arg(encodeing));
        QStringEncoder encoder(encodeing.toStdString().c_str());
        QByteArray array = encoder.encode(data.toUtf8());
        QFile file(destPath);
        if (!file.open(QFile::WriteOnly))
        {
            this->addMessage("文件打开失败", "ERROR");
            return;
        }
        file.write(array);
        file.close();
    }
}
