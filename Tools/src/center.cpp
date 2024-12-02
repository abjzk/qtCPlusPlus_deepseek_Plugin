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

Center::Center(QWidget *parent)
    : QWidget(parent), ui(new Ui::Center)
{
    this->_config->registerConfig("Plugins", "插件", TConfig::Type::String, QString(), false);
    ui->setupUi(this);
    this->initUi();
    this->initConnect();
    this->loadPluginTree();
    // 获取第一个插件
    QTreeWidgetItem *groupItem = ui->pluginTree->topLevelItem(0);
    QTreeWidgetItem *pluginItem = groupItem->child(0);
    QAbstractPlugin *plugin = qobject_cast<QAbstractPlugin *>(pluginItem->data(0, Qt::UserRole).value<QObject *>());
    plugin->start(new TConfig(plugin->name(), plugin));
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
    // ui->pluginTree->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    // ui->pluginTree->setItemsExpandable(true);
}

void Center::initConnect()
{
    // connect(ui->pluginTree, &QTreeWidget::customContextMenuRequested, this, &Center::showPluginTreeMenu);
}

void Center::showPluginTreeMenu(QPoint pos)
{
    QPoint globalPos = QCursor::pos();
    QMenu menu(this);
    menu.exec(globalPos);
}
void Center::loadPluginTree()
{
    QString str = this->_config->read("Plugins").toString();
    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
    QJsonObject obj = doc.object();
    QDir dir(QApplication::applicationDirPath());
    // 读取该目录下所有*Plugin.dll文件
    QStringList list = dir.entryList(QStringList("*Plugin.dll"), QDir::Files);
    for (int i = 0; i < list.size(); i++)
    {
        QString filename = list.at(i).split(".")[0];
        if (obj.contains(filename))
        {
            QJsonObject item = obj.value(filename).toObject();
            if (item.contains("enable"))
            {
                bool enable = item.value("enable").toBool();
                if (enable)
                {
                    int retFlag;
                    addPlugin(filename, retFlag);
                    if (retFlag == 3)
                        continue;
                }
            }
        }
        else
        {
            int retFlag;
            addPlugin(filename, retFlag);
            if (retFlag == 3)
                continue;
            QJsonObject newObj = {{"enable", true}};
            obj.insert(filename, newObj);
        }
    }
    doc.setObject(obj);
    QVariant value = QString::fromUtf8(doc.toJson());
    this->_config->write(QString("Plugins"), value);
}

QAbstractPlugin *Center::findPlugin(QString &name)
{
    QPluginLoader loader(QApplication::applicationDirPath() + "/" + name + ".dll");
    PluginFactory *factory = qobject_cast<PluginFactory *>(loader.instance());
    return factory->create();
}

void Center::addPlugin(QString &filename, int &retFlag)
{
    QAbstractPlugin *plugin = findPlugin(filename);
    // plugin->start(new TConfig(plugin->name(), plugin));
    QString group = plugin->group().isEmpty() ? "未分组" : plugin->group();
    // 查找是否存在当前分组的节点
    QTreeWidgetItem *groupItem = nullptr;
    for (int i = 0; i < ui->pluginTree->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *item1 = ui->pluginTree->topLevelItem(i);
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
    pluginItem->setData(0, Qt::UserRole, plugin->objectName());
    pluginItem->setToolTip(0, plugin->description());
    // 添加到组中
    groupItem->addChild(pluginItem);
    qDebug()<<groupItem->childCount();
    qDebug() << "插件" << plugin->name() << "加载成功";
    delete plugin;
}