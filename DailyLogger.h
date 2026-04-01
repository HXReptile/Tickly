#ifndef DAILYLOGGER_H
#define DAILYLOGGER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDir>
#include <QMutex>
#include <QCoreApplication>

class DailyLogger : public QObject
{
	Q_OBJECT
public:
	static DailyLogger* instance();
	static void log(const QString &message);
	static void setLogDir(const QString &logDirPath);
	static void cleanup();  // 添加公共的清理接口

private:
	explicit DailyLogger(QObject *parent = NULL);
	~DailyLogger();

	void writeLog(const QString &message);
	void checkDate();
	void openLogFile();

	QFile m_logFile;
	QTextStream m_logStream;
	QString m_logDirPath;
	QDate m_currentDate;
	QMutex m_mutex;

	static DailyLogger* m_instance;
	static QMutex m_instanceMutex;
};

#endif // DAILYLOGGER_H