#ifndef SETTINGWIDGET2_H
#define SETTINGWIDGET2_H

#include <QWidget>

namespace Ui {
class settingwidget2;
}

class settingwidget2 : public QWidget
{
    Q_OBJECT

public:
    explicit settingwidget2(QWidget *parent = nullptr);
    ~settingwidget2();
protected slots:
    void slots_isAutoStart(bool enable);//自启动
private:
    bool isAutoStartEnabled();
private:
    Ui::settingwidget2 *ui;
};

#endif // SETTINGWIDGET2_H
