#ifndef REMINDWIDGET_H
#define REMINDWIDGET_H

#include <QWidget>

namespace Ui {
class remindwidget;
}

class remindwidget : public QWidget
{
    Q_OBJECT

public:
    explicit remindwidget(QWidget *parent = nullptr);
    ~remindwidget();

    void setText(QString text);
private:
    void move2center();
private:
    Ui::remindwidget *ui;
};

#endif // REMINDWIDGET_H
