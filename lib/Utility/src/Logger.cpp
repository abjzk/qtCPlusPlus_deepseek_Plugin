#include "Logger.h"
#include <QApplication>
#include <LCore>
Logger::Logger(QString &name, QObject *parent)
    : QObject(parent),_name(name)
{
    setlocale(LC_ALL,"zhCn.utf8");
    tp = std::make_shared<spdlog::details::thread_pool>(102400, 1);
    QString filename = QString("%1/logs/%2/%2.log").arg(QApplication::applicationDirPath()).arg(_name);
    file_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(filename.toStdString(),0, 0);

    callback_sink = std::make_shared<spdlog::sinks::callback_sink_mt>([=](const spdlog::details::log_msg &msg) 
    {
        this->sendLogger(std::move(LoggerDetails(msg)));
    });
    _logger = std::make_shared<spdlog::async_logger>(_name.toStdString(), 
        spdlog::sinks_init_list({file_sink,callback_sink}),
        tp, 
        spdlog::async_overflow_policy::block);
    spdlog::flush_every(std::chrono::seconds(1));
}

Logger::~Logger()
{
}

void Logger::write(spdlog::level::level_enum level, const QString &msg)
{
    _logger->log(level,msg.toLocal8Bit().toStdString());
    _logger->flush();
}

void Logger::trace(const QString &msg)
{
    this->write(spdlog::level::trace,msg);
}

void Logger::debug(const QString &msg)
{
    this->write(spdlog::level::debug,msg);
}

void Logger::info(const QString &msg)
{
    this->write(spdlog::level::info,msg);
}

void Logger::warn(const QString &msg)
{
    this->write(spdlog::level::warn,msg);
}

void Logger::error(const QString &msg)
{
    this->write(spdlog::level::err,msg);
}

void Logger::critical(const QString &msg)
{
    this->write(spdlog::level::critical,msg);
}

QStringList Logger::levels()
{
    QStringList list = {"trace", "debug", "info", "warn", "error", "critical", "off"};
    return list;
}

QString Logger::levelToString(spdlog::level::level_enum level)
{
    switch (level)
    {
    case spdlog::level::trace:
        return "trace";
    
    case spdlog::level::debug:
        return "debug";
    
    case spdlog::level::info:
        return "info";
    
    case spdlog::level::warn:
        return "warn";
    
    case spdlog::level::err:
        return "error";
    
    case spdlog::level::critical:
        return "critical";
    
    case spdlog::level::off:
        return "off";
    
    case spdlog::level::n_levels:
        return "n_levels";
    default:
        return "n_levels";
    }
}

spdlog::level::level_enum Logger::stringToLevel(const QString &level)
{
    if (level == "trace")
        return spdlog::level::trace;
    if (level == "debug")
        return spdlog::level::debug;
    if (level == "info")
        return spdlog::level::info;
    if (level == "warn")
        return spdlog::level::warn;
    if (level == "error")
        return spdlog::level::err;
    if (level == "critical")
        return spdlog::level::critical;
    if (level == "off")
        return spdlog::level::off;
    return spdlog::level::n_levels;
}

void Logger::flush_on(QString name)
{
    auto level = stringToLevel(name);
    file_sink->set_level(level);
    callback_sink->set_level(level);
}

LoggerDetails::LoggerDetails(QDateTime date, QString level, QString message)
    : date(date), level(level), message(message)
{
}

LoggerDetails::LoggerDetails(const spdlog::details::log_msg &msg)
{

    date = LFunc::chrono_time_point_to_QDateTime(msg.time);
    level = Logger::levelToString(msg.level);
    message = QString::fromLocal8Bit(msg.payload.data(), msg.payload.size());
}

LoggerDetails::~LoggerDetails()
{
}
