#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QJsonDocument>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<ItemInfo>("ItemInfo");

    // QFile file(STYLE_PATH);
    // if (!file.open(QIODevice::ReadOnly))
    //     return -1;
    // QByteArray data = file.readAll();
    // file.close();
    // QJsonDocument doc = QJsonDocument::fromJson(data);
    // if (!doc.isObject())
    //     return -1;
    // QString qss = doc.toVariant().toString();
    // qApp->setStyleSheet(qss);

    MainWindow w;
    // w.show();
    return a.exec();
}
