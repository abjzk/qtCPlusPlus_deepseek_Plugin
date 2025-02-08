#pragma once
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include "Utility_global.h"

class UTILITY_EXPORT DeepSeek : public QObject
{
    Q_OBJECT
public:
    enum ErrorCode
    {
        NoError = 200,
        FormatError = 400,
        AuthError = 401,
        BalanceError = 402,
        ParameterError = 422,
        SpeedError = 429,
        ServerError = 500,
        ServerBusy = 503
    };
    struct Message
    {
        bool finish_reason; // 是否思考结束，只针对可以思考的模型
        QString role;       // 消息角色
        QString content;    // 消息内容
        int index;          // 消息索引
        Message(const QString &role, const QString &content) : role(role), content(content) {};
        Message(QJsonObject obj);
        QJsonObject toJson() const;
    };

    static QString errorCodeToString(const ErrorCode &code);
    DeepSeek(const QString &token, QObject *parent = nullptr);
    ~DeepSeek();
    bool stream() const { return _isStream; }

    QString model() const { return _model; }

    double frequencyPenalty() const { return _frequency_penalty; }

    int maxTokens() const { return _max_tokens; }

    double presencePenalty() const { return _presence_penalty; }

    QStringList stopStrings() const { return _stopStrings; }

    double temperature() const { return _temperature; }

    double topP() const { return _top_p; }

    QString systemMessage() const { return _system_messages; }

public slots:
    /**
     * @brief 发送种子消息
     * @param oldMessages 历史消息
     * @param message 新消息
     */
    void seedMessage(const QList<Message> &oldMessages, const QString &message);
    /**
     * @brief 作为调节采样温度的替代方案，模型会考虑前 top_p 概率的 token 的结果。所以 0.1 就意味着只有包括在最高 10% 概率中的 token 会被考虑。
     * 我们通常建议修改这个值或者更改 temperature，但不建议同时对两者进行修改。
     * @param top_p
     */
    void setTopP(double top_p = 1);
    /**
     * @brief 采样温度，介于 0 和 2 之间。更高的值，如 0.8，会使输出更随机，而更低的值，如 0.2，会使其更加集中和确定。
     * 我们通常建议可以更改这个值或者更改 top_p，但不建议同时对两者进行修改。
     * @param temperature
     */
    void setTemperature(double temperature = 1);
    /**
     * @brief 一个 string 或最多包含 16 个 string 的 list，在遇到这些词时，API 将停止生成更多的 token
     * @param stopStrings
     */
    void setStopStrings(const QStringList &stopStrings);
    /**
     * @brief介于 1 到 8192 间的整数，限制一次请求中模型生成 completion 的最大 token 数。输入 token 和输出 token 的总长度受模型的上下文长度的限制。
     * 如未指定 max_tokens参数，默认使用 4096。
     * @param max_tokens
     */
    void setMaxTokens(int max_tokens = 4096);
    /**
     * @brief 介于 -2.0 和 2.0 之间的数字。如果该值为正，那么新 token 会根据其在已有文本中的出现频率受到相应的惩罚，降低模型重复相同内容的可能性。
     * @param frequency_penalty
     */
    void setFrequencyPenalty(double frequency_penalty = 0);
    /**
     * @brief 设置模型
     * @param model 模型名称
     */
    void setModel(const QString &model) { _model = model; }
    /**
     * @brief 设置是否流式输出
     * @param isStream
     */
    void setStream(bool isStream) { _isStream = isStream; }
    /**
     * @brief 设置系统消息
     * @param message 消息内容
     */
    void setSystemMessage(const QString &message) { _system_messages = message; }
    /**
     * @brief 介于 -2.0 和 2.0 之间的数字。如果该值为正，那么新 token 会根据其是否已在已有文本中出现受到相应的惩罚，从而增加模型谈论新主题的可能性。
     * @param presence_penalty
     */
    void setPresencePenalty(double presence_penalty) { _presence_penalty = presence_penalty; }
signals:
    /**
     * @brief 流式消息信号
     * @param message 流式消息内容
     */
    void replyStreamMessage(const Message &message);
    /**
     * @brief 消息信号
     * @param message 消息内容
     */
    void replyMessage(const Message &message);
    /**
     * @brief 当前对话完成信号
     */
    void replyFinished();

private:
    QString _token{QString()};
    bool _isStream{true};
    QString _model{"deepseek-chat"};
    double _frequency_penalty{0};
    double _presence_penalty{0};
    int _max_tokens{4096};
    QStringList _stopStrings;
    double _temperature{1};
    double _top_p{1};
    QNetworkAccessManager _manager;
    QString _system_messages{"You are a helpful assistant"};
private slots:
    void readStream();
    void replyFinished_();
    void replyError(QNetworkReply::NetworkError error);

private:
    static QJsonObject messageToJson(const QString &role, const QString &content);
};