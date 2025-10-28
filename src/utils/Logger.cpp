#include "utils/Logger.h"
#include <QDir>
#include <QDebug>
#include <cstdio>
#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>

Logger& Logger::instance()
{
    static Logger instance;
    return instance;
}

Logger::Logger()
{
    // Criar diretório logs usando mkdir
    mkdir("logs", 0755);
    
    printf("Sistema de logs inicializado\n");
    fflush(stdout);
    
    // Testar criação de arquivos
    FILE *f = fopen("logs/player.log", "w");
    if (f) {
        fprintf(f, "=== Log do AudioPlayer ===\n");
        fflush(f);
        fclose(f);
        printf("Arquivo logs/player.log criado\n");
    } else {
        printf("ERRO: Não foi possível criar logs/player.log\n");
    }
    
    f = fopen("logs/audio.log", "w");
    if (f) {
        fprintf(f, "=== Log de Áudio/Visualização ===\n");
        fflush(f);
        fclose(f);
        printf("Arquivo logs/audio.log criado\n");
    } else {
        printf("ERRO: Não foi possível criar logs/audio.log\n");
    }
    
    f = fopen("logs/app.log", "w");
    if (f) {
        fprintf(f, "=== Log da Aplicação ===\n");
        fflush(f);
        fclose(f);
        printf("Arquivo logs/app.log criado\n");
    } else {
        printf("ERRO: Não foi possível criar logs/app.log\n");
    }
    
    fflush(stdout);
}

Logger::~Logger()
{
}

void Logger::log(const QString &message)
{
    writeToFile("logs/app.log", message);
}

void Logger::logAudio(const QString &message)
{
    writeToFile("logs/audio.log", message);
}

void Logger::logPlayer(const QString &message)
{
    writeToFile("logs/player.log", message);
}

void Logger::writeToFile(const QString &filename, const QString &message)
{
    QMutexLocker locker(&m_mutex);
    
    // Usar FILE* e fprintf para garantir gravação
    FILE *f = fopen(filename.toUtf8().constData(), "a");
    if (f) {
        // Timestamp
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        char timestamp[64];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", t);
        
        // Escrever no arquivo
        fprintf(f, "[%s] %s\n", timestamp, message.toUtf8().constData());
        fflush(f);
        fclose(f);
    } else {
        printf("ERRO ao abrir %s\n", filename.toUtf8().constData());
    }
    
    // Também imprimir no console
    printf("%s\n", message.toUtf8().constData());
    fflush(stdout);
}
