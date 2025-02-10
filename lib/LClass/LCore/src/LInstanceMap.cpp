#include "LInstanceMap.h"
#include <QThread>
using namespace jzk;

jzk::Content::Content(QString key, const QVariant &value, const QVariant &oldValue, Type type, const QDateTime& time)
    : _key(std::move(key)), _value(value), _oldValue(oldValue), _type(type), _time(QDateTime::currentDateTime())
{

}

jzk::Content::~Content()
= default;

QString jzk::Content::toString() const
{
    return QString(
        R"(
            {
                "key":"%1",
                "value":"%2",
                "oldValue":"%3",
                "type":"%4",
                "time":"%5"
            }
        )"
    ).arg(_key).arg(_value.toString()).arg(_oldValue.toString()).arg(_type).arg(_time.toString("yyyy-MM-dd hh:mm:ss"));
}

jzk::LInstanceMap& jzk::LInstanceMap::instance()
{
    static LInstanceMap _instance; // 单例
    return _instance;
}
void jzk::LInstanceMap::set(const QString &key, const QVariant &value, bool isRegister)
{
    _lock.lockForWrite();
    bool keyExist = this->contains(key);

    Content content(key, value, _map.value(key), keyExist ? Content::Type::Update : Content::Type::Insert);
    content._threadId = static_cast<int>(reinterpret_cast<uintptr_t>(QThread::currentThreadId()));
    _map.insert(key, value);

    if (isRegistered(key) || isRegister)
    {
        this->valueChange(content);
    }
    _lock.unlock();
    if (isRegister)
    {
        this->registerKey(QStringList() << key);
    }
}
void jzk::LInstanceMap::set(const QMap<QString, QVariant> &map, bool isRegister)
{
    for (auto it = map.begin(); it != map.end(); it++)
    {
        this->set(it.key(), it.value(), isRegister);
    }
}
QVariant jzk::LInstanceMap::get(const QString &key)
{
    _lock.lockForRead();
    QVariant value = _map.value(key);
    _lock.unlock();
    return value;
}
QVariantList jzk::LInstanceMap::get(const QStringList &keys)
{
    QVariantList list;
    for (const auto & key : keys)
    {
        list.append(this->get(key));
    }
    return list;
}
jzk::LInstanceMap::LInstanceMap()
    :QObject(nullptr)
{
}

jzk::LInstanceMap::~LInstanceMap()
= default;

void jzk::LInstanceMap::remove(const QString &key, bool isUnRegister)
{
    _lock.lockForWrite();
    if (!this->contains(key))
    {
        _lock.unlock();
        return;
    }
    _map.remove(key);
    if (isRegistered(key))
    {
        Content content(key, QVariant(), _map.value(key), Content::Type::Delete);
        content._threadId = static_cast<int>(reinterpret_cast<uintptr_t>(QThread::currentThreadId()));
        this->valueChange(content);
    }
    _lock.unlock();
 
    if (isUnRegister)
    {
        this->unRegisterKey(QStringList() << key);
    }
}

void jzk::LInstanceMap::remove(const QStringList &keys, bool isUnRegister)
{
    for (const auto & key : keys)
    {
        this->remove(key, isUnRegister);
    }
}

void jzk::LInstanceMap::clear()
{
    for(auto it = _map.begin(); it != _map.end(); it++)
    {
        this->remove(it.key());
    }
}

int jzk::LInstanceMap::count()
{
    QMutexLocker locker(&_mutex);
    return _map.count();
}

QStringList jzk::LInstanceMap::keys()
{
    QMutexLocker locker(&_mutex);
    return _map.keys();
}

bool jzk::LInstanceMap::contains(const QString &key)
{
    QMutexLocker locker(&_mutex);
    return _map.contains(key);
}

bool jzk::LInstanceMap::isEmpty()
{
    QMutexLocker locker(&_mutex);
    return _map.isEmpty();
}

QMap<QString, QVariant> jzk::LInstanceMap::map()
{
    return _map;
}

void jzk::LInstanceMap::registerkey(const QString key)
{
    QMutexLocker locker(&_mutex);
    _keySet.insert(key);
}

void jzk::LInstanceMap::registerKey(const QStringList &keys)
{
    this->registerKey(QSet<QString>(keys.begin(), keys.end()));
}

void jzk::LInstanceMap::registerKey(const QSet<QString> &keys)
{
    QMutexLocker locker(&_mutex);
    _keySet.unite(keys);
}

void jzk::LInstanceMap::unRegisterKey(const QString &key)
{
    QMutexLocker locker(&_mutex);
    _keySet.remove(key);
}

void jzk::LInstanceMap::unRegisterKey(const QStringList &keys)
{
    this->unRegisterKey(QSet<QString>(keys.begin(), keys.end()));
}

void jzk::LInstanceMap::unRegisterKey(const QSet<QString> &keys)
{
    QMutexLocker locker(&_mutex);
    _keySet.subtract(keys);
}

QStringList jzk::LInstanceMap::registeredKeys()
{
    QMutexLocker locker(&_mutex);
    return {_keySet.begin(), _keySet.end()};
}

bool jzk::LInstanceMap::isRegistered(const QString &key)
{
    QMutexLocker locker(&_mutex);
    return _keySet.contains(key);
}

void jzk::LInstanceMap::valueChange(Content content)
{
    if (content._time < time)
        qDebug() << "error";

	time = content._time;
    emit valueChanged(content);
}
