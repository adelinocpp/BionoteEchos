#include "audio/AudioDecoder.h"
#include "models/AudioFile.h"
#include <QFileInfo>
#include <QDebug>
#include <sndfile.h>

AudioDecoder::AudioDecoder(QObject *parent) 
    : QObject(parent) 
{
}

AudioDecoder::~AudioDecoder() 
{
}

bool AudioDecoder::decode(const QString &filePath, std::shared_ptr<AudioFile> audioFile)
{
    if (!audioFile) {
        m_lastError = tr("Objeto AudioFile inválido");
        emit error(m_lastError);
        return false;
    }
    
    if (filePath.isEmpty()) {
        m_lastError = tr("Caminho do arquivo vazio");
        emit error(m_lastError);
        return false;
    }
    
    QFileInfo fileInfo(filePath);
    if (!fileInfo.exists()) {
        m_lastError = tr("Arquivo não encontrado: %1").arg(filePath);
        emit error(m_lastError);
        return false;
    }
    
    emit decodingStarted(filePath);
    
    // Abrir arquivo com libsndfile
    SF_INFO sfInfo;
    memset(&sfInfo, 0, sizeof(sfInfo));
    
    SNDFILE* sndFile = sf_open(filePath.toUtf8().constData(), SFM_READ, &sfInfo);
    
    if (!sndFile) {
        m_lastError = tr("Erro ao abrir arquivo: %1").arg(sf_strerror(nullptr));
        emit error(m_lastError);
        emit decodingFinished(false);
        return false;
    }
    
    // Preencher metadados
    audioFile->setFilePath(filePath);
    audioFile->setSampleRate(sfInfo.samplerate);
    audioFile->setNumChannels(sfInfo.channels);
    audioFile->setNumSamples(sfInfo.frames);
    audioFile->setDuration(static_cast<double>(sfInfo.frames) / sfInfo.samplerate);
    audioFile->setFileSize(fileInfo.size());
    
    // Determinar codec baseado no formato
    QString codec;
    switch (sfInfo.format & SF_FORMAT_TYPEMASK) {
        case SF_FORMAT_WAV:   codec = "WAV"; break;
        case SF_FORMAT_FLAC:  codec = "FLAC"; break;
        case SF_FORMAT_OGG:   codec = "OGG"; break;
        case SF_FORMAT_AIFF:  codec = "AIFF"; break;
        default:              codec = "Unknown"; break;
    }
    audioFile->setCodec(codec);
    
    // Determinar bit depth
    int bitDepth = 16; // padrão
    switch (sfInfo.format & SF_FORMAT_SUBMASK) {
        case SF_FORMAT_PCM_S8:
        case SF_FORMAT_PCM_U8:
            bitDepth = 8;
            break;
        case SF_FORMAT_PCM_16:
            bitDepth = 16;
            break;
        case SF_FORMAT_PCM_24:
            bitDepth = 24;
            break;
        case SF_FORMAT_PCM_32:
        case SF_FORMAT_FLOAT:
            bitDepth = 32;
            break;
        case SF_FORMAT_DOUBLE:
            bitDepth = 64;
            break;
    }
    audioFile->setBitDepth(bitDepth);
    
    // Ler amostras
    const int bufferSize = 8192;
    QVector<float> interleavedBuffer(bufferSize);
    QVector<QVector<float>> channelBuffers(sfInfo.channels);
    
    for (int ch = 0; ch < sfInfo.channels; ++ch) {
        channelBuffers[ch].reserve(sfInfo.frames);
    }
    
    sf_count_t totalRead = 0;
    sf_count_t framesRead;
    
    while ((framesRead = sf_readf_float(sndFile, interleavedBuffer.data(), 
                                         bufferSize / sfInfo.channels)) > 0) {
        // Desentrelaçar canais
        for (sf_count_t frame = 0; frame < framesRead; ++frame) {
            for (int ch = 0; ch < sfInfo.channels; ++ch) {
                channelBuffers[ch].append(interleavedBuffer[frame * sfInfo.channels + ch]);
            }
        }
        
        totalRead += framesRead;
        
        // Emitir progresso
        int progress = static_cast<int>((totalRead * 100) / sfInfo.frames);
        emit decodingProgress(progress);
    }
    
    // Armazenar amostras no AudioFile
    for (int ch = 0; ch < sfInfo.channels; ++ch) {
        audioFile->setSamples(ch, channelBuffers[ch]);
    }
    
    sf_close(sndFile);
    
    emit decodingProgress(100);
    emit decodingFinished(true);
    
    qDebug() << "Arquivo decodificado:" << filePath;
    qDebug() << "  Canais:" << sfInfo.channels;
    qDebug() << "  Taxa de amostragem:" << sfInfo.samplerate << "Hz";
    qDebug() << "  Amostras:" << sfInfo.frames;
    qDebug() << "  Duração:" << audioFile->getDuration() << "s";
    
    return true;
}

bool AudioDecoder::getInfo(const QString &filePath, std::shared_ptr<AudioFile> audioFile)
{
    if (!audioFile) {
        m_lastError = tr("Objeto AudioFile inválido");
        return false;
    }
    
    QFileInfo fileInfo(filePath);
    if (!fileInfo.exists()) {
        m_lastError = tr("Arquivo não encontrado: %1").arg(filePath);
        return false;
    }
    
    SF_INFO sfInfo;
    memset(&sfInfo, 0, sizeof(sfInfo));
    
    SNDFILE* sndFile = sf_open(filePath.toUtf8().constData(), SFM_READ, &sfInfo);
    
    if (!sndFile) {
        m_lastError = tr("Erro ao abrir arquivo: %1").arg(sf_strerror(nullptr));
        return false;
    }
    
    // Preencher apenas metadados
    audioFile->setFilePath(filePath);
    audioFile->setSampleRate(sfInfo.samplerate);
    audioFile->setNumChannels(sfInfo.channels);
    audioFile->setNumSamples(sfInfo.frames);
    audioFile->setDuration(static_cast<double>(sfInfo.frames) / sfInfo.samplerate);
    audioFile->setFileSize(fileInfo.size());
    
    QString codec;
    switch (sfInfo.format & SF_FORMAT_TYPEMASK) {
        case SF_FORMAT_WAV:   codec = "WAV"; break;
        case SF_FORMAT_FLAC:  codec = "FLAC"; break;
        case SF_FORMAT_OGG:   codec = "OGG"; break;
        case SF_FORMAT_AIFF:  codec = "AIFF"; break;
        default:              codec = "Unknown"; break;
    }
    audioFile->setCodec(codec);
    
    sf_close(sndFile);
    
    return true;
}

bool AudioDecoder::isFormatSupported(const QString &filePath)
{
    QFileInfo fileInfo(filePath);
    QString extension = fileInfo.suffix().toLower();
    
    QStringList supported = getSupportedFormats();
    return supported.contains(extension);
}

QStringList AudioDecoder::getSupportedFormats()
{
    return QStringList() 
        << "wav" << "flac" << "ogg" << "opus" 
        << "aiff" << "aif" << "au" << "snd"
        << "voc" << "w64" << "mat" << "pvf"
        << "htk" << "sds" << "avr" << "ircam"
        << "sf" << "caf" << "wve" << "paf";
}

QString AudioDecoder::getFileDialogFilter()
{
    return tr("Arquivos de Áudio (*.wav *.flac *.ogg *.opus *.aiff *.aif *.au *.snd);;"
              "WAV (*.wav);;"
              "FLAC (*.flac);;"
              "OGG (*.ogg *.opus);;"
              "AIFF (*.aiff *.aif);;"
              "Todos os Arquivos (*)");
}
