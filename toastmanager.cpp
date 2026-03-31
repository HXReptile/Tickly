#include "toastmanager.h"

QVector<toast*> ToastManager::activeToasts;
int ToastManager::m_x;
int ToastManager::m_y;
ToastManager::ToastManager()
{

}

void ToastManager::show(const QString &text)
{
    toast *t = new toast(text);

    // 先把新 toast 加到集合
    activeToasts.append(t);
    repositionAll();
}

void ToastManager::repositionAll()
{
    // 先把底部的从上往下排列
    int y = m_y - 10;
    // 依次向上
    for (int i = activeToasts.size() - 1; i >= 0; --i)
    {
        toast *t = activeToasts.at(i);
        if (!t->isVisible())
        {
            activeToasts.remove(i);// 已被 deleteLater 的会被移除
            continue;
        }
        int w = t->width();
        int h = t->height();
        int x = m_x - w;
        y -= h;
        t->move(x, y);      // 直接设定位置
        y -= 10;            // 10px 间距
    }
}

void ToastManager::moveToPosition(int x, int y)
{
    m_x = x;
    m_y = y;
}
