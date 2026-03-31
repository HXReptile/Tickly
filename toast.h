#ifndef TOAST_H
#define TOAST_H

#include <QWidget>

namespace Ui {
class toast;
}

class toast : public QWidget
{
    Q_OBJECT
//    Q_PROPERTY(int posY READ posY WRITE setPosY)   // 用于滑动动画
public:
    explicit toast(const QString &text, QWidget *parent = NULL);
    ~toast();

    int posY();
    void setPosY(int y);
public slots:
    void slots_closeLater();
private:
    Ui::toast *ui;
    int m_posY;
    QTimer *m_autoCloseTimer;      // 10 秒后关闭
};

#endif // TOAST_H
