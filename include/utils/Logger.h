#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMutex>

/**
 * @brief Sistema de logging para arquivo
 */
class Logger
{
public:
    static Logger& instance();
    
    void log(const QString &message);
    void logAudio(const QString &message);
    void logPlayer(const QString &message);
    
private:
    Logger();
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    void writeToFile(const QString &filename, const QString &message);
    
    QMutex m_mutex;
};

// Macros para facilitar uso
#define LOG(msg) Logger::instance().log(QString("[%1:%2] %3").arg(__FILE__).arg(__LINE__).arg(msg))
#define LOG_AUDIO(msg) Logger::instance().logAudio(QString("[%1:%2] %3").arg(__FILE__).arg(__LINE__).arg(msg))
#define LOG_PLAYER(msg) Logger::instance().logPlayer(QString("[%1:%2] %3").arg(__FILE__).arg(__LINE__).arg(msg))

#endif // LOGGER_H
