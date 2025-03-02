#include "center.h"
#include <QMenu>
#include <QCursor>
#include <QAction>
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QApplication>
#include <QDir>
#include <AbstractPlugin.h>
#include <QPluginLoader>
#include <qexception.h>
#include <qtreewidget.h>
#include "configDialog.h"
#include <LCore>
#include "MainWindow.h"
#include <RemoveLogTask.h>
#ifdef Q_OS_WIN
#include <Windows.h>
#endif

Center::Center(TConfig *config, QWidget *parent)
    : QWidget(parent), ui(new Ui::Center), _config(config)
{
    this->_config->registerReadConfigBeforeCallback([this](ReadConfigEvent &event) -> void
                                                    { this->readConfigBeforeEvent(event); });
    this->_config->registerReadConfigAfterCallback([this](ReadConfigEvent &event) -> void
                                                   { this->readConfigAfterEvent(event); });
    this->_config->registerWriteConfigBeforeCallback([this](WriteConfigEvent &event) -> void
                                                     { this->writeConfigBeforeEvent(event); });
    this->_config->registerWriteConfigAfterCallback([this](WriteConfigEvent &event) -> void
                                                    { this->writeConfigAfterEvent(event); });
    this->_config->registerConfig("Plugins", "插件", TConfig::Type::String, QString(), false);
    this->_config->registerConfig("autoStart", "开机自启动", TConfig::Type::Bool, false, true);
    this->_config->registerConfig("SavelogDay", "保存日志天数", TConfig::Type::Int, 30, true);
    ui->setupUi(this);
    this->initUi();
    this->initConnect();
    this->loadPluginTree();
    RemoveLogTask *task = new RemoveLogTask(_config->read("SavelogDay").value.toInt(), this);
    connect(task, &RemoveLogTask::finished, task, &RemoveLogTask::deleteLater);
    task->start();
    // QWebEnginePage *view = new QWebEnginePage(this);
}

Center::~Center()
{
    QList<AbstractPlugin *> list = this->findChildren<AbstractPlugin *>();
    for (AbstractPlugin *plugin : list)
    {
        plugin->stop();
    }
    delete ui;
}

void Center::initUi()
{
    // 给treeWidget设置右键菜单
    // 初始化分割器的尺寸
    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 6);
    ui->pluginTree->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    ui->pluginTree->setItemsExpandable(true);
    QFont font = ui->pluginTree->font();
    font.setPointSize(15);
    font.setBold(true);
    ui->pluginTree->setFont(font);
    ui->pluginTree->setIconSize(QSize(25, 25));
}

void Center::initConnect()
{
    connect(ui->pluginTree, &QTreeWidget::customContextMenuRequested, this, &Center::showPluginTreeMenu);
    connect(ui->pluginTree, &QTreeWidget::itemClicked, this, &Center::pluginItemClicked);
    connect(ui->pushButton, &QPushButton::clicked, this, &Center::showConfigDialog);
    connect(ui->logButton, &QPushButton::clicked, this, &Center::showLogDialog);
}

void Center::showPluginTreeMenu(QPoint pos)
{
    QPoint globalPos = QCursor::pos();
    QMenu menu(this);
    QAction *action = menu.addAction(LFunc::getoppositeColorIcon(QIcon(":/res/icon/refresh.png"), this->palette().window().color()), "刷新");
    connect(action, &QAction::triggered, this, &Center::loadPluginTree);
    menu.exec(globalPos);
}
void Center::writeConfigAfterEvent(WriteConfigEvent &event)
{
    if (event.key == "autoStart")
    {
        QString name = QApplication::applicationName();
        LFunc::autoRun(event.newValue().toBool(), "JZK" + name);
    }
}
void Center::writeConfigBeforeEvent(WriteConfigEvent &event)
{
    if (event.key == "SavelogDay")
    {

        int days = event.newValue().toInt();
        if (days < 0)
        {
            event.isValid = false;
            event.message = "保存日志天数不能小于0";
        }
        else if (days > 365)
        {
            event.isValid = false;
            event.message = "保存日志天数不能大于365";
        }
    }
}
void Center::readConfigBeforeEvent(ReadConfigEvent &event)
{
}
void Center::readConfigAfterEvent(ReadConfigEvent &event)
{
}
void Center::showConfigDialog()
{
    if (dialog)
        return;
    dialog = new ConfigDialog(currentPlugin->config());
    connect(dialog, &ConfigDialog::closed, this, &Center::reSet);
    this->setEnabled(false);
    dialog->setWindowIcon(currentPlugin->icon());
    dialog->getTitleBar()->setTitleIcon(currentPlugin->icon());
    dialog->exec();
}
void Center::showLogDialog()
{
    if (logDialog)
    {
        logDialog->deleteLater();
        logDialog = nullptr;
    }
    logDialog = new LogDialog();
    logDialog->setWindowIcon(currentPlugin->icon());
    logDialog->getTitleBar()->setTitleIcon(currentPlugin->icon());
    logDialog->resize(600, 400);
    logDialog->setObjectName(currentPlugin->logger()->name());
    logDialog->show();
    connect(currentPlugin->logger(), &Logger::sendLogger, logDialog, &LogDialog::addLogItem);
}

void Center::reSet()
{
    this->setEnabled(true);
    if (dialog)
    {
        dialog->deleteLater();
        dialog = nullptr;
    }
}
void Center::pluginItemClicked(QTreeWidgetItem *item, int column)
{
    if (item->childCount() > 0)
        return;
    if (currentPlugin && item->text(0) == currentPlugin->name())
        return;
    delete ui->scrollArea->widget();
    if (currentPlugin)
        currentPlugin->stop();
    delete currentPlugin;
    if (logDialog)
        logDialog->deleteLater();
    logDialog = nullptr;
    currentPlugin = this->findPlugin(item->data(0, Qt::UserRole).toString());
    ui->nameLabel->setText(currentPlugin->name());
    ui->versionlabel->setText(currentPlugin->version());
    ui->authorlabel->setText(currentPlugin->author());
    ui->scrollArea->setWidget(currentPlugin->start());
    QWidget *parent = this->parentWidget();
    if (parent)
    {
        MainWindow *mainWindow = qobject_cast<MainWindow *>(parent);
        mainWindow->systemSettingsChangedSlot();
        // #ifdef Q_OS_WIN
        //         mainWindow->setWindowFlags(Qt::FramelessWindowHint | Qt::Window); // 无边框
        //         mainWindow->setAttribute(Qt::WA_TranslucentBackground);           // 背景透明
        //         HWND hwnd = reinterpret_cast<HWND>(mainWindow->winId());
        //         DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);
        //         ::SetWindowLong(hwnd, GWL_STYLE, style | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU);
        //         mainWindow->show();
        // #endif
    }
}
void Center::loadPluginTree()
{
    ui->pluginTree->clear();
    QString str = this->_config->read("Plugins").value.toString();
    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
    QJsonObject obj = doc.object();
    QDir dir(QApplication::applicationDirPath());
    // 读取该目录下所有*Plugin.dll文件
    QStringList list = dir.entryList(QStringList("*Plugin.dll"), QDir::Files);
    for (int i = 0; i < list.size(); i++)
    {
        QString filename = list.at(i).split(".")[0];
        auto treeItem = addPlugin(filename);
        if (treeItem && !obj.contains(filename))
        {
            QJsonObject newObj = {{"enable", true}};
            obj.insert(filename, newObj);
        }
    }
    doc.setObject(obj);
    QVariant value = QString::fromUtf8(doc.toJson());
    QString message;
    this->_config->write(QString("Plugins"), value, message);
    // 获取第一个插件
    QTreeWidgetItem *groupItem = ui->pluginTree->topLevelItem(0);
    QTreeWidgetItem *pluginItem = groupItem->child(0);
    this->pluginItemClicked(pluginItem, 0);
}

AbstractPlugin *Center::findPlugin(QString name)
{
    QPluginLoader loader(QApplication::applicationDirPath() + "/" + name + ".dll");
    PluginFactory *factory = qobject_cast<PluginFactory *>(loader.instance());
    return factory ? factory->create(new Logger(name), new TConfig(name)) : nullptr;
}

QTreeWidgetItem *Center::addPlugin(QString &filename)
{
    AbstractPlugin *plugin = findPlugin(filename);
    if (plugin == nullptr)
        return nullptr;
    plugin->registerConfig();
    // plugin->start(new TConfig(plugin->name(), plugin));
    QString group = plugin->group().isEmpty() ? "未分组" : plugin->group();
    // 查找是否存在当前分组的节点
    QTreeWidgetItem *groupItem = nullptr;
    for (int i = 0; i < ui->pluginTree->topLevelItemCount(); i++)
    {
        auto item1 = ui->pluginTree->topLevelItem(i);
        if (item1->text(0) == group)
        {
            groupItem = item1;
            break;
        }
    }
    if (!groupItem)
    {
        groupItem = new QTreeWidgetItem(ui->pluginTree);
        groupItem->setText(0, group);
        ui->pluginTree->addTopLevelItem(groupItem);
    }
    QTreeWidgetItem *pluginItem = new QTreeWidgetItem(groupItem);
    pluginItem->setText(0, plugin->name());
    pluginItem->setIcon(0, plugin->icon());
    pluginItem->setData(0, Qt::UserRole, filename);
    pluginItem->setToolTip(0, plugin->description());
    // 添加到组中
    groupItem->addChild(pluginItem);
    delete plugin;
    return pluginItem;
}
void Center::on_logButton_customContextMenuRequested(const QPoint &pos)
{

}

