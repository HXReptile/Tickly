#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <common_utils.h>
#include "settingwidget2.h"
#include <QTimer>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    qint64 compute_time();//计算
protected slots:
    void slots_addTask();//添加任务
    void slots_rowmove();//顺序拖动
    void slots_delete();//删除
    void slots_toggled(bool tg);//状态
    void slots_settings();//设置
    void slots_ItemInfo(ItemInfo info);
    void slots_test();//测试接口
    void slots_timeout();
    void slots_helpSettings();//设置
    void slots_openWindow();
private:
    void setMenu();//设置菜单
    bool ensureJsonFile(const QString& filePath);//判断JSON文件状态
    void addTask(const ItemInfo& info);

    void loadFromJson(const QString& filePath);//读取JSON
    bool saveToJson(const QString& filePath, const QList<ItemInfo>& list);//写入

private:
    Ui::MainWindow *ui;
    QList<ItemInfo> m_ItemInfo;
    QTimer* m_Timer;
    QString m_showContent;
    settingwidget2* m_settingwidget2;
};
#endif // MAINWINDOW_H
