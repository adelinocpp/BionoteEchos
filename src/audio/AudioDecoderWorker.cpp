#include "audio/AudioDecoderWorker.h"
#include "audio/AudioDecoder.h"
#include "models/AudioFile.h"
#include <QDebug>

AudioDecoderWorker::AudioDecoderWorker(QObject *parent)
    : QObject(parent)
{
}

AudioDecoderWorker::~AudioDecoderWorker()
{
}

void AudioDecoderWorker::decodeFile(const QString &filePath, std::shared_ptr<AudioFile> audioFile)
{
    qDebug() << "AudioDecoderWorker: Iniciando decodificação em thread separada:" << filePath;
    
    AudioDecoder decoder;
    
    // Emitir progresso inicial
    emit progressUpdated(0);
    
    bool success = decoder.decode(filePath, audioFile);
    
    // Emitir progresso final
    emit progressUpdated(100);
    
    if (success) {
        qDebug() << "AudioDecoderWorker: Decodificação concluída com sucesso";
        emit decodingFinished(true, QString());
    } else {
        QString error = decoder.getLastError();
        qDebug() << "AudioDecoderWorker: Erro na decodificação:" << error;
        emit decodingFinished(false, error);
    }
}
