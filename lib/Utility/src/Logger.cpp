#include "Logger.h"
#include <QApplication>

Logger::Logger(QString &name, QObject *parent)
    : QObject(parent),_name(name)
{
    setlocale(LC_ALL,"zhCn.utf8");
    tp = std::make_shared<spdlog::details::thread_pool>(102400, 1);
    QString filename = QString("%1/logs/%2/%2").arg(QApplication::applicationDirPath()).arg(_name);
    file_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(filename.toStdString(),0, 0);

    callback_sink = std::make_shared<spdlog::sinks::callback_sink_mt>([=](const spdlog::details::log_msg &msg) 
    {
            // 将 time_point 转换为 time_t
        std::time_t time = std::chrono::system_clock::to_time_t(msg.time);

        // 将 time_t 转换为 tm 结构
        std::tm tm = *std::localtime(&time);
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(msg.time.time_since_epoch()) % 1000;
        // 使用 stringstream 格式化日期时间字符串
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S ");
        oss << '.' << std::setfill('0') << std::setw(3) << millis.count(); // 添加毫秒部分
        QString timeStr = QString::fromStdString(oss.str());
        QString type = Logger::levelToString(msg.level);
        QString message = QString::fromLocal8Bit(msg.payload.data(), static_cast<int>(msg.payload.size()));
        this->sendLogger({
            {"time",timeStr},
            {"level",type},
            {"message",message}
        });
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
    else if (level == "debug")
        return spdlog::level::debug;
    else if (level == "info")
        return spdlog::level::info;
    else if (level == "warn")
        return spdlog::level::warn;
    else if (level == "error")
        return spdlog::level::err;
    else if (level == "critical")
        return spdlog::level::critical;
    else if (level == "off")
        return spdlog::level::off;
    else
        return spdlog::level::n_levels;
}

void Logger::flush_on(QString &name)
{
    auto level = stringToLevel(name);
    file_sink->set_level(level);
    callback_sink->set_level(level);
}
