#ifndef SETTINGWIDGET_H
#define SETTINGWIDGET_H

#include <QWidget>
#include <common_utils.h>

namespace Ui {
class settingwidget;
}

class settingwidget : public QWidget
{
    Q_OBJECT

public:
    explicit settingwidget(ItemInfo info,QWidget *parent = nullptr);
    ~settingwidget();

protected slots:
    void slots_sureclicked();
    void slots_newadd();//新增
signals:
    void signals_ItemInfo(ItemInfo info);
private:
    void addTimeRange(const TimeRange& range);
    void removeLayout(QLayout* layout);
    QList<TimeRange> getRanges();
private:
    Ui::settingwidget *ui;
    ItemInfo m_ItemInfo;
};

#endif // SETTINGWIDGET_H
