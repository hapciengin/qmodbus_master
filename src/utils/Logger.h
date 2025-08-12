#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QMutex>
#include <QQueue>
#include <memory>

class Logger : public QObject {
    Q_OBJECT

public:
    // Log seviyeleri
    enum class Level {
        DEBUG,      // Geliştirme aşamasında detaylı bilgi
        INFO,       // Genel bilgilendirme
        WARNING,    // Potansiyel sorunlar
        ERROR,      // Hata durumları
        CRITICAL    // Kritik hatalar
    };

    // Singleton erişimi
    static Logger& instance();

    // Log dosyası yönetimi
    bool setLogFile(const QString& filename);
    QString getLogFile() const;
    void closeLogFile();
    
    // Log ayarları
    void setMaxLogSize(qint64 bytes);
    void setMaxLogFiles(int count);
    void setLogLevel(Level level);
    void setConsoleOutput(bool enabled);
    void setFileOutput(bool enabled);
    void setIncludeTimestamp(bool include);
    void setIncludeLevel(bool include);
    void setIncludeContext(bool include);
    
    // Log buffer yönetimi
    void flush();
    void clear();
    
    // Log istatistikleri
    int getTotalLogCount() const;
    int getLogCount(Level level) const;
    QDateTime getLastLogTime() const;
    QString getLastLogMessage() const;

public slots:
    // Log yazma slot'ları
    void debug(const QString& message, const QString& context = QString());
    void info(const QString& message, const QString& context = QString());
    void warning(const QString& message, const QString& context = QString());
    void error(const QString& message, const QString& context = QString());
    void critical(const QString& message, const QString& context = QString());

signals:
    // Log bildirimleri
    void messageLogged(Level level, const QString& message, 
                      const QString& context, const QDateTime& timestamp);
    void logFileChanged(const QString& filename);
    void logFileError(const QString& error);

protected:
    // Singleton yapısı için protected constructor
    explicit Logger(QObject* parent = nullptr);
    virtual ~Logger();

private:
    // Log girişi yapısı
    struct LogEntry {
        Level level;
        QString message;
        QString context;
        QDateTime timestamp;
    };

    // Dosya yönetimi
    QFile logFile;
    QTextStream logStream;
    QString currentLogFile;
    qint64 maxLogSize;
    int maxLogFiles;
    
    // Log ayarları
    Level minimumLevel;
    bool consoleOutput;
    bool fileOutput;
    bool includeTimestamp;
    bool includeLevel;
    bool includeContext;
    
    // Log buffer'ı
    QQueue<LogEntry> logBuffer;
    static const int MAX_BUFFER_SIZE = 1000;
    
    // İstatistikler
    QMap<Level, int> logCounts;
    QDateTime lastLogTime;
    QString lastLogMessage;
    
    // Thread safety
    mutable QMutex mutex;
    
    // Singleton instance
    static Logger* instance_;
    
    // Yardımcı fonksiyonlar
    void writeLog(Level level, const QString& message, const QString& context);
    void writeToFile(const LogEntry& entry);
    void writeToConsole(const LogEntry& entry);
    QString formatLogMessage(const LogEntry& entry) const;
    QString getLevelString(Level level) const;
    void rotateLogFiles();
    bool checkLogFileSize();
    void updateStatistics(const LogEntry& entry);
    
    // Singleton yapısı için private copy constructor ve assignment operator
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
};

// Global log fonksiyonları
#define LOG_DEBUG(msg, ctx) Logger::instance().debug(msg, ctx)
#define LOG_INFO(msg, ctx) Logger::instance().info(msg, ctx)
#define LOG_WARNING(msg, ctx) Logger::instance().warning(msg, ctx)
#define LOG_ERROR(msg, ctx) Logger::instance().error(msg, ctx)
#define LOG_CRITICAL(msg, ctx) Logger::instance().critical(msg, ctx)

#endif // LOGGER_H