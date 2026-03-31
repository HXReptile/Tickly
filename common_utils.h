#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H
#include <QString>
#include <QVector>
#define ROOT_PATH QApplication::applicationDirPath() + "/"
#define DATA_PATH ROOT_PATH + "data/person_data.json"
#define STYLE_PATH ROOT_PATH + "style/init.json"
#define TIME_FORMAT "HH:mm"
struct TimeRange {
    QString start;
    QString end;
};
struct ItemInfo
{
    QString id;
    int order;
    QString name;
    bool enabled;
    int interval;
    QVector<TimeRange> timevec;
    bool skipSaturdays;
    bool skipSundays;
    bool skipHolidays;
    int notificationType;
    QString message;
    ItemInfo() {
        id = QString();
        order = -1;
        name = QString();
        enabled = true;
        interval = -1;
        timevec = QVector<TimeRange>();
        skipSaturdays = true;
        skipSundays = true;
        skipHolidays = true;
        notificationType = -1;
        message = QString();
    }
};

#endif // COMMON_UTILS_H
