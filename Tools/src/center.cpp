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
#include <QAbstractPlugin.h>
#include <QPluginLoader>
#include <qexception.h>
#include <qtreewidget.h>
#include "configDialog.h"
#include <LCore>
#include "MainWindow.h"

Center::Center(TConfig *config,QWidget *parent)
    : QWidget(parent), ui(new Ui::Center), _config(config)
{
    this->_config->registerWriteConfigAfterCallback([this](WriteConfigEvent &event)->void { this->writeConfigAfterEvent(event); });
    this->_config->registerConfig("Plugins", "插件", TConfig::Type::String, QString(), false);
    this->_config->registerConfig("autoStart", "开机自启动", TConfig::Type::Bool, false, true);
    ui->setupUi(this);
    this->initUi();
    this->initConnect();
    this->loadPluginTree();
}

Center::~Center()
{
    QList<QAbstractPlugin *> list = this->findChildren<QAbstractPlugin *>();
    for (QAbstractPlugin *plugin : list)
    {
        plugin->stop();
    }
    delete ui;
}

void Center::initUi()
{
    // 给treeWidget设置右键菜单
    // 初始化分割器的尺寸
    ui->splitter->setSizes(QList<int>() << 200 << 600);
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
    connect(ui->logButton,&QPushButton::clicked,this,&Center::showLogDialog);
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
        auto item = event.newItem;
        item.value.toBool();
        QString name = QApplication::applicationName();
        LFunc::autoRun(item.value.toBool(), "LJZ"+ name);
    }
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
    if(logDialog) return;
    logDialog = new LogDialog();
    logDialog->setWindowIcon(currentPlugin->icon());
    logDialog->getTitleBar()->setTitleIcon(currentPlugin->icon());
    logDialog->resize(600,400);
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
    if(item->childCount() > 0) return;
    if(currentPlugin && item->text(0) == currentPlugin->name()) return;
    delete ui->scrollArea->widget();
    if (currentPlugin)
        currentPlugin->stop();
    delete currentPlugin;
    if(logDialog)
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
        if (!obj.contains(filename))
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

QAbstractPlugin *Center::findPlugin(QString name)
{
    QPluginLoader loader(QApplication::applicationDirPath() + "/" + name + ".dll");
    PluginFactory *factory = qobject_cast<PluginFactory *>(loader.instance());
    return factory->create(new Logger(name),new TConfig(name));
}

QTreeWidgetItem *Center::addPlugin(QString &filename)
{
    QAbstractPlugin *plugin = findPlugin(filename);
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