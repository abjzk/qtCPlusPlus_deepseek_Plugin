#include "FileAnalysisTask.h"
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSettings>
#include <QDir>

// 构造函数需要增加jsonTemplatePath参数
FileAnalysisTask::FileAnalysisTask(const QString& filePath,
                                   const QString& outputDir,
                                   const QString& programID,
                                   jzk::LSqlExecutor* sqlExecutor,
                                   Logger* logger,
                                   QMutex* mutex,
                                   QWaitCondition* condition,
                                   QList<QPair<QString, QString>>* results,
                                   AnalysisCallback analysisCallback,
                                   const QString& jsonTemplatePath, // 新增模板路径参数
                                   const QStringList& userRules)
    : m_filePath(filePath), m_outputDir(outputDir), m_programID(programID),
    m_sqlExecutor(sqlExecutor), m_logger(logger),
    m_mutex(mutex), m_condition(condition), m_results(results),
    m_analysisCallback(analysisCallback),
    m_jsonTemplatePath(jsonTemplatePath), // 初始化模板路径
    m_userRules(userRules)
{
    setAutoDelete(true);
}

void FileAnalysisTask::run() {
    // 1. 读取文件内容（使用智能截断）
    QString fileContent = readFileWithTruncation(m_filePath);

    // 2. 生成分析问题（带大小校验）
    QString question = generateQuestion(m_filePath, fileContent,false);

    // 记录问题大小
    m_logger->info(QString("生成分析问题，大小: %1 字符").arg(question.size()));

    // 3. 通过回调在主线程执行分析
    QString analysisResult = m_analysisCallback(question);

    // 4. 保存分析结果
    saveResult(analysisResult);

    // 5. 通知主线程结果更新
    QMutexLocker locker(m_mutex);
    m_results->append(qMakePair(m_filePath, analysisResult));
    m_condition->wakeAll();
}

QString FileAnalysisTask::readFileWithTruncation(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_logger->error(QString("无法打开文件: %1").arg(filePath));
        return "文件打开失败";
    }

    // 获取文件大小
    qint64 fileSize = file.size();
    const qint64 MAX_FILE_SIZE = 2 * 1024 * 1024; // 2MB限制

    if (fileSize > MAX_FILE_SIZE) {
        m_logger->warn(QString("文件 %1 过大 (%2 MB)，将进行截断处理")
                              .arg(filePath)
                              .arg(fileSize / (1024.0 * 1024.0), 0, 'f', 2));
    }

    QTextStream in(&file);
    QString content;

    // 分块读取，避免大文件导致内存问题
    const int CHUNK_SIZE = 4096;
    while (!in.atEnd()) {
        content += in.read(CHUNK_SIZE);

        // 如果内容超过限制，截断并添加提示
        if (content.size() > MAX_FILE_SIZE) {
            content = content.left(MAX_FILE_SIZE);
            content += "\n\n[文件内容过长，已截断后续部分...]";
            break;
        }
    }

    file.close();
    return content;
}

QString FileAnalysisTask::generateQuestion(const QString& filePath, const QString& content,bool isdataUp=false) {
    // 如果没有模板路径，使用默认生成方式
    if (m_jsonTemplatePath.isEmpty() || !QFile::exists(m_jsonTemplatePath)) {
        return generateDefaultQuestion(filePath, content);
    }

    // 尝试读取模板文件
    QFile templateFile(m_jsonTemplatePath);
    if (!templateFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_logger->warn(QString("无法打开模板文件: %1，将使用默认问题生成").arg(m_jsonTemplatePath));
        return generateDefaultQuestion(filePath, content);
    }

    // 读取模板内容
    QTextStream templateStream(&templateFile);
    QString templateContent = templateStream.readAll();
    templateFile.close();

    // 准备模板变量
    QJsonObject variables;
    variables["file_path"] = filePath;
    if(isdataUp){variables["file_content"] = "【过长省略，如有需要请查看对应文件路径】";}
    else variables["file_content"] = content;
    variables["file_size"] = QString::number(content.size());

    // 检测文件语言
    QString language = "Unknown";
    if (filePath.endsWith(".cpp") || filePath.endsWith(".h") || filePath.endsWith(".hpp")) {
        language = "C++";
    } else if (filePath.endsWith(".py")) {
        language = "Python";
    } else if (filePath.endsWith(".java")) {
        language = "Java";
    } else if (filePath.endsWith(".js") || filePath.endsWith(".ts")) {
        language = "JavaScript/TypeScript";
    } else if (filePath.endsWith(".cs")) {
        language = "C#";
    }
    variables["language"] = language;
    //规则
    // 解析模板中的变量占位符
    QString question = templateContent;
    for (auto it = variables.begin(); it != variables.end(); ++it) {
        QString placeholder = QString("{{%1}}").arg(it.key());
        question.replace(placeholder, it.value().toString());
    }
    int i=1;
    QString uRules;
    for(auto m :m_userRules){
        uRules+= QString("规则%1: %2 /n")
                      .arg(i)
                      .arg(m);

        i++;
    }
    variables["extra_rules"] = uRules;

    // 处理特殊占位符
    question.replace("{{detect_language(file_path)}}", language);

    // 校验并截断问题大小
    return truncateQuestionIfNeeded(question);
}

QString FileAnalysisTask::generateDefaultQuestion(const QString& filePath, const QString& content) {
    QString question;

    if (filePath.endsWith(".cpp") || filePath.endsWith(".h") || filePath.endsWith(".hpp")) {
        question = QString("请分析以下C++代码文件，描述其主要功能、关键类和方法，并指出可能的改进点:\n%1").arg(content);
    } else if (filePath.endsWith(".py")) {
        question = QString("请分析以下Python脚本，描述其主要功能、关键函数和算法流程:\n%1").arg(content);
    } else if (filePath.endsWith(".java")) {
        question = QString("请分析以下Java代码，描述其类结构、主要方法和设计模式:\n%1").arg(content);
    } else {
        question = QString("请分析以下文件内容:\n%1").arg(content);
    }

    // 校验并截断问题大小
    return truncateQuestionIfNeeded(question);
}

QString FileAnalysisTask::truncateQuestionIfNeeded(const QString& question) {
    const int MAX_QUESTION_SIZE = 15000; // 最大问题长度（字符数）
    const int WARNING_THRESHOLD = 10000; // 警告阈值

    if (question.size() > WARNING_THRESHOLD) {
        m_logger->warn(QString("生成的问题大小超过警告阈值: %1/%2 字符")
                              .arg(question.size())
                              .arg(WARNING_THRESHOLD));
    }

    if (question.size() > MAX_QUESTION_SIZE) {
        m_logger->error(QString("生成的问题过大 (%1 字符)，已截断至 %2 字符")
                            .arg(question.size())
                            .arg(MAX_QUESTION_SIZE));

        // 智能截断：保留开头和结尾部分
        int keepStart = MAX_QUESTION_SIZE * 0.7; // 保留70%的开头
        int keepEnd = MAX_QUESTION_SIZE * 0.3;   // 保留30%的结尾

        QString truncated = question.left(keepStart);
        truncated += "\n\n[内容过长，中间部分已截断...]\n\n";
        truncated += question.right(keepEnd);

        return truncated.left(MAX_QUESTION_SIZE);
    }

    return question;
}

void FileAnalysisTask::saveResult(const QString& analysisResult) {
    // 保存原始分析结果到文本文件
    QString resultTextFile = m_outputDir + "/" +"-"+ m_filePath+"-"+QFileInfo(m_filePath).baseName() + "_analysis.txt";
    saveToFile(resultTextFile, analysisResult);

    // 从分析结果中提取JSON部分
    QJsonObject jsonReport = extractJsonReport(analysisResult);

    // 保存JSON报告到单独文件
    QString resultJsonFile = m_outputDir + "/" +"-"+ m_filePath+"-"+ QFileInfo().baseName() + "_analysis.json";
    saveJsonToFile(resultJsonFile, jsonReport);

    // 准备数据库插入数据
    // QString functionalOverview = jsonReport.value("summary").toObject().value("functional_overview").toString();
    // QString architectureRole = jsonReport.value("summary").toObject().value("architecture_role").toString();

    // 构建改进建议字符串
    // QString improvementSuggestions;
    // QJsonArray suggestionsArray = jsonReport.value("summary").toObject().value("improvement_suggestions").toArray();
    // for (const auto& suggestion : suggestionsArray) {
    //     QJsonObject suggestionObj = suggestion.toObject();
    //     improvementSuggestions += QString("[%1] %2; ")
    //                                   .arg(suggestionObj["priority"].toString())
    //                                   .arg(suggestionObj["suggestion"].toString());
    // }

    // 保存到数据库
    // QString analysisResult和 QString question = generateQuestion(m_filePath, fileContent);
    QString question = generateQuestion(m_filePath, "",true);//去代码的问题
    QJsonArray array;

    // auto chatFrame = static_cast<ChatFrame *>(_mainLayout->itemAt(i)->widget());
    // QJsonObject chat{{"role", chatFrame->role()},
    //                  {"content", chatFrame->chatText()},
    //                  {"reasoning_content", chatFrame->reasonerText()},
    //                  {"total_tokens", chatFrame->tokenSize()},
    //                  {"datetime", chatFrame->dateTime()}};
    QJsonObject chatuser{{"role", 'user'},
                     {"content", question},
                     {"reasoning_content",""},
                     {"total_tokens", NULL},
                     {"datetime",QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")}};
    array.append(chatuser);
    QJsonObject chatassistant{{"role", "assistant"},
                         {"content", analysisResult},
                         {"reasoning_content",""},
                         {"total_tokens", NULL},
                         {"datetime",QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")}};
    array.append(chatassistant);
    QJsonDocument doc(array);
    QString content = doc.toJson(QJsonDocument::Compact);
    auto sql = QString("INSERT INTO messages (programID,  fileName, datetime,content ,resultPath)"
                       "VALUES ('%1', '%2', '%3', '%4')")
                   .arg(m_programID)
                   .arg(QFileInfo(m_filePath).path())
                   .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                   .arg(content)
                   .arg(resultJsonFile);
    m_sqlExecutor->executeNonQuery(sql);
}

QJsonObject FileAnalysisTask::extractJsonReport(const QString& analysisResult) {
    // 尝试从分析结果中提取JSON部分
    static QRegularExpression jsonRegex(R"(\n```json\s*(\{[\s\S]*?\})\s*```\n)");
    QRegularExpressionMatch match = jsonRegex.match(analysisResult);

    if (match.hasMatch()) {
        QString jsonString = match.captured(1);
        QJsonParseError parseError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8(), &parseError);

        if (parseError.error == QJsonParseError::NoError) {
            return jsonDoc.object();
        } else {
            m_logger->warn(QString("JSON解析错误: %1 (位置: %2)")
                                  .arg(parseError.errorString())
                                  .arg(parseError.offset));
        }
    }

    // 如果没有找到JSON块或解析失败，尝试解析整个内容
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(analysisResult.toUtf8(), &parseError);

    if (parseError.error == QJsonParseError::NoError) {
        return jsonDoc.object();
    }

    // 如果完全不是JSON，创建错误报告
    m_logger->error("分析结果中未找到有效的JSON报告");
    return createErrorReport(analysisResult);
}

void FileAnalysisTask::saveToFile(const QString& filePath, const QString& content) {
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << content;
        file.close();
    } else {
        m_logger->error(QString("无法保存文件: %1").arg(filePath));
    }
}

void FileAnalysisTask::saveJsonToFile(const QString& filePath, const QJsonObject& json) {
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << QJsonDocument(json).toJson(QJsonDocument::Indented);
        file.close();
    } else {
        m_logger->error(QString("无法保存JSON文件: %1").arg(filePath));
    }
}

QJsonObject FileAnalysisTask::createErrorReport(const QString& content) {
    // 创建包含错误信息的JSON报告
    return QJsonObject{
        {"file", QFileInfo(m_filePath).fileName()},
        {"language", "Unknown"},
        {"analysis_level", 0},
        {"error", "无法解析分析结果"},
        {"summary", QJsonObject{
                        {"functional_overview", "分析失败"},
                        {"architecture_role", "无法确定"},
                        {"critical_insights", QJsonArray{"分析结果格式不正确"} },
                        {"improvement_suggestions", QJsonArray{
                                                        QJsonObject{{"priority", "critical"}, {"suggestion", "检查分析引擎输出格式"}}
                                                    }},
                        {"knowledge_points", QJsonArray{}}
                    }},
        {"raw_output", content.left(5000)}  // 保存部分原始输出用于调试
    };
}
