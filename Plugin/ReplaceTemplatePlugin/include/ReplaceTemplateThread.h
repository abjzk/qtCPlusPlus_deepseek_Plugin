#pragma once
#include <QThread>
#include "ReplaceTemplatePlugin_global.h"

class ReplaceTemplatePlugin_EXPORT ReplaceTemplateThread : public QThread
{
    Q_OBJECT
public:
    ReplaceTemplateThread(QObject *parent = nullptr);
    ~ReplaceTemplateThread();
    virtual void run() override;
    QString templatePath = "";
    QString outPath = "";
    QString templateName = "";
    QString outName = "";
    QStringList binType = {};
    bool isRecursive = true;
    bool isre = false;
    bool isGenerateTopFloder = true;
    void addMessage(const QString& message,const QString& type="INFO");
signals:
    void message(const QString& message);
private:
    void convertFile(const QString& path,const QString& newPath);
    void copyFile(const QString& srcPath, const QString& destPath);
};