#ifndef FILEANALYSISTASK_H
#define FILEANALYSISTASK_H
#pragma once

#include <AbstractPlugin.h>
#include <QRunnable>
#include <QString>
#include <QMutex>
#include <QWidget>
#include <QWaitCondition>
#include <QVector>
#include <QPair>
#include <LCore> // 包含LSqlExecutor定义
#include <QtCore/qglobal.h>
#include <utility>
#include <config.h>
// 分析结果回调函数类型
typedef std::function<QString(const QString&,const QString&)> AnalysisCallback;
class FileAnalysisTask : public QRunnable {
public:

    FileAnalysisTask(const QString& filePath,
                     const QString& outputDir,
                     const QString& programID,
                     jzk::LSqlExecutor* sqlExecutor,
                     Logger* logger,
                     QMutex* mutex,
                     QWaitCondition* condition,
                     QList<QPair<QString, QString>>* results, // 改为 QList
                     AnalysisCallback analysisCallback,
                     const QString& jsonTemplatePath,
                     const QStringList& userRules
                     );

    void run() override;
    QString readFileWithTruncation(const QString& filePath);
    QString truncateQuestionIfNeeded(const QString& question);
    QString generateQuestion(const QString& filePath, const QString& content,bool isdataUp);
    QString generateDefaultQuestion(const QString& filePath, const QString& content);
    QJsonObject createErrorReport(const QString& content);
    void saveJsonToFile(const QString& filePath, const QJsonObject& json);
    void saveToFile(const QString& filePath, const QString& content);
    QJsonObject extractJsonReport(const QString& analysisResult);
    QString run1();
private:


    void saveResult(const QString& analysisResult);
    QString m_filePath;
    QString m_outputDir;
    QString m_programID;
    QStringList m_userRules;
    jzk::LSqlExecutor* m_sqlExecutor; // 添加命名空间
    Logger* m_logger;
    QMutex* m_mutex;
    QWaitCondition* m_condition;
    QList<QPair<QString, QString>>* m_results; // 改为 QList
    AnalysisCallback m_analysisCallback;
    QString m_jsonTemplatePath;
};
#endif // FILEANALYSISTASK_H
