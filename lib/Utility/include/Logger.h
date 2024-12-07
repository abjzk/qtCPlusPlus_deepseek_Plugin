#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/async.h>
#include <QObject>
#include <QMap>
#include "Utility_global.h"
#include "spdlog/sinks/callback_sink.h"
#include <QDateTime>
struct UTILITY_EXPORT LoggerDetails
{
    LoggerDetails(QDateTime date, QString level, QString message);
    LoggerDetails(const spdlog::details::log_msg& msg);
    ~LoggerDetails();
    QDateTime date;
    QString level;
    QString message;
};



class UTILITY_EXPORT Logger : public QObject
{
    Q_OBJECT
public:
    explicit Logger(QString& name, QObject *parent = nullptr);
    ~Logger();
    void write(spdlog::level::level_enum level, const QString& msg);
    void trace(const QString& msg);
    void debug(const QString& msg);
    void info(const QString& msg);
    void warn(const QString& msg);
    void error(const QString& msg);
    void critical(const QString& msg);
    static QStringList levels();
    static QString levelToString(spdlog::level::level_enum level);
    static spdlog::level::level_enum stringToLevel(const QString& level);
    void flush_on(QString name);
signals:
    void sendLogger(LoggerDetails details);
private:
    std::shared_ptr<spdlog::logger> _logger;
    QString _name;
    spdlog::level::level_enum _flushLevel;
    std::shared_ptr<spdlog::sinks::daily_file_sink_mt> file_sink;
    std::shared_ptr<spdlog::sinks::callback_sink_mt> callback_sink;
    std::shared_ptr<spdlog::details::thread_pool> tp;
};