#include "DailyLogger.h"
#include <QDebug>

DailyLogger* DailyLogger::m_instance = NULL;
QMutex DailyLogger::m_instanceMutex;

static void cleanupDailyLogger()
{
	DailyLogger::cleanup();
}

void DailyLogger::cleanup()
{
	QMutexLocker locker(&m_instanceMutex);
	delete m_instance;
	m_instance = NULL;
}

DailyLogger* DailyLogger::instance()
{
	QMutexLocker locker(&m_instanceMutex);
	if (!m_instance) {
		m_instance = new DailyLogger();
		qAddPostRoutine(cleanupDailyLogger);
	}
	return m_instance;
}

void DailyLogger::log(const QString &message)
{
	instance()->writeLog(message);
}

void DailyLogger::setLogDir(const QString &logDirPath)
{
	QMutexLocker locker(&m_instanceMutex);
	if (!m_instance) {
		m_instance = new DailyLogger();
		qAddPostRoutine(cleanupDailyLogger);
	}
	m_instance->m_logDirPath = logDirPath;

	// 确保日志目录存在
	QDir logDir(logDirPath);
	if (!logDir.exists()) {
		logDir.mkpath(".");
	}

	// 重新打开日志文件
	QMutexLocker fileLocker(&m_instance->m_mutex);
	if (m_instance->m_logFile.isOpen()) {
		m_instance->m_logFile.close();
	}
	m_instance->openLogFile();
}

DailyLogger::DailyLogger(QObject *parent)
	: QObject(parent), m_currentDate(QDate::currentDate())
{
	// 默认日志目录为应用程序目录下的logs文件夹
	m_logDirPath = QDir::currentPath()+ "/../var/log/davc_monitor_cal";

	// 确保日志目录存在
	QDir logDir(m_logDirPath);
	if (!logDir.exists()) {
		logDir.mkpath(".");
	}

	openLogFile();
}

DailyLogger::~DailyLogger()
{
	QMutexLocker locker(&m_mutex);
	if (m_logFile.isOpen()) {
		m_logFile.close();
	}
}

void DailyLogger::writeLog(const QString &message)
{
	QMutexLocker locker(&m_mutex);
	checkDate();

	if (m_logFile.isOpen()) {
		QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
		m_logStream << "[" << timestamp << "] " << message << "\n";
		m_logStream.flush(); // 确保立即写入文件
	}
}

void DailyLogger::checkDate()
{
	QDate today = QDate::currentDate();
	if (today != m_currentDate) {
		m_currentDate = today;
		if (m_logFile.isOpen()) {
			m_logFile.close();
		}
		openLogFile();
	}
}

void DailyLogger::openLogFile()
{
	QString fileName = m_currentDate.toString("yyyy-MM-dd") + ".log";
	QString filePath = QDir(m_logDirPath).filePath(fileName);
	
	m_logFile.setFileName(filePath);
	if (!m_logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
		qWarning() << "Failed to open log file:" << filePath;
		return;
	}

	m_logStream.setDevice(&m_logFile);
	// 写入日志文件头
	QString header = QString("\n===== Log started at %1 =====\n")
		.arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
	m_logStream << header;
	m_logStream.flush();
}