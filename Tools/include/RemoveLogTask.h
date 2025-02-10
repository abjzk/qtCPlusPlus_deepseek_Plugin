#pragma once
#include <QThread>

class RemoveLogTask : public QThread
{
    Q_OBJECT
public:
    RemoveLogTask(int days,QObject *parent = nullptr);
    ~RemoveLogTask() = default;
    void run() override;
private:
    int _days;
};