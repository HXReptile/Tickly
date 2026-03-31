#ifndef TOASTMANAGER_H
#define TOASTMANAGER_H
#include <QVector>
#include <QScreen>
#include "toast.h"

class ToastManager
{
public:
    ToastManager();
    // 统一入口
    static void show(const QString &text);

    static void moveToPosition(int x,int y);
private:
    // 所有活跃 Toast 的指针
    static QVector<toast*> activeToasts;

    // 重新排布所有 Toast
    static void repositionAll();

    static int m_x;
    static int m_y;
};

#endif // TOASTMANAGER_H
