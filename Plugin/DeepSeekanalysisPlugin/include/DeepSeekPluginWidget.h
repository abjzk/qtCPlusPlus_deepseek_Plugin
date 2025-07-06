#pragma once
#include "DeepSeekPlugin_global.h"
#include "ui_DeepSeekanalysisPluginWidget.h"
#include <config.h>
#include <AbstractPlugin.h>
#include <DeepSeek.h>
#include "ChatFrame.h"
#include <QLayout>
#include <QEvent>
#include <QKeyEvent>
#include <QWidget>
#include <LCore>
#include "sdialog.h"
#include <QProgressDialog> // 新增进度对话框支持
#include <QDir>           // 新增目录操作支持
#include <QFileInfo>      // 新增文件信息支持
#include <QMessageBox>
#include <QImage>
#include <QIcon>
#include <qdesktopservices.h>
#include <QScrollBar>
#include <QTimer>
#include <QMenu>
#include <QAction>
#include <QUuid>
#include <QDebug>
// 在头文件顶部添加
#include <QThreadPool>
#include <QMutex>
#include <QWaitCondition>
#include <QRunnable>
#include "FileAnalysisTask.h" // 确保包含任务类头文件

class DeepSeekanalysisPlugin_EXPORT DeepSeekWidget : public QWidget
{
    Q_OBJECT
public:
    DeepSeekWidget(Logger *logger, TConfig *config, QWidget *parent = nullptr);
    ~DeepSeekWidget();

public:
    void setParmas(QString key, QVariant value);
    // 多线程分析辅助函数
    QString analyzeFileOnMainThread(const QString& question);
private:
    void initUi();
    void initConnect();
    PathConfigDialog* pathConfigDialog;
    Ui::DeepSeekPluginWidget *ui;
    //配置 日志
    TConfig *_config;
    Logger *_logger;
    QString _seed_key;     // 发送快捷键
    QString _identifier;
    QString _name;//对话项名
    QString _inputPath;    // 输入目录路径
    QString _outputPath;   // 输出目录路径

    QWidget *_mainWidget = new QWidget(this);
    QVBoxLayout *_mainLayout = new QVBoxLayout(_mainWidget);
    DeepSeek *deepSeek = nullptr;

    virtual void keyPressEvent(QKeyEvent *event) override;
    void addLastMessage(const DeepSeek::Message &message);
    void moneyChange(const DeepSeek::Balance &balance);
    void finished(QNetworkReply::NetworkError error, int httpStatusCode, const QString &errorString);
    QList<DeepSeek::Message> oldMessage();
    QWidget *_spacer = nullptr;

    void newChat();
    void loadChat();
    void showContextMenu(const QPoint &pos);
    void showListWidgetContextMenu(const QPoint &pos);
    void loadChatMessage(QListWidgetItem *item);
    void deleteChat();

    // 文件分析相关函数
    void fileAnalysis(const QString& filePath, const QString& outputDir);
    void AnalysisTR(); // 分析结果转换
    void AnalysisSummary(); // 分析结果总结



    void on_pushButton_clicked(); // 主分析按钮点击处理

    jzk::LSqlExecutor *_sqlExecutor = new jzk::LSqlExecutor(QApplication::applicationDirPath() + "/Deepseekconfig.db"); // 添加命名空间
};
