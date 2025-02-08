#include "DeepSeek.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#define SYSTEMROLE "system"
#define USERROLE "user"
#define ASSISTANTROLE "assistant"

QString DeepSeek::errorCodeToString(const ErrorCode &code)
{
    switch (code)
    {
    case NoError:
        return "无错误";
    case FormatError:
        return "格式错误";
    case AuthError:
        return "认证失败";
    case BalanceError:
        return "余额不足";
    case ParameterError:
        return "参数错误";
    case SpeedError:
        return "请求速度超过限制";
    case ServerError:
        return "服务器错误";
    case ServerBusy:
        return "服务器繁忙";
    default:
        return "未知错误";
    }
}

DeepSeek::DeepSeek(const QString &token, QObject *parent)
    : QObject(parent), _token(token)
{
}

DeepSeek::~DeepSeek()
{
}

void DeepSeek::setFrequencyPenalty(double frequency_penalty)
{
    if (frequency_penalty < -2.0 || frequency_penalty > 2.0)
        return;
    _frequency_penalty = frequency_penalty;
}
void DeepSeek::readStream()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply->error() != QNetworkReply::NoError)
        return;
    QString text = reply->readAll();
    QStringList textList = text.split("data: ");
    for (auto t : textList)
    {
        QJsonDocument doc = QJsonDocument::fromJson(t.toUtf8());
        if (doc.isNull() || doc.isEmpty())
            continue;
        auto message = Message(doc.object());
        emit replyStreamMessage(message);
    }
}
void DeepSeek::replyFinished_()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply->error() != QNetworkReply::NoError)
        return;
    QString text = reply->readAll();

    if (!_isStream)
    {
        QJsonDocument doc = QJsonDocument::fromJson(text.toUtf8());
        if (doc.isNull() || doc.isEmpty())
            return;
        emit replyMessage(Message(doc.object()));
    }
    reply->deleteLater();
    emit replyFinished();
}

void DeepSeek::replyError(QNetworkReply::NetworkError error)
{
}

QJsonObject DeepSeek::messageToJson(const QString &role, const QString &content)
{
    return QJsonObject{{"role", role}, {"content", content}};
}

void DeepSeek::setMaxTokens(int max_tokens)
{
    if (max_tokens < 1 || max_tokens > 8192)
        return;
    _max_tokens = max_tokens;
}

void DeepSeek::setStopStrings(const QStringList &stopStrings)
{
    if (stopStrings.size() > 16)
        return;
    _stopStrings = stopStrings;
}

void DeepSeek::setTemperature(double temperature)
{
    if (temperature < 0.0 || temperature > 2.0)
        return;
    _temperature = temperature;
}

void DeepSeek::setTopP(double top_p)
{
    if (top_p < 0.0 || top_p > 1.0)
        return;
    _top_p = top_p;
}

void DeepSeek::seedMessage(const QList<Message> &oldMessages, const QString &message)
{
    QNetworkRequest request;
    request.setUrl(QUrl("https://api.deepseek.com/chat/completions"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Accept", "application/json");
    request.setRawHeader("Authorization", ("Bearer " + _token).toUtf8());
    QJsonObject obj;
    obj.insert("model", _model);
    obj.insert("frequency_penalty", _frequency_penalty);
    obj.insert("max_tokens", _max_tokens);
    obj.insert("presence_penalty", _presence_penalty);
    if (!_stopStrings.isEmpty())
        obj.insert("stop", QJsonArray::fromStringList(_stopStrings));
    else
        obj.insert("stop", QJsonValue::Null);
    obj.insert("temperature", _temperature);
    obj.insert("top_p", _top_p);
    obj.insert("stream", _isStream);
    obj.insert("response_format", QJsonObject{{"type", "text"}});
    obj.insert("stream_options", QJsonValue::Null);
    obj.insert("tools", QJsonValue::Null);
    obj.insert("tool_choice", "none");
    obj.insert("logprobs", false);
    obj.insert("top_logprobs", QJsonValue::Null);
    QJsonArray messages;
    messages.append(messageToJson(SYSTEMROLE, _system_messages));
    for (auto &oldMessage : oldMessages)
        messages.append(oldMessage.toJson());
    messages.append(messageToJson(USERROLE, message));
    obj.insert("messages", messages);
    QString json = QJsonDocument(obj).toJson(QJsonDocument::Compact);
    QNetworkReply *_reply = _manager.post(request, QJsonDocument(obj).toJson());
    if (_isStream)
    {
        connect(_reply, &QNetworkReply::readyRead, this, &DeepSeek::readStream);
    }
    connect(_reply, &QNetworkReply::finished, this, &DeepSeek::replyFinished_);
    connect(_reply, &QNetworkReply::errorOccurred, this, &DeepSeek::replyError);
}

DeepSeek::Message::Message(QJsonObject obj)
{
    auto choices = obj.value("choices").toArray();
    auto choice = choices.at(0).toObject();
    finish_reason = choice.value("finish_reason").toString() == "stop";
    index = choice.value("index").toInt();
    if (choice.contains("message"))
    {
        role = choice.value("message").toObject().value("role").toString();
        content = choice.value("message").toObject().value("content").toString();
    }
    else
    {
        role = ASSISTANTROLE;
        content = choice.value("delta").toObject().value("content").toString();
    }
}

QJsonObject DeepSeek::Message::toJson() const
{
    return QJsonObject{{"role", role}, {"content", content}};
}
