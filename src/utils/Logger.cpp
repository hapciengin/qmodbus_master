#include "Logger.h"
#include <QDebug>
#include <QDir>
#include <QDateTime>
#include <QFileInfo>

// Singleton instance
Logger* Logger::instance_ = nullptr;

Logger& Logger::instance()
{
    if (!instance_) {
        instance_ = new Logger();
    }
    return *instance_;
}

Logger::Logger(QObject* parent)
    : QObject(parent)
    , maxLogSize(10 * 1024 * 1024)  // 10MB varsayılan
    , maxLogFiles(5)
    , minimumLevel(Level::INFO)
    , consoleOutput(true)
    , fileOutput(false)
    , includeTimestamp(true)
    , includeLevel(true)
    , includeContext(true)
{
    // Log buffer'ını temizle
    logBuffer.clear();
}

Logger::~Logger()
{
    flush();
    if (logFile.isOpen()) {
        logFile.close();
    }
}

bool Logger::setLogFile(const QString& filename)
{
    QMutexLocker locker(&mutex);

    // Eğer açık bir dosya varsa kapat
    if (logFile.isOpen()) {
        logFile.close();
    }

    currentLogFile = filename;
    logFile.setFileName(filename);

    // Dosyayı aç
    if (!logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        emit logFileError(tr("Failed to open log file: %1").arg(filename));
        return false;
    }

    logStream.setDevice(&logFile);
    fileOutput = true;
    emit logFileChanged(filename);
    return true;
}

QString Logger::getLogFile() const
{
    QMutexLocker locker(&mutex);
    return currentLogFile;
}

void Logger::closeLogFile()
{
    QMutexLocker locker(&mutex);
    if (logFile.isOpen()) {
        logFile.close();
    }
    fileOutput = false;
}

void Logger::setMaxLogSize(qint64 bytes)
{
    QMutexLocker locker(&mutex);
    maxLogSize = bytes;
}

void Logger::setMaxLogFiles(int count)
{
    QMutexLocker locker(&mutex);
    maxLogFiles = count;
}

void Logger::setLogLevel(Level level)
{
    QMutexLocker locker(&mutex);
    minimumLevel = level;
}

void Logger::setConsoleOutput(bool enabled)
{
    QMutexLocker locker(&mutex);
    consoleOutput = enabled;
}

void Logger::setFileOutput(bool enabled)
{
    QMutexLocker locker(&mutex);
    fileOutput = enabled;
}

void Logger::setIncludeTimestamp(bool include)
{
    QMutexLocker locker(&mutex);
    includeTimestamp = include;
}

void Logger::setIncludeLevel(bool include)
{
    QMutexLocker locker(&mutex);
    includeLevel = include;
}

void Logger::setIncludeContext(bool include)
{
    QMutexLocker locker(&mutex);
    includeContext = include;
}

void Logger::debug(const QString& message, const QString& context)
{
    writeLog(Level::DEBUG, message, context);
}

void Logger::info(const QString& message, const QString& context)
{
    writeLog(Level::INFO, message, context);
}

void Logger::warning(const QString& message, const QString& context)
{
    writeLog(Level::WARNING, message, context);
}

void Logger::error(const QString& message, const QString& context)
{
    writeLog(Level::ERROR, message, context);
}

void Logger::critical(const QString& message, const QString& context)
{
    writeLog(Level::CRITICAL, message, context);
}

void Logger::flush()
{
    QMutexLocker locker(&mutex);
    
    if (fileOutput && logFile.isOpen()) {
        logStream.flush();
        logFile.flush();
    }
}

void Logger::clear()
{
    QMutexLocker locker(&mutex);
    logBuffer.clear();
    logCounts.clear();
    lastLogTime = QDateTime();
    lastLogMessage.clear();
}

int Logger::getTotalLogCount() const
{
    QMutexLocker locker(&mutex);
    int total = 0;
    for (const auto& count : logCounts) {
        total += count;
    }
    return total;
}

int Logger::getLogCount(Level level) const
{
    QMutexLocker locker(&mutex);
    return logCounts.value(level, 0);
}

QDateTime Logger::getLastLogTime() const
{
    QMutexLocker locker(&mutex);
    return lastLogTime;
}

QString Logger::getLastLogMessage() const
{
    QMutexLocker locker(&mutex);
    return lastLogMessage;
}

void Logger::writeLog(Level level, const QString& message, const QString& context)
{
    if (level < minimumLevel) {
        return;
    }

    QMutexLocker locker(&mutex);

    // Log girişini oluştur
    LogEntry entry;
    entry.level = level;
    entry.message = message;
    entry.context = context;
    entry.timestamp = QDateTime::currentDateTime();

    // Buffer'a ekle
    logBuffer.enqueue(entry);
    if (logBuffer.size() > MAX_BUFFER_SIZE) {
        logBuffer.dequeue();
    }

    // İstatistikleri güncelle
    logCounts[level]++;
    lastLogTime = entry.timestamp;
    lastLogMessage = message;

    // Log mesajını formatla
    QString formattedMessage = formatLogMessage(entry);

    // Konsola yaz
    if (consoleOutput) {
        writeToConsole(entry);
    }

    // Dosyaya yaz
    if (fileOutput) {
        writeToFile(entry);
    }

    // Sinyali gönder
    emit messageLogged(level, message, context, entry.timestamp);
}

void Logger::writeToFile(const LogEntry& entry)
{
    if (!logFile.isOpen()) {
        return;
    }

    // Dosya boyutunu kontrol et
    if (checkLogFileSize()) {
        rotateLogFiles();
    }

    logStream << formatLogMessage(entry) << Qt::endl;
    logStream.flush();
}

void Logger::writeToConsole(const LogEntry& entry)
{
    QString message = formatLogMessage(entry);
    
    switch (entry.level) {
        case Level::DEBUG:
            qDebug().noquote() << message;
            break;
        case Level::INFO:
            qInfo().noquote() << message;
            break;
        case Level::WARNING:
            qWarning().noquote() << message;
            break;
        case Level::ERROR:
            qCritical().noquote() << message;
            break;
        case Level::CRITICAL:
            qCritical().noquote() << message;
            break;
    }
}

QString Logger::formatLogMessage(const LogEntry& entry) const
{
    QString message;

    if (includeTimestamp) {
        message += entry.timestamp.toString("yyyy-MM-dd hh:mm:ss.zzz") + " ";
    }

    if (includeLevel) {
        message += "[" + getLevelString(entry.level) + "] ";
    }

    if (includeContext && !entry.context.isEmpty()) {
        message += entry.context + ": ";
    }

    message += entry.message;
    return message;
}

QString Logger::getLevelString(Level level) const
{
    switch (level) {
        case Level::DEBUG:    return "DEBUG";
        case Level::INFO:     return "INFO";
        case Level::WARNING:  return "WARNING";
        case Level::ERROR:    return "ERROR";
        case Level::CRITICAL: return "CRITICAL";
        default:             return "UNKNOWN";
    }
}

void Logger::rotateLogFiles()
{
    if (currentLogFile.isEmpty()) {
        return;
    }

    // Mevcut log dosyasını kapat
    logFile.close();

    // Dosya rotasyonu yap
    QFileInfo fileInfo(currentLogFile);
    QString baseName = fileInfo.absolutePath() + "/" + fileInfo.baseName();
    QString suffix = fileInfo.suffix();

    // Eski log dosyalarını kaydır
    for (int i = maxLogFiles - 1; i >= 0; --i) {
        QString oldName = QString("%1.%2.%3").arg(baseName).arg(i).arg(suffix);
        QString newName = QString("%1.%2.%3").arg(baseName).arg(i + 1).arg(suffix);
        
        if (QFile::exists(oldName)) {
            if (i == maxLogFiles - 1) {
                QFile::remove(oldName);
            } else {
                QFile::rename(oldName, newName);
            }
        }
    }

    // Mevcut dosyayı .1 olarak yeniden adlandır
    QFile::rename(currentLogFile, QString("%1.1.%2").arg(baseName).arg(suffix));

    // Yeni log dosyası aç
    logFile.setFileName(currentLogFile);
    logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    logStream.setDevice(&logFile);
}

bool Logger::checkLogFileSize()
{
    if (!logFile.isOpen()) {
        return false;
    }

    return (logFile.size() >= maxLogSize);
}