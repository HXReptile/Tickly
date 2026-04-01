#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QJsonDocument>
#include <common_utils.h>

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

    DailyLogger::setLogDir(LOG_PATH);

    MainWindow w;
    // w.show();
    return a.exec();
}
