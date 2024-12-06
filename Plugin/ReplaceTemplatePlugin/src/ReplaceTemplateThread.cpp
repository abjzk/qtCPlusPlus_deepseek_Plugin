#include "ReplaceTemplateThread.h"
#include "QDateTime"
#include "QDir"
#include "QFile"
#include "QRegularExpression"
#include <QStringDecoder>
using namespace spdlog::level;
ReplaceTemplateThread::ReplaceTemplateThread(QObject *parent)
    :QThread(parent)
{
}

ReplaceTemplateThread::~ReplaceTemplateThread()
{
}

void ReplaceTemplateThread::run()
{
    this->addMessage(info,"开始执行");
    this->addMessage(info,QString("模板文件路径:%1").arg(this->templatePath));
    this->addMessage(info,QString("输出文件路径:%1").arg(this->outPath));
    this->addMessage(info,QString("待替换文本:%1").arg(this->templateName));
    this->addMessage(info,QString("替换后文本:%1").arg(this->outName));
    this->addMessage(info,QString("二进制文件类型:%1").arg(this->binType.join(";")));
    this->addMessage(info,QString("是否使用正则:%1").arg(this->isre ? "是" : "否"));
    this->addMessage(info,QString("是否递归:%1").arg(this->isRecursive ? "是" : "否"));
    this->addMessage(info,QString("是否生成顶层文件夹:%1").arg(this->isGenerateTopFloder ? "是" : "否"));
    this->convertFile(this->templatePath,this->outPath);
    this->addMessage(info,"执行完成");
}

void ReplaceTemplateThread::addMessage(spdlog::level::level_enum level,const QString& message)
{
    // QString msg = QString("[%1]     [%2]     [%3]。").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(type).arg(message);
    emit this->message(level,message);
}

void ReplaceTemplateThread::convertFile(const QString &path, const QString& newPath) 
{
    // 判断该目录或文件是否存在
    if (!QFile::exists(path))
    {
        this->addMessage(err,QString("文件或目录不存在：%1").arg(path));
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
        addMessage(level_enum::info, QString("复制目录:%1 -> %2").arg(oldFilePath).arg(newFilePath));
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
    this->addMessage(info,QString("复制文件:%1 -> %2").arg(srcPath).arg(destPath));
    QFile file(srcPath);
    if (!file.exists())
    {
        this->addMessage(err,"文件不存在");
        return;
    }
    if (!file.open(QFile::ReadOnly))
    {
        this->addMessage(err,"文件打开失败");
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
            this->addMessage(warn,"未识别的文件编码,视为二进制文件，不做内容替换处理");
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
        this->addMessage(info,QString("替换 %1 文件内容").arg(srcPath));
        this->addMessage(info,QString("文件编码为:%1").arg(encodeing));
        QStringEncoder encoder(encodeing.toStdString().c_str());
        QByteArray array = encoder.encode(data.toUtf8());
        QFile file(destPath);
        if (!file.open(QFile::WriteOnly))
        {
            this->addMessage(err,"文件打开失败");
            return;
        }
        file.write(array);
        file.close();
    }
}
