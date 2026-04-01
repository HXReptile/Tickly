#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingwidget.h"
#include <QUuid>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <itemwidget.h>
#include <toastmanager.h>
#include <QSystemTrayIcon>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_Timer(new QTimer)
    , m_settingwidget2(new settingwidget2(this))
{
    ui->setupUi(this);
    setWindowTitle("Tickly");

    QRect rect = QGuiApplication::primaryScreen()->availableGeometry();
    int x = rect.right();
    int y = rect.bottom();
    ToastManager::moveToPosition(x,y);

    ui->listWidget_task->setDragDropMode(QAbstractItemView::InternalMove);
    connect(ui->listWidget_task->model(), &QAbstractItemModel::rowsMoved,this, &MainWindow::slots_rowmove);
    setMenu();

    m_ItemInfo.clear();
    if (ensureJsonFile(DATA_PATH)) {
        loadFromJson(DATA_PATH);

        qDebug() << QString::fromLocal8Bit("JSON 已准备好")<<QString::number(m_ItemInfo.size());
        int size = m_ItemInfo.size();
        for(int i = 0;i<size;++i){
            ItemInfo info = m_ItemInfo.at(i);

            addTask(info);
        }
    }else{
        qDebug() << "失败";
    }

    connect(m_Timer,&QTimer::timeout,this,&MainWindow::slots_timeout);
    qint64 interal = compute_time();
    if(interal <= 0){
        m_Timer->stop();
    }
    qDebug()<<"next time:"<<QString::number(interal);
    m_Timer->start(interal*1000);

    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/re/alarm.png"));
    trayIcon->setToolTip("Tickly");
    // 托盘菜单
    QMenu *menu = new QMenu(this);
    QAction *openAction = new QAction("打开", this);
    QAction *quitAction = new QAction("退出", this);
    connect(openAction, &QAction::triggered, this, &MainWindow::slots_openWindow);
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);
    menu->addAction(openAction);
    menu->addAction(quitAction);
    trayIcon->setContextMenu(menu);
    trayIcon->show();
    setWindowFlags(Qt::Tool);
    hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

qint64 MainWindow::compute_time()
{
    qint64 curTime = QDateTime::currentSecsSinceEpoch();
    //计算距离下个触发还需多久 和ID绑定 QList<ItemInfo>
    QMap<qint64,ItemInfo> minTime;
    int size = m_ItemInfo.size();
    for(int i = 0;i<size;++i){
        ItemInfo info = m_ItemInfo.at(i);
        if(!info.enabled){
            continue;
        }
        qint64 value = -1;
        for(int j = 0;j < info.timevec.size();++j){
            qint64 startTime = QDateTime(QDate::currentDate(), QTime::fromString(info.timevec[j].start, TIME_FORMAT)).toSecsSinceEpoch();
            qint64 endTime = QDateTime(QDate::currentDate(), QTime::fromString(info.timevec[j].end, TIME_FORMAT)).toSecsSinceEpoch();

            //与当前时间对比 前 中 后
            if(endTime <= curTime){
                continue;
            }else if(startTime >= curTime){
                value = startTime - endTime;
            }else if(startTime < curTime && endTime > curTime){
                value = startTime;
                while (value <= curTime) {
                    value += info.interval*60;
                }
                value = value - curTime;
                break;
            }
        }
        if(value > 0){
            minTime.insert(value,info);
        }
    }
    if(minTime.size() >= 1){
        m_showContent = minTime.first().message;
        return minTime.firstKey();
    }
    return -1;
}

void MainWindow::slots_addTask()
{
    QUuid id = QUuid::createUuid();
    QString idStr = id.toString();

    ItemInfo info;
    info.id = idStr;
    info.order = ui->listWidget_task->count() + 1;
    info.name = tr("新建任务");

    m_ItemInfo.append(info);
    addTask(info);

    saveToJson(DATA_PATH,m_ItemInfo);
}

void MainWindow::slots_rowmove()
{
    QList<ItemInfo> newList;

    for(int i = 0; i < ui->listWidget_task->count(); ++i)
    {
        QListWidgetItem* item = ui->listWidget_task->item(i);

        ItemInfo info = item->data(Qt::UserRole).value<ItemInfo>();
        info.order = i;

        newList.append(info);
    }

    m_ItemInfo = newList;

    saveToJson(DATA_PATH,m_ItemInfo);
}

void MainWindow::slots_delete()
{
    itemwidget* senderWidget = qobject_cast<itemwidget*>(sender());
    if(!senderWidget) return;
    QListWidgetItem* item = senderWidget->property("item").value<QListWidgetItem*>();

    if(!item) return;

    int row = ui->listWidget_task->row(item);
    ItemInfo info = item->data(Qt::UserRole).value<ItemInfo>();

    int size = m_ItemInfo.size();
    for(int i = 0;i<size;++i){
        ItemInfo tmp = m_ItemInfo.at(i);

        if(tmp.id == info.id){
            m_ItemInfo.removeAt(i);
            break;
        }
    }

    delete ui->listWidget_task->takeItem(row);
    saveToJson(DATA_PATH,m_ItemInfo);
}

void MainWindow::slots_toggled(bool tg)
{
    itemwidget* senderWidget = qobject_cast<itemwidget*>(sender());
    if(!senderWidget) return;
    QListWidgetItem* item = senderWidget->property("item").value<QListWidgetItem*>();

    if(!item) return;

    int row = ui->listWidget_task->row(item);
    ItemInfo info = item->data(Qt::UserRole).value<ItemInfo>();

    int size = m_ItemInfo.size();
    for(int i = 0;i<size;++i){
        if(m_ItemInfo.at(i).id == info.id){
            m_ItemInfo[i].enabled = tg;
            break;
        }
    }

    saveToJson(DATA_PATH,m_ItemInfo);
}

void MainWindow::slots_settings()
{
    itemwidget* senderWidget = qobject_cast<itemwidget*>(sender());
    if(!senderWidget) return;
    QListWidgetItem* item = senderWidget->property("item").value<QListWidgetItem*>();
    if(!item) return;

    ItemInfo info = item->data(Qt::UserRole).value<ItemInfo>();

    settingwidget* widget = new settingwidget(info,this);
    connect(widget,&settingwidget::signals_ItemInfo,this,&MainWindow::slots_ItemInfo);
    widget->show();
}

void MainWindow::slots_ItemInfo(ItemInfo info)
{
    int size = m_ItemInfo.size();
    for(int i = 0;i< size;++i){
        if(m_ItemInfo[i].id == info.id){
            m_ItemInfo[i].name = info.name;
            m_ItemInfo[i].interval = info.interval;
            m_ItemInfo[i].message = info.message;
            m_ItemInfo[i].timevec = info.timevec;
            break;
        }
    }
    saveToJson(DATA_PATH,m_ItemInfo);

    //刷新
    for (int i = 0; i < ui->listWidget_task->count(); ++i){
        QListWidgetItem* item = ui->listWidget_task->item(i);
        if(!item) return;
        ItemInfo tmp = item->data(Qt::UserRole).value<ItemInfo>();
        if(tmp.id == info.id){
            item->setData(Qt::UserRole, QVariant::fromValue(info));

            itemwidget* taskWidget = qobject_cast<itemwidget*>(ui->listWidget_task->itemWidget(item));
            if(!taskWidget)
                return;
            taskWidget->setItemInfo(info);
            break;
        }
    }
}

void MainWindow::slots_test()
{
    QString appName = QCoreApplication::applicationFilePath();
    qDebug()<<"name:"<<appName;
}

void MainWindow::slots_timeout()
{
    DailyLogger::log(m_showContent);
    ToastManager::show(m_showContent);

    qint64 interal = compute_time();
    if(interal <= 0){
        m_Timer->stop();
    }
    qDebug()<<"next time:"<<QString::number(interal);
    m_Timer->start(interal*1000);
}

void MainWindow::slots_helpSettings()
{
    if(m_settingwidget2){
        m_settingwidget2->show();
    }
}

void MainWindow::slots_openWindow()
{
    setWindowFlags(Qt::Window);
    show();
}

void MainWindow::setMenu()
{
    QMenuBar *menuBar = this->menuBar();

    QMenu *fileMenu = menuBar->addMenu("文件");
    // QMenu *editMenu = menuBar->addMenu("编辑");
    QMenu *helpMenu = menuBar->addMenu("帮助");

    QAction *newAction = new QAction(QIcon(":/re/home-1.png"),"新建", this);
    QAction *openAction = new QAction(QIcon(":/re/Upload.png"),"打开", this);
    QAction *testAction = new QAction(QIcon(":/re/Upload.png"),"测试", this);
    QAction *exitAction = new QAction(QIcon(":/re/power.png"),"退出", this);
    QAction *settingsAction = new QAction(QIcon(":/re/settings.png"),"设置", this);

    fileMenu->addAction(newAction);
    // fileMenu->addAction(openAction);
    // fileMenu->addAction(testAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    helpMenu->addAction(settingsAction);

    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);
    connect(newAction, &QAction::triggered, this, &MainWindow::slots_addTask);
    connect(testAction, &QAction::triggered, this, &MainWindow::slots_test);
    connect(settingsAction, &QAction::triggered, this, &MainWindow::slots_helpSettings);
}

bool MainWindow::ensureJsonFile(const QString &filePath)
{
    QFile file(filePath);

    if (file.exists()) {
        return true;
    }

    QFileInfo fileInfo(filePath);
    QDir dir;

    if (!dir.exists(fileInfo.path())) {
        if (!dir.mkpath(fileInfo.path())) {
            qDebug() << "创建目录失败:" << fileInfo.path();
            return false;
        }
    }

    // 3. 创建 JSON 文件（写入默认内容）
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "创建文件失败:" << filePath;
        return false;
    }

    file.close();

    return true;
}

void MainWindow::addTask(const ItemInfo& info)
{
    itemwidget* taskWidget = new itemwidget(info);
    connect(taskWidget,&itemwidget::signals_delete,this,&MainWindow::slots_delete);
    connect(taskWidget,&itemwidget::signals_toggled,this,&MainWindow::slots_toggled);
    connect(taskWidget,&itemwidget::signals_setting,this,&MainWindow::slots_settings);
    QListWidgetItem *item = new QListWidgetItem(ui->listWidget_task);
    item->setData(Qt::UserRole, QVariant::fromValue(info));
    item->setSizeHint(taskWidget->sizeHint());
    taskWidget->setProperty("item", QVariant::fromValue(item));
    // ui->listWidget_task->addItem(item);
    ui->listWidget_task->setItemWidget(item, taskWidget);
    ui->listWidget_task->insertItem(info.order, item);
}

void MainWindow::loadFromJson(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return;

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray()) return;

    QJsonArray arr = doc.array();

    for (const QJsonValue& val : arr) {
        QJsonObject obj = val.toObject();

        ItemInfo item;
        item.id = obj["id"].toString();
        item.order = obj["order"].toInt();
        item.name = obj["name"].toString();
        item.enabled = obj["enabled"].toBool();
        item.interval = obj["interval"].toInt();

        QJsonArray ranges = obj["timeRanges"].toArray();
        for (const QJsonValue& r : ranges) {
            QJsonObject rObj = r.toObject();
            TimeRange tr;
            tr.start = rObj["start"].toString();
            tr.end = rObj["end"].toString();
            item.timevec.append(tr);
        }

        item.skipSaturdays = obj["skipSaturdays"].toBool();
        item.skipSundays = obj["skipSundays"].toBool();
        item.skipHolidays = obj["skipHolidays"].toBool();
        item.notificationType = obj["notificationType"].toInt();
        item.message = obj["message"].toString();

        m_ItemInfo.append(item);
    }

    return;
}

bool MainWindow::saveToJson(const QString &filePath, const QList<ItemInfo> &list)
{
    QJsonArray arr;

    int size = list.size();
    for(int i = 0;i<size;++i){
        ItemInfo item = list.at(i);

        QJsonObject obj;
        obj["id"] = item.id;
        obj["order"] = item.order;
        obj["name"] = item.name;
        obj["enabled"] = item.enabled;
        obj["interval"] = item.interval;

        // timeRanges
        QJsonArray ranges;
        for (const TimeRange& tr : item.timevec) {
            QJsonObject rObj;
            rObj["start"] = tr.start;
            rObj["end"] = tr.end;
            ranges.append(rObj);
        }
        obj["timeRanges"] = ranges;

        obj["skipSaturdays"] = item.skipSaturdays;
        obj["skipSundays"] = item.skipSundays;
        obj["skipHolidays"] = item.skipHolidays;
        obj["notificationType"] = item.notificationType;
        obj["message"] = item.message;

        arr.append(obj);
    }

    QJsonDocument doc(arr);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) return false;

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    return true;
}
