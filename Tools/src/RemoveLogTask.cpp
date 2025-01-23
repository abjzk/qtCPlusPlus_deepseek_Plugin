#include <RemoveLogTask.h>
#include <QApplication>
#include <QDir>
RemoveLogTask::RemoveLogTask(int days, QObject *parent)
    :QThread(parent),_days(days)
{
}

void RemoveLogTask::run()
{
    auto path  = QApplication::applicationDirPath() + "/logs/";
    auto deleteDateTime = QDateTime::currentDateTime().addDays(-_days);
    for(auto name : QDir(path).entryList(QDir::Dirs | QDir::NoDotAndDotDot))
        for(auto file : QDir(path + name).entryList(QDir::Files | QDir::NoDotAndDotDot))
            if(QFileInfo(path + name + "/" + file).lastModified() < deleteDateTime)
                QFile::remove(path + name + "/" + file);
}
