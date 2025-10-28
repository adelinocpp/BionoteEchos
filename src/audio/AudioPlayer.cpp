#include "audio/AudioPlayer.h"
#include "models/AudioFile.h"
#include <QUrl>
#include <QDebug>

AudioPlayer::AudioPlayer(QObject *parent) 
    : QObject(parent)
    , m_player(nullptr)
    , m_audioOutput(nullptr)
    , m_loopEnabled(false)
{
    // Criar player e output de áudio
    m_player = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    m_player->setAudioOutput(m_audioOutput);
    
    // Conectar sinais
    connect(m_player, &QMediaPlayer::positionChanged,
            this, &AudioPlayer::onPositionChanged);
    connect(m_player, &QMediaPlayer::durationChanged,
            this, &AudioPlayer::onDurationChanged);
    connect(m_player, &QMediaPlayer::playbackStateChanged,
            this, &AudioPlayer::onPlaybackStateChanged);
    connect(m_player, &QMediaPlayer::mediaStatusChanged,
            this, &AudioPlayer::onMediaStatusChanged);
    connect(m_player, &QMediaPlayer::errorOccurred,
            this, &AudioPlayer::onErrorOccurred);
    
    // Volume padrão: 50%
    m_audioOutput->setVolume(0.5f);
}

AudioPlayer::~AudioPlayer()
{
    stop();
}

void AudioPlayer::setAudioFile(std::shared_ptr<AudioFile> audioFile)
{
    stop();
    m_audioFile = audioFile;
    
    if (m_audioFile) {
        // Carregar arquivo no player
        QString filePath = m_audioFile->getFilePath();
        QUrl fileUrl = QUrl::fromLocalFile(filePath);
        
        m_player->setSource(fileUrl);
        
        qDebug() << "AudioPlayer: Arquivo carregado:" << filePath;
    } else {
        m_player->setSource(QUrl());
    }
}

void AudioPlayer::play()
{
    if (m_audioFile) {
        m_player->play();
        qDebug() << "AudioPlayer: Play";
    }
}

void AudioPlayer::pause()
{
    m_player->pause();
    qDebug() << "AudioPlayer: Pause";
}

void AudioPlayer::stop()
{
    m_player->stop();
    qDebug() << "AudioPlayer: Stop";
}

bool AudioPlayer::isPlaying() const
{
    return m_player->playbackState() == QMediaPlayer::PlayingState;
}

void AudioPlayer::setPosition(qint64 positionMs)
{
    m_player->setPosition(positionMs);
}

qint64 AudioPlayer::getPosition() const
{
    return m_player->position();
}

qint64 AudioPlayer::getDuration() const
{
    return m_player->duration();
}

void AudioPlayer::setVolume(float volume)
{
    // Garantir que está no intervalo [0.0, 1.0]
    volume = qMax(0.0f, qMin(volume, 1.0f));
    m_audioOutput->setVolume(volume);
}

float AudioPlayer::getVolume() const
{
    return m_audioOutput->volume();
}

void AudioPlayer::setLoop(bool loop)
{
    m_loopEnabled = loop;
    qDebug() << "AudioPlayer: Loop" << (loop ? "ativado" : "desativado");
}

bool AudioPlayer::isLooping() const
{
    return m_loopEnabled;
}

void AudioPlayer::onPositionChanged(qint64 position)
{
    emit positionChanged(position);
}

void AudioPlayer::onDurationChanged(qint64 duration)
{
    emit durationChanged(duration);
}

void AudioPlayer::onPlaybackStateChanged(QMediaPlayer::PlaybackState state)
{
    emit playbackStateChanged(state);
    
    if (state == QMediaPlayer::StoppedState) {
        qDebug() << "AudioPlayer: Reprodução parada";
    } else if (state == QMediaPlayer::PlayingState) {
        qDebug() << "AudioPlayer: Reproduzindo";
    } else if (state == QMediaPlayer::PausedState) {
        qDebug() << "AudioPlayer: Pausado";
    }
}

void AudioPlayer::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia) {
        emit playbackFinished();
        
        if (m_loopEnabled) {
            // Reiniciar do início
            m_player->setPosition(0);
            m_player->play();
            qDebug() << "AudioPlayer: Loop - reiniciando";
        } else {
            qDebug() << "AudioPlayer: Fim da reprodução";
        }
    } else if (status == QMediaPlayer::LoadedMedia) {
        qDebug() << "AudioPlayer: Mídia carregada";
    } else if (status == QMediaPlayer::InvalidMedia) {
        qDebug() << "AudioPlayer: Mídia inválida";
    }
}

void AudioPlayer::onErrorOccurred(QMediaPlayer::Error error, const QString &errorString)
{
    QString errorMsg;
    
    switch (error) {
        case QMediaPlayer::NoError:
            return;
        case QMediaPlayer::ResourceError:
            errorMsg = "Erro de recurso: " + errorString;
            break;
        case QMediaPlayer::FormatError:
            errorMsg = "Erro de formato: " + errorString;
            break;
        case QMediaPlayer::NetworkError:
            errorMsg = "Erro de rede: " + errorString;
            break;
        case QMediaPlayer::AccessDeniedError:
            errorMsg = "Acesso negado: " + errorString;
            break;
        default:
            errorMsg = "Erro desconhecido: " + errorString;
            break;
    }
    
    qDebug() << "AudioPlayer: ERRO -" << errorMsg;
    emit errorOccurred(errorMsg);
}
