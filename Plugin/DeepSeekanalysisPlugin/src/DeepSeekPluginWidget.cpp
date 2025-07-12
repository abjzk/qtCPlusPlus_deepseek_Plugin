#include "DeepSeekPluginWidget.h"


DeepSeekWidget::DeepSeekWidget(Logger *logger, TConfig *config, QWidget *parent)
    : QWidget(parent), ui(new Ui::DeepSeekPluginWidget()), _config(config), _logger(logger)
{
    //AnalysisMessage表
    //索引，项目id，identifier号，创立时间，聊天项名，总结的内容，合法？
    _sqlExecutor->executeNonQuery(R"(CREATE TABLE IF NOT EXISTS AnalysisMessage
        (id INTEGER PRIMARY KEY AUTOINCREMENT,identifier TEXT, datetime TEXT, chat_name TEXT, content TEXT, isLegal INTEGER);)");
    // file_analysis (表)
    //     索引，项目id，项目根目录，项目子文件名，messages (表)索引，创立时间，弹窗返回的各种模设定提问和模型参数json，涉及变量的路径，分析json结果路径,
    _sqlExecutor->executeNonQuery(R"(CREATE TABLE IF NOT EXISTS file_analysis
        (id INTEGER PRIMARY KEY AUTOINCREMENT,programID TEXT,rootPath TEXT, filesName TEXT, chat_name TEXT,datetime TEXT, params TEXT,paramPath TEXT,jsonResultPath TEXT, content TEXT);)");
    //     messages (表)
    //     索引，对应的项目id，文件名，创立时间，对话内容（尽量采用变量替换代码,减少内存消耗）,结果路径
    _sqlExecutor->executeNonQuery(R"(CREATE TABLE IF NOT EXISTS messages
        (id INTEGER PRIMARY KEY AUTOINCREMENT,programID TEXT, fileName TEXT, datetime TEXT,content TEXT,resultPath TEXT);)");
    ui->setupUi(this);
    deepSeek = new DeepSeek(_config->read("token").valueString(), this);
    setconfig(deepSeek);
    _seed_key=_config->read("seed_key").value.value<ComboxData>().currentText();
    this->initUi();
    this->initConnect();
    this->loadChat();
}
void DeepSeekWidget::setconfig(DeepSeek* deepSeek1){
    deepSeek1->setSystemMessage(_config->read("system_messages").valueString());
    deepSeek1->setMaxTokens(_config->read("max_tokens").value.toInt());
    deepSeek1->setTemperature(_config->read("temperature").value.toDouble());
    deepSeek1->setTopP(_config->read("top_p").value.toDouble());
    deepSeek1->setPresencePenalty(_config->read("presence_penalty").value.toDouble());
    deepSeek1->setFrequencyPenalty(_config->read("frequency_penalty").value.toDouble());
    deepSeek1->setModel(_config->read("model").value.value<ComboxData>().currentText());
    deepSeek1->setStream(_config->read("isStream").value.toBool());
}
DeepSeekWidget::~DeepSeekWidget()
{
    delete ui;
    delete _sqlExecutor;
}
/**
 * @brief 参数更新在这里
 */
void DeepSeekWidget::setParmas(QString key, QVariant value)

{
    qDebug()<<"DeepSeekWidget::setParmas";


    if (key == "system_messages")
    {
        deepSeek->setSystemMessage(value.toString());

    }
    else if (key == "max_tokens")
    {
        deepSeek->setMaxTokens(value.toInt());

    }
    else if (key == "temperature")
    {
        deepSeek->setTemperature(value.toDouble());

    }
    else if (key == "top_p")
    {
        deepSeek->setTopP(value.toDouble());

    }
    else if (key == "presence_penalty")
    {
        deepSeek->setPresencePenalty(value.toDouble());

    }
    else if (key == "frequency_penalty")
    {
        deepSeek->setFrequencyPenalty(value.toDouble());

    }
    else if (key == "model")
    {
        deepSeek->setModel(value.value<ComboxData>().currentText());
        // auto sql = QString("UPDATE file_analysis SET '%1' = '%2' isLegal = 1 WHERE identifier = '%3'").arg(key)
        //                .arg(value.value<ComboxData>().currentText())
        //                .arg(_identifier);
        // _sqlExecutor->executeNonQuery(sql);
    }
    else if (key == "isStream")
    {
        deepSeek->setStream(value.toBool());
    }
    else if (key == "token")
    {
        deepSeek->setToken(value.toString());
    }
    else if(key == "seed_key")
    {
        _seed_key=value.toString();
        ui->textEdit->setPlaceholderText(QString("请输入你的问题后按下%1来进行提问!").arg(_seed_key));

    }


}


/**
 * @brief 初始化UI界面最好在这里完成
 */
void DeepSeekWidget::initUi()
{
#pragma region 初始化分割器的尺寸
    ui->hSplitter->setStretchFactor(0, 1);
    ui->hSplitter->setStretchFactor(1, 5);
    ui->vSplitter->setSizes(QList<int>() << 700 << 200<<40);
    ui->vSplitter->setStretchFactor(0, 5);
    ui->vSplitter->setStretchFactor(1, 1);
    ui->vSplitter->setStretchFactor(2, 1);
#pragma endregion 初始化分割器的尺寸
    _spacer = new QWidget();
    _spacer->setFixedHeight(300);
    // 聊天区
    _mainLayout->addWidget(_spacer);
    ui->chatScrollArea->setWidget(_mainWidget);
    ui->chatScrollArea->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->chatListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    // 多选
    ui->chatListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->textEdit->setPlaceholderText(QString("请输入你的问题后按下%1来进行提问!").arg(_seed_key));


}
/**
 * @brief 初始化各种信号和槽最好在这里完成
 */
void DeepSeekWidget::initConnect()
{
    connect(deepSeek, &DeepSeek::replyBalance, this, &DeepSeekWidget::moneyChange);//jzk
    connect(deepSeek, &DeepSeek::replyStreamMessage, this, &DeepSeekWidget::addLastMessage);
    connect(deepSeek, &DeepSeek::replyMessage, this, &DeepSeekWidget::addLastMessage);

    connect(deepSeek, &DeepSeek::replyFinished, this, &DeepSeekWidget::finished);
    connect(ui->chatScrollArea, &QScrollArea::customContextMenuRequested, this, &DeepSeekWidget::showContextMenu);
    connect(ui->newChatButton, &QPushButton::clicked, this, &DeepSeekWidget::newChat);
    connect(ui->analysisButton, &QPushButton::clicked, this, &DeepSeekWidget::on_pushButton_clicked);
    connect(ui->chatListWidget, &QListWidget::itemClicked, this, &DeepSeekWidget::loadChatMessage);
    connect(ui->chatListWidget, &QListWidget::customContextMenuRequested, this, &DeepSeekWidget::showListWidgetContextMenu);
    // connect(this,&DeepSeekWidget::Finished(),this, &DeepSeekWidget::changeFish);
}

/**
 * @brief 余额更新//jzk
 */
void DeepSeekWidget::moneyChange(const DeepSeek::Balance &balance){
    //double转字符串
    ui->moneyShow->setText( QString("余额: %1").arg(balance.total_balance));
    qDebug() << QString("余额: %1").arg(balance.total_balance);
}
/**
 * @brief 事件Ctrl + Enter ，发送消息
 */
void DeepSeekWidget::keyPressEvent(QKeyEvent *event)

{   qDebug() << QString("keyPressEvent");
    isanalysis=false;
    // Ctrl + Enter 或者Ctrl
    if (event->key() == Qt::Key_Return && (event->modifiers() == Qt::ControlModifier || _seed_key==QString("Enter")))
    {
        if (deepSeek->isRequesting())
            return;
        auto text = ui->textEdit->toPlainText();

        // 生成项目总结报告刚生成判断
        qDebug()<< QString("keyPressEvent")<<_identifier<<_name;
        //项目分析项展示不一样
        if(_name.startsWith("项目分析: ") && _name.endsWith(_identifier)){
            auto chatFrame = static_cast<ChatFrame *>(_mainLayout->itemAt(_mainLayout->count() - 2)->widget());
            QString content = chatFrame->chatText();
            if(content.startsWith("正在生成项目总结报告"))
            text="以上是对于项目的各个文件的分析以及最后的总结。\n\n下面是新的提问：\n\n"+text;
            //创建新数据
            qDebug() << QString("AnalysisMessage插入新项目分析数据");
            //更新对话到数据库,读取config表里的模型参数，更新到表中*****
            auto sql1 = QString("INSERT INTO AnalysisMessage (identifier, datetime, chat_name, isLegal) VALUES ('%1', '%2', '%3', 1)")
                            .arg(_identifier)
                            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                            .arg(_name);
            _sqlExecutor->executeNonQuery(sql1);
        }
        //对话-问
        ChatFrame *chatFrame = new ChatFrame(Role::User, this);
        chatFrame->setChatText(text);
        // 在右界面的继续添加提问
        _mainLayout->insertWidget(_mainLayout->count() - 1, chatFrame);
        // scrollArea 滚动到_spacer上面，也就要要最大值 - _spacer的高度
        // ui->chatScrollArea->verticalScrollBar()->setValue(ui->chatScrollArea->verticalScrollBar()->maximum() - spacer->height());
        // 在右界面的继续添加回答
        ChatFrame *reasonerFrame = new ChatFrame(Role::Assistant, this);
        _mainLayout->insertWidget(_mainLayout->count() - 1, reasonerFrame);
        _logger->info(QString("seed: %1").arg(text));
        //deepseek库更新消息
        deepSeek->seedMessage(oldMessage(), text);

        // deepseek库更新余额//jzk
        deepSeek->queryBalance();

        //延迟执行了一个 lambda 函数,确保完成回复内容部件在对话区可见// 滚动到AI回复框
        QTimer::singleShot(500, this, [=]
                           { ui->chatScrollArea->ensureWidgetVisible(reasonerFrame); });
        //回复转圈动画
        reasonerFrame->startLoading();
        //单次对话完成
        ui->textEdit->clear();
        ui->textEdit->setEnabled(false);
        if (_mainLayout->count() == 3)//发送信息发现在新对话中
        {
            qDebug() << QString("插入新数据");
            _identifier = QUuid::createUuid().toString();
            _name = text.size() > 10 ? text.left(10) + "..." : text;
            //更新对话到数据库,读取config表里的模型参数，更新到表中*****
            auto sql1 = QString("INSERT INTO AnalysisMessage (identifier,inputPath,outputPath, datetime, chat_name, isLegal) VALUES ('%1', '%2', '%3','%4', '%5', 0)")
                           .arg(_identifier)
                           .arg(_inputPath)
                           .arg(_outputPath)
                           .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                           .arg(_name);
            _sqlExecutor->executeNonQuery(sql1);
            _inputPath="";
            _outputPath="";
        }
    }
}
//自动触发，在回复过程中几乎实时划分推理和回复
void DeepSeekWidget::addLastMessage(const DeepSeek::Message &message)
{
    // qDebug() << QString("addLastMessage");
    auto widget = _mainLayout->itemAt(_mainLayout->count() - 2)->widget();//已存在的回复框
    if (widget)
    {
        ChatFrame *chatFrame = static_cast<ChatFrame *>(widget);
        if (!message.content.isEmpty())
        {
            chatFrame->addChatText(message.content);
            _logger->info(QString("content: %1").arg(message.content));
        }
        if (!message.reasoning_content.isEmpty())
        {
            chatFrame->addReasonerText(message.reasoning_content);
            _logger->info(QString("reasoning_content: %1").arg(message.reasoning_content));
        }
        ui->chatScrollArea->ensureWidgetVisible(widget);
    }
    // ui->chatScrollArea->verticalScrollBar()->setValue(ui->chatScrollArea->verticalScrollBar()->maximum());
}
//单次对话完成，更新数据库对话内容并看情况在右侧插入新对话项
void DeepSeekWidget::finished(QNetworkReply::NetworkError error, int httpStatusCode, const QString &errorString)
{

    qDebug() << QString("finished");
    ui->textEdit->setEnabled(true);
    ui->textEdit->setFocus();
    // 获取布局中倒数第二个组件（通常是最后一个聊天框）
    // 将其转换为 ChatFrame 类型（自定义聊天框控件）
    // 设置消耗的 token 数量（从 DeepSeek 响应中获取）
    // 设置错误文本（如果有错误）
    // 停止加载动画（如旋转图标）
    auto widget = _mainLayout->itemAt(_mainLayout->count() - 2)->widget();//最后一个聊天框
    if (widget)
    {
        ChatFrame *chatFrame = static_cast<ChatFrame *>(widget);
        chatFrame->setTokenSize(deepSeek->lastUsage().total_tokens);
        chatFrame->setErrorText(errorString);
        chatFrame->stopLoading();
    }
    QJsonArray array;
    if(!isanalysis){
        for (int i = 0; i < _mainLayout->count() - 1; ++i)//遍历布局中的所有聊天框（排除最后一个占位区）
        {
            auto chatFrame = static_cast<ChatFrame *>(_mainLayout->itemAt(i)->widget());
            if (chatFrame)
            {

                QJsonObject chat{{"role", chatFrame->role()},
                                 {"content", chatFrame->chatText()},
                                 {"reasoning_content", chatFrame->reasonerText()},
                                 {"total_tokens", chatFrame->tokenSize()},
                                 {"datetime", chatFrame->dateTime()}};
                array.append(chat);
            }
        }

        QJsonDocument doc(array);
        QString content = doc.toJson(QJsonDocument::Compact);
        //确定两路径才创建对话列表
        // auto sql_path = QString("SELECT inputPath,outputPath FROM AnalysisMessage WHERE identifier = '%1'").arg(_identifier);
        // QList<QVariantMap> paths = _sqlExecutor->executeQuery(sql_path);//identifier唯一
        // while(paths.at(0)["inputPath"].toString()==""||paths.at(0)["inputPath"].toString()==""){};
        // while((_inputPath==""||_outputPath=="")){};
        // 使用参数化查询（安全且高效）
        QMap<QString, QVariant> params;
        params[":content"] = content;       // JSON内容
        params[":identifier"] = _identifier; // 唯一标识符

        try {
            int affectedRows = _sqlExecutor->executeNonQuerywithparams(
                "UPDATE AnalysisMessage SET content = :content, isLegal = 1 "
                "WHERE identifier = :identifier",
                params
                );
            qDebug() << "成功更新" << affectedRows << "条记录";
        } catch (const std::runtime_error &e) {
            qCritical() << "数据库更新失败:" << e.what();
            // 这里可以添加恢复逻辑或用户提示
        }

    }else if(!_issummary){
        //文件分析时的聊天区操作
        // auto chatFrame = static_cast<ChatFrame *>(_mainLayout->itemAt(_mainLayout->count() - 3)->widget());
        // if (chatFrame)
        // {

        //     QJsonObject chat{{"role", chatFrame->role()},
        //                      {"content", chatFrame->chatText()},
        //                      {"reasoning_content", chatFrame->reasonerText()},
        //                      {"total_tokens", chatFrame->tokenSize()},
        //                      {"datetime", chatFrame->dateTime()}};
        //     array.append(chat);
        // }
        // //布局中的最后一聊天框（提问已经放入）
        // auto chatFramereply = static_cast<ChatFrame *>(_mainLayout->itemAt(_mainLayout->count() - 2)->widget());
        // if (chatFramereply &&chatFramereply->role()=="assistant")
        // {

        //     QJsonObject chat{{"role", chatFramereply->role()},
        //                      {"content", chatFramereply->chatText()},
        //                      {"reasoning_content", chatFramereply->reasonerText()},
        //                      {"total_tokens", chatFramereply->tokenSize()},
        //                      {"datetime", chatFramereply->dateTime()}};
        //     array.append(chat);
        // }else{
        //     qDebug()<<"回复聊天chatFrame读取失败";
        // }
        // 在分析类保存结果时进行了数据库行创建

        // QJsonDocument doc(array);
        // QString content = doc.toJson(QJsonDocument::Compact);
        // // 使用参数化查询（安全且高效）
        // QMap<QString, QVariant> params;
        // params[":content"] = content;       // JSON内容
        // params[":programID"] = _identifier; // 程序ID
        // params[":fileName"] = _fileName;    // 文件名

        // try {
        //     int affectedRows = _sqlExecutor->executeNonQuerywithparams(
        //         "UPDATE messages SET content = :content, isLegal = 1 "
        //         "WHERE programID = :programID AND fileName = :fileName",
        //         params
        //         );
        //     qDebug() << "成功更新" << affectedRows << "条记录";
        // } catch (const std::exception &e) {
        //     qCritical() << "数据库更新失败:" << e.what();
        // }
    }else{

        // 处理分析模式下的结果
        // //布局中的最后一聊天框（提问已经放入）

            auto widget = _mainLayout->itemAt(_mainLayout->count() - 2)->widget();
            if (widget) {
                ChatFrame *chatFrame = static_cast<ChatFrame *>(widget);
                QString summaryContent = chatFrame->chatText();

                // 保存总结报告到文件
                QString summaryPath = _outputPath + "/project_summary.md";
                QFile summaryFile(summaryPath);
                if (summaryFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
                    QTextStream out(&summaryFile);
                    out << summaryContent;
                    summaryFile.close();
                }
                // auto chatFramereply = static_cast<ChatFrame *>(_mainLayout->itemAt(_mainLayout->count() - 2)->widget());
                if (chatFrame &&chatFrame->role()=="assistant")
                {

                        QJsonObject chat{{"role", chatFrame->role()},
                                         {"content", chatFrame->chatText()},
                                         {"reasoning_content", chatFrame->reasonerText()},
                                         {"total_tokens", chatFrame->tokenSize()},
                                         {"datetime", chatFrame->dateTime()}};
                        array.append(chat);
                    }else{
                        qDebug()<<"回复聊天chatFrame读取失败";
                    }
                // 更新数据库中的总结路径
                try {
                    QMap<QString, QVariant> params;
                    params[":jsonResultPath"] = summaryPath;
                    params[":programID"] = _identifier;
                    QJsonDocument doc(array);
                    QString content = doc.toJson(QJsonDocument::Compact);
                    params[":content"] = content;

                    _sqlExecutor->executeNonQuerywithparams(
                        "UPDATE file_analysis SET jsonResultPath = :jsonResultPath,content = :content  "
                        "WHERE programID = :programID",
                        params
                        );
                } catch (const std::exception& e) {
                    _logger->error(QString("更新总结路径失败: %1").arg(e.what()));
                }
            }

    }

    if (_mainLayout->count() == 3)//检查布局组件数量是否为 3，ChatFrame (用户消息1)ChatFrame (AI回复1)_spacer (空白占位)

    {
        qDebug() << QString("newChat_mainLayout");
        // 在listWidget中第一行插入一行
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(_name);
        item->setData(Qt::UserRole, _identifier);
        ui->chatListWidget->insertItem(0, item);
    }
    _logger->info("finished");
    _logger->info(QString("total_tokens: %1").arg(deepSeek->lastUsage().total_tokens));
    qDebug() << "analyzeFileOnMainThread finish" << _fileName;
    iscontinue.wakeOne();
}
//右边对话区之前所有其他聊天内容（role()，chatText()）
QList<DeepSeek::Message> DeepSeekWidget::oldMessage(bool signal)
{

    auto ms = QList<DeepSeek::Message>();
    if(signal)return ms;
    for (int i = 0; i < _mainLayout->count() - 1; ++i)
    {
        auto chatFrame = static_cast<ChatFrame *>(_mainLayout->itemAt(i)->widget());
        if (chatFrame)
        {
            ms.append(DeepSeek::Message(chatFrame->role(), chatFrame->chatText()));
        }
    }
    return ms;
}

void DeepSeekWidget::newChat()
{
    qDebug()<<"newChat";

    // 判断当前的对话是否合法
    auto sql = QString("SELECT isLegal FROM AnalysisMessage WHERE identifier = '%1'").arg(_identifier);
    int isLegal = _sqlExecutor->executeScalar<int>(sql);
    if (isLegal == 0)
    {
        sql = QString("Delete FROM AnalysisMessage WHERE identifier = '%1'").arg(_identifier);
        _sqlExecutor->executeNonQuery(sql);
    }
    //清空对话区
    while (_mainLayout->count() > 1)
    {
        auto chatFrame = static_cast<ChatFrame *>(_mainLayout->itemAt(0)->widget());
        if (chatFrame)
        {
            _mainLayout->removeWidget(chatFrame);
            delete chatFrame;
        }
    }
}

void DeepSeekWidget::loadChat()
{
    ui->chatListWidget->clear();
    qDebug()<<"loadChat";
    // deepseek库更新余额//jzk
    deepSeek->queryBalance();
    //加载所用合法对话放到左边对话项列表
    auto sql = QString("SELECT chat_name,identifier FROM AnalysisMessage where isLegal = 1 order by datetime desc");
    auto rows = _sqlExecutor->executeQuery(sql);
    for (int i = 0; i < rows.size(); ++i)
    {
        auto row = rows.at(i);
        auto identifier = row.value("identifier").toString();
        auto chat_name = row.value("chat_name").toString();
        if(chat_name.startsWith("项目分析: ") && chat_name.endsWith(identifier))continue;//跳过重复项目对话
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(chat_name);
        item->setData(Qt::UserRole, identifier);
        ui->chatListWidget->addItem(item);
    }
    //先在分析表找合法对话ID，放到左边对话项列表
    auto sql_analysis = QString("SELECT chat_name,programID  FROM file_analysis where filesName IS NOT NULL order by datetime desc");
    auto rows_analysis = _sqlExecutor->executeQuery(sql_analysis);
    for (int i = 0; i < rows_analysis.size(); ++i)
    {
        auto row = rows_analysis.at(i);
        auto identifier = row.value("programID").toString();
        auto chat_name = row.value("chat_name").toString();
        QListWidgetItem *item = new QListWidgetItem();
        qDebug()<<identifier;
        item->setText(chat_name);
        item->setData(Qt::UserRole, identifier);
        ui->chatListWidget->addItem(item);
    }
}

void DeepSeekWidget::showContextMenu(const QPoint &pos)
{
    QMenu menu(this);
    auto newChatAction = menu.addAction("新聊天");
    connect(newChatAction, &QAction::triggered, this, &DeepSeekWidget::newChat);
    menu.exec(QCursor::pos());
}

void DeepSeekWidget::showListWidgetContextMenu(const QPoint &pos)
{
    QMenu menu(this);
    auto deleteChatAction = menu.addAction("删除（无法恢复）");
    
    connect(deleteChatAction, &QAction::triggered, this, &DeepSeekWidget::deleteChat);
    menu.exec(QCursor::pos());
}

void DeepSeekWidget::loadChatMessage(QListWidgetItem *item)
{

    // deepseek库更新余额//jzk
    deepSeek->queryBalance();
    this->newChat();
    _identifier = item->data(Qt::UserRole).toString();
    _name = item->text();
    qDebug()<<_identifier<<_name;
    //项目分析项展示不一样
    if(_name.startsWith("项目分析: ") && _name.endsWith(_identifier)){
        qDebug()<<"项目分析loadChatMessage";
        //获取message表内容创建对话区
        auto sql = QString("SELECT content FROM messages WHERE programID = '%1'").arg(_identifier);
        QList<QVariantMap> rows = _sqlExecutor->executeQuery(sql);

        for(auto _: rows){
            auto content_all = _.value("content").toString();
            auto doc = QJsonDocument::fromJson(content_all.toUtf8());
            auto array = doc.array();
            int num=1;
            for (auto arr : array)
            {
                auto obj = arr.toObject();
                if(obj.value("role").toString()!="assistant" ||num!=1)continue;
                auto role = obj.value("role").toString();
                auto content = obj.value("content").toString();
                auto reasoning_content = obj.value("reasoning_content").toString();
                auto datetime = obj.value("datetime").toString();
                auto total_tokens = obj.value("total_tokens").toInt();

                auto chatFrame = new ChatFrame(stringToRole(role), nullptr);
                if (!content.isEmpty())
                {
                    chatFrame->addChatText(content);
                }
                if (!reasoning_content.isEmpty())
                {
                    chatFrame->addReasonerText(reasoning_content);
                }
                chatFrame->setTokenSize(total_tokens);
                chatFrame->setDateTime(datetime);
                _mainLayout->insertWidget(_mainLayout->count() - 1, chatFrame);
                num++;
            }
        }
        //获取analysis表结果创建对话区
        auto sql1 = QString("SELECT content FROM  file_analysis WHERE programID = '%1'").arg(_identifier);
        auto row = _sqlExecutor->executeFirstRow(sql1);
        // _identifier = row.value("identifier").toString();
        // _name = row.value("chat_name").toString();
        auto content = row.value("content").toString();
        auto doc = QJsonDocument::fromJson(content.toUtf8());
        auto array = doc.array();
        for (auto row : array)
        {
            auto obj = row.toObject();
            auto role = obj.value("role").toString();
            auto content = obj.value("content").toString();
            auto reasoning_content = obj.value("reasoning_content").toString();
            auto datetime = obj.value("datetime").toString();
            auto total_tokens = obj.value("total_tokens").toInt();
            auto chatFrame = new ChatFrame(stringToRole(role), nullptr);
            if (!content.isEmpty())
            {
                chatFrame->addChatText(content);
            }
            if (!reasoning_content.isEmpty())
            {
                chatFrame->addReasonerText(reasoning_content);
            }
            chatFrame->setTokenSize(total_tokens);
            chatFrame->setDateTime(datetime);
            _mainLayout->insertWidget(_mainLayout->count() - 1, chatFrame);

        }

    }
    qDebug()<<"loadChatMessage";
    auto sql = QString("SELECT chat_name,identifier,content FROM AnalysisMessage WHERE identifier = '%1'").arg(_identifier);
    auto row = _sqlExecutor->executeFirstRow(sql);
    // _identifier = row.value("identifier").toString();
    // _name = row.value("chat_name").toString();
    auto content = row.value("content").toString();
    auto doc = QJsonDocument::fromJson(content.toUtf8());
    auto array = doc.array();

    for (auto row : array)
    {
        auto obj = row.toObject();
        auto role = obj.value("role").toString();
        auto content = obj.value("content").toString();
        auto reasoning_content = obj.value("reasoning_content").toString();
        auto datetime = obj.value("datetime").toString();
        auto total_tokens = obj.value("total_tokens").toInt();
        auto chatFrame = new ChatFrame(stringToRole(role), nullptr);
        if (!content.isEmpty())
        {
            chatFrame->addChatText(content);
        }
        if (!reasoning_content.isEmpty())
        {
            chatFrame->addReasonerText(reasoning_content);
        }
        chatFrame->setTokenSize(total_tokens);
        chatFrame->setDateTime(datetime);
        _mainLayout->insertWidget(_mainLayout->count() - 1, chatFrame);

    }


}

void DeepSeekWidget::deleteChat()
{
    auto items = ui->chatListWidget->selectedItems();

    for (auto item : items)
    {
        auto identifier = item->data(Qt::UserRole).toString();
        _name= item->text();

        //项目分析项展示不一样
        if(_name.startsWith("项目分析: ") && _name.endsWith(_identifier)){
            auto sql = QString("Delete FROM file_analysis WHERE programID  = '%1'").arg(identifier);
            _sqlExecutor->executeNonQuery(sql);
            auto sql1 = QString("Delete FROM messages WHERE programID  = '%1'").arg(identifier);
            _sqlExecutor->executeNonQuery(sql1);
            continue;
        }

        if (identifier == _identifier)
            newChat();
        auto sql = QString("Delete FROM AnalysisMessage WHERE identifier = '%1'").arg(identifier);
        _sqlExecutor->executeNonQuery(sql);
        ui->chatListWidget->removeItemWidget(item);
        delete item;
    }
}



QStringList getFilesFromLevel2AndDeeper(const QString& rootPath,int FL) {
    QStringList result;

    // 获取标准化的根路径（处理Windows盘符和反斜杠）
    QDir rootDir(rootPath);
    QString absRoot = rootDir.absolutePath();
    // 统一使用正斜杠（Qt支持Windows上的正斜杠）
    absRoot.replace('\\', '/');
    if (!absRoot.endsWith('/')) absRoot += '/';

    QDirIterator it(rootPath,
                    QDir::Files,
                    QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString filePath = it.next().replace('\\', '/');  // 统一路径格式
        QString dirPath = QFileInfo(filePath).absolutePath().replace('\\', '/');
        if (!dirPath.endsWith('/')) dirPath += '/';
        // 计算深度：路径中比根目录多出的斜杠数量
        int depth = dirPath.count('/') - absRoot.count('/');

        // 只保留深度<=2的文件
        if (depth <= FL) {
            result << filePath;
        }
    }
    return result;
}


QStringList ignore_files(const QString& inputPath,QJsonArray& suffixesArray,int FL){
    QStringList filesName;
    if(inputPath==""){qDebug() << "空目录" ;return filesName;}
    // 读取目录并过滤 ignore_suffixes 文件
    // QDir inputDir(inputPath);
    // 获取目录下的所有文件名
    // filesName = inputDir.entryList(QDir::Files); // 正确的调用方式
    // 使用示例：
    filesName = getFilesFromLevel2AndDeeper(inputPath,FL);
    // 从配置中获取忽略后缀列表
    QStringList ignoreSuffixes;
    // QJsonArray suffixesArray = config["ignore_suffixes"].toArray();
    for (const QJsonValue& suffix : suffixesArray) {
        ignoreSuffixes.append(suffix.toString());
    }

    // 过滤文件：移除所有以 ignore_suffixes 中后缀结尾的文件
    QStringList filteredFiles;
    for (const QString& fileName : filesName) {
        bool shouldIgnore = false;

        // 检查文件名是否以任何忽略后缀结尾
        for (const QString& suffix : ignoreSuffixes) {
            if (fileName.endsWith(suffix, Qt::CaseInsensitive)) {
                shouldIgnore = true;
                break;
            }
        }
        // 如果不忽略，则添加到过滤后的列表
        if (!shouldIgnore) {
            filteredFiles.append(fileName);
        }
    }

    // 更新文件列表和总数
    filesName = filteredFiles;
    // 记录过滤信息
    qDebug() << "过滤后的文件数量:" << filesName.size();
    qDebug() << "忽略的文件后缀:" << ignoreSuffixes.join(", ");
    return filesName;
}

// 在类实现中添加分析文件回调函数形式的辅助函数
QString DeepSeekWidget::analyzeFileOnMainThread(const QString& question,const QString& question_que) {
    qDebug() << "analyzeFileOnMainThread" << _fileName;

    // iscontinue.wait(&_mutex);
    // 使用事件循环等待分析完成
    // QMutexLocker locker(&_mutex);
    QEventLoop loop;
    connect(deepSeek, &DeepSeek::replyFinished, &loop, &QEventLoop::quit);

    //对话-问
    ChatFrame *chatFrame = new ChatFrame(Role::User, this);
    chatFrame->setChatText(question_que);
    // 在右界面的继续添加提问
    _mainLayout->insertWidget(_mainLayout->count() - 1, chatFrame);
    // scrollArea 滚动到_spacer上面，也就要要最大值 - _spacer的高度
    // ui->chatScrollArea->verticalScrollBar()->setValue(ui->chatScrollArea->verticalScrollBar()->maximum() - spacer->height());
    // 在右界面的继续添加回答加载界面
    ChatFrame *reasonerFrame = new ChatFrame(Role::Assistant, this);
    _mainLayout->insertWidget(_mainLayout->count() - 1, reasonerFrame);
    // _logger->info(QString("seed: %1").arg(question));
    //deepseek库更新消息
    deepSeek->seedMessage(oldMessage(true), question);

    //延迟执行了一个 lambda 函数,确保完成赋值_identifier，_name再执行finish中后续相关内容
    QTimer::singleShot(500, this, [=]
                       { ui->chatScrollArea->ensureWidgetVisible(reasonerFrame); });
    reasonerFrame->startLoading();

    loop.exec();
    // deepseek库更新余额//jzk
    deepSeek->queryBalance();
    //等执行完成再出去
    return reasonerFrame->chatText();
}
// 文件分析结果总结
void DeepSeekWidget::AnalysisSummary() {
    qDebug() << "AnalysisSummary ";
    // QMutexLocker locker(&_mutex);
    _issummary=true;
    // 1. 根据项目id，读取messages中所有content结果
    QString summaryQuestion;
    try {
        // 获取所有分析结果
        QMap<QString, QVariant> params;
        auto sql = QString("SELECT content FROM messages WHERE programID = '%1'").arg(_identifier);
        QList<QVariantMap> rows = _sqlExecutor->executeQuery(sql);
        QString analysisContent;
        //保存提问文件到日志中

        // 2. 构建总结提问内容
        // _identifier = row.value("identifier").toString();
        // _name = row.value("chat_name").toString();
        for(auto _: rows){
            auto content_all = _.value("content").toString();
            auto doc = QJsonDocument::fromJson(content_all.toUtf8());
            auto array = doc.array();
            int num=1;
            for (auto arr : array)
            {
                auto obj = arr.toObject();
                if(obj.value("role").toString()!="assistant")continue;
                // auto role = obj.value("role").toString();
                auto content = obj.value("content").toString();
                // auto reasoning_content = obj.value("reasoning_content").toString();
                // auto datetime = obj.value("datetime").toString();
                // auto total_tokens = obj.value("total_tokens").toInt();
                analysisContent +="文件" +QString::number(num)+"分析结果:\n\n{"+content + "}\n\n";
                num++;

            }
        }
        _logger->info(QString("生成总结提问材料: %1\n\n").arg(analysisContent));
        // 构建总结提示词
        // 如果没有模板路径，使用默认生成方式
        QString m_jsonTemplatePath;
        m_jsonTemplatePath="./dependency_summary_template.txt";
        if (m_jsonTemplatePath.isEmpty() || !QFile::exists(m_jsonTemplatePath)) {
            summaryQuestion= "generateDefaultQuestion(filePath, content)";
            qDebug()<< "AnalysisSummary:generateDefaultQuestion(filePath, content)1 ";
        }

        // 尝试读取模板文件
        QFile templateFile(m_jsonTemplatePath);
        if (!templateFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            // m_logger->warn(QString("无法打开模板文件: %1，将使用默认问题生成").arg(m_jsonTemplatePath));
            summaryQuestion= "generateDefaultQuestion(filePath, content);";
            qDebug()<< "AnalysisSummary:generateDefaultQuestion(filePath, content)2 ";
        }

        // 读取模板内容
        QTextStream templateStream(&templateFile);
        QString templateContent = templateStream.readAll();
        templateFile.close();
        QJsonObject variables;
        variables["file_content"] = analysisContent;
        QString summaryQuestion = templateContent;
        for (auto it = variables.begin(); it != variables.end(); ++it) {
            QString placeholder = QString("{%1}").arg(it.key());
            summaryQuestion.replace(placeholder, it.value().toString());
        }

        // 3. 创建提问并发送到DeepSeek
        ChatFrame *chatFrame = new ChatFrame(Role::User, this);
        chatFrame->setChatText(summaryQuestion);
        _mainLayout->insertWidget(_mainLayout->count() - 1, chatFrame);

        ChatFrame *reasonerFrame = new ChatFrame(Role::Assistant, this);
        _mainLayout->insertWidget(_mainLayout->count() - 1, reasonerFrame);
        reasonerFrame->setChatText("正在生成项目总结报告...:\n\n");
        reasonerFrame->startLoading();

        deepSeek->seedMessage(oldMessage(true), summaryQuestion);
        deepSeek->queryBalance();

        QTimer::singleShot(500, this, [=] {
            ui->chatScrollArea->ensureWidgetVisible(reasonerFrame);
        });
    } catch (const std::exception& e) {
        _logger->error(QString("生成总结报告失败: %1").arg(e.what()));
        QMessageBox::critical(this, "错误", "生成总结报告时发生错误");
    }




}


void DeepSeekWidget::on_pushButton_clicked()

{
    isanalysis=true;
    // 1. 显示参数配置对话框
    pathConfigDialog = new PathConfigDialog(this);
    if (pathConfigDialog->exec() != QDialog::Accepted) {
        delete pathConfigDialog;
        pathConfigDialog=NULL;
        return;
    }


    // 2. 获取配置
    QJsonObject config = pathConfigDialog->getConfig();
    _inputPath = config.value("inputDir").toString();
    _outputPath = config.value("outputDir").toString();
    delete pathConfigDialog;
    pathConfigDialog = nullptr;
    qDebug() << "参数配置对话框完成传输配置";
    // 3. 创建新对话项
    // 3.1新建file_analysis表格行
    _identifier = QUuid::createUuid().toString();
    _name = "项目分析: " + QFileInfo(_inputPath).fileName()+"-"+_identifier;
     //  //3.2读取目录（FL级子目录以上文件)且滤除ignore_suffixes文件
    QJsonArray suffixesArray = config["ignore_suffixes"].toArray();
    QStringList filesName = ignore_files(_inputPath,suffixesArray,config.value("min_file_level").toInt());
    int totalFiles = filesName.size();
       // 3.4. 执行插入
    QJsonArray filesArray;
    for (const QString &file : filesName) {
        filesArray.append(file);
    }
    QJsonDocument filesDoc(filesArray);
    QString filesJson = filesDoc.toJson(QJsonDocument::Compact);
    QJsonDocument doc(config);
    QString configcontent = doc.toJson(QJsonDocument::Compact);
    try {
        // 准备参数化查询
        QMap<QString, QVariant> params;
        params[":programID"] = _identifier;
        params[":rootPath"] = _inputPath;
        params[":filesName"] = filesJson;  // JSON格式文件列表
        params[":datetime"] = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        params[":params"] = configcontent;
        params[":paramPath"] = "";  // 空字符串
        params[":jsonResultPath"] = _outputPath;
        params[":chat_name"] = _name;


        // 执行参数化插入
        int affectedRows = _sqlExecutor->executeNonQuerywithparams(
            "INSERT INTO file_analysis "
            "(programID, rootPath, filesName,chat_name, datetime, params, paramPath, jsonResultPath) "
            "VALUES (:programID, :rootPath, :filesName,:chat_name, :datetime, :params, :paramPath, :jsonResultPath)",
            params,
            true  // 启用事务
            );

        qDebug() << "成功插入" << affectedRows << "条记录";
    } catch (const std::runtime_error &e) {
        qCritical() << "保存项目配置到数据库失败:" << e.what();
        return;
    }
    //设置分析参数
    // +++ 新增代码：根据配置更新参数 +++
    // 获取single_file_analysis配置
    QJsonObject singleFileConfig = config.value("single_file_analysis").toObject();

    // 设置模型参数
    if (singleFileConfig.contains("model")&&0) {

        deepSeek->setModel(singleFileConfig["model"].toString());
    }

    // 设置其他数值参数
    QStringList floatParams = {"temperature", "top_p", "presence_penalty", "frequency_penalty"};
    for (const QString& param : floatParams) {
        if (singleFileConfig.contains(param)) {
            setParmas(param, singleFileConfig[param].toDouble());
        }
    }
    // +++ 参数更新结束 +++
    // 4. 在列表中添加新项
    //清空对话区
    while (_mainLayout->count() > 1)
    {
        auto chatFrame = static_cast<ChatFrame *>(_mainLayout->itemAt(0)->widget());
        if (chatFrame)
        {
            _mainLayout->removeWidget(chatFrame);
            delete chatFrame;
        }
    }
    QProgressDialog progress("分析项目文件...", "取消", 0, 100, this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(0);
    int processed = 0;
    // 7. 使用线程池进行多线程文件分析
    QThreadPool threadPool;
    threadPool.setMaxThreadCount(QThread::idealThreadCount()); // 使用最优线程数
    int idealThreadCount=QThread::idealThreadCount();
    qDebug()<<"理想线程数:"<<idealThreadCount;
        // 创建互斥锁和条件变量，用于线程同步
        QMutex mutex;
        QWaitCondition condition;
        // 用于存储分析结果
        QList<QPair<QString, QString>> analysisResults;
        // 创建分析回调函数
    auto analysisCallback = [this](const QString& question,const QString& question_que) -> QString {
        return this->analyzeFileOnMainThread(question,question_que);
    };
    QStringList extra_rules;
    // QJsonArray suffixesArray = config["ignore_suffixes"].toArray();
    for (const QJsonValue& suffix : config["extra_rules"].toArray()) {
        extra_rules.append(suffix.toString());
    }
    // 7.1 创建并提交分析任务
    iscontinue.wakeOne();
    FileAnalysisTask* task;
    QString answer;
    progress.setValue((0 * 75) / totalFiles);
    progress.setLabelText(QString("分析中: %1/%2 文件")
                              .arg(processed)
                              .arg(totalFiles));
    QCoreApplication::processEvents();
    for (const QString &filePath : filesName) {
        // QString filePath = _inputPath + "/" + file;

        _fileName=filePath;
        // 创建分析任务
        task = new FileAnalysisTask(
            filePath,
            _outputPath,
            _identifier,
            _sqlExecutor,
            _logger,
            &mutex,
            &condition,
            &analysisResults,
            analysisCallback,
            config["json_template"].toString(),
            extra_rules
            );

        // threadPool.start(task);
        task->run();
        processed++;
        // / 更新进度
        progress.setValue((processed * 75) / totalFiles);
        progress.setLabelText(QString("分析中: %1/%2 文件")
                                  .arg(processed)
                                  .arg(totalFiles));
        QCoreApplication::processEvents();
    }

    // 7.2 等待所有任务完成并更新进度
    while (processed < totalFiles) {
        if (progress.wasCanceled()) {
            threadPool.clear(); // 取消剩余任务
            break;
        }

    }
    // 9. 结果总结
    //设置分析参数
    // +++ 新增代码：根据配置更新参数 +++
    // 获取single_file_analysis配置
    QJsonObject singleFileConfig1 = config.value("summary").toObject();

    // 设置模型参数
    if (singleFileConfig.contains("model")&&0) {

        deepSeek->setModel(singleFileConfig1["model"].toString());
    }

    // 设置其他数值参数
    QStringList floatParams1 = {"temperature", "top_p", "presence_penalty", "frequency_penalty"};
    for (const QString& param : floatParams1) {
        if (singleFileConfig.contains(param)) {
            setParmas(param, singleFileConfig[param].toDouble());
        }
    }
    progress.setLabelText("生成总结报告...");
    progress.setValue(85);
    // _mutex.lock();
    // 临界区代码...

    // condition.wait(&_mutex); // 通知一个生产者
    AnalysisSummary();
    // _mutex.unlock();
    // 10. 完成
    progress.setValue(100);
    QMessageBox::information(this, "分析完成", "项目分析已完成，总结报告正在生成！");
    // 恢复设置的参数
    setconfig(deepSeek);
}


