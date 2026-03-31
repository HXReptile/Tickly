#ifndef ITEMWIDGET_H
#define ITEMWIDGET_H

#include <QWidget>
#include <common_utils.h>
namespace Ui {
class itemwidget;
}

class itemwidget : public QWidget
{
    Q_OBJECT

public:
    explicit itemwidget(ItemInfo info,QWidget *parent = nullptr);
    ~itemwidget();
    void setItemInfo(ItemInfo info);
protected slots:
    void slots_toggledclicked(bool tg);
signals:
    void signals_delete();
    void signals_toggled(bool tg);
    void signals_setting();
private:
    Ui::itemwidget *ui;
};

#endif // ITEMWIDGET_H
