#include "audio/AudioPlayer.h"
#include "models/AudioFile.h"
#include "utils/Logger.h"
#include <QUrl>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

AudioPlayer::AudioPlayer(QObject *parent) 
    : QObject(parent)
    , m_player(nullptr)
    , m_audioOutput(nullptr)
    , m_loopEnabled(false)
    , m_hasPlaybackRegion(false)
    , m_regionStartMs(0)
    , m_regionEndMs(0)
    , m_isRepositioning(false)
    , m_lastPosition(-1)
{
    LOG_PLAYER("AudioPlayer construtor iniciado");
    
    // Criar player e output de áudio
    m_player = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    m_player->setAudioOutput(m_audioOutput);
    
    LOG_PLAYER("QMediaPlayer e QAudioOutput criados");
    
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
    m_audioFile = audioFile;
    
    if (audioFile) {
        QString filePath = audioFile->getFilePath();
        QUrl fileUrl = QUrl::fromLocalFile(filePath);
        
        LOG_PLAYER(QString("Carregando arquivo: %1").arg(filePath));
        
        // Parar reprodução anterior
        m_player->stop();
        
        // Carregar novo arquivo
        m_player->setSource(fileUrl);
        
        // Pré-carregar para reduzir latência inicial
        m_player->setPosition(0);
        
        LOG_PLAYER("Arquivo carregado e pré-bufferizado");
        qDebug() << "AudioPlayer: Arquivo carregado:" << filePath;
    } else {
        m_player->setSource(QUrl());
    }
}

void AudioPlayer::play()
{
    if (!m_audioFile) {
        LOG_PLAYER("ERRO: play() chamado sem arquivo de áudio");
        return;
    }
    
    QMediaPlayer::PlaybackState currentState = m_player->playbackState();
    QMediaPlayer::MediaStatus mediaStatus = m_player->mediaStatus();
    
    LOG_PLAYER(QString("play() - Estado: %1, MediaStatus: %2, Região: %3, Loop: %4")
        .arg(currentState).arg(mediaStatus).arg(m_hasPlaybackRegion).arg(m_loopEnabled));
    
    // Verificar se mídia está carregada
    if (mediaStatus == QMediaPlayer::NoMedia || mediaStatus == QMediaPlayer::InvalidMedia) {
        LOG_PLAYER("ERRO: Mídia não carregada ou inválida");
        return;
    }
    
    // Se há região definida e não está tocando, posicionar no início da região
    if (m_hasPlaybackRegion && currentState != QMediaPlayer::PlayingState) {
        qint64 currentPos = m_player->position();
        LOG_PLAYER(QString("Posição atual: %1ms, Região: %2ms - %3ms")
            .arg(currentPos).arg(m_regionStartMs).arg(m_regionEndMs));
        
        // Se está fora da região, voltar ao início
        if (currentPos < m_regionStartMs || currentPos >= m_regionEndMs) {
            LOG_PLAYER(QString("PRECISA reposicionar de %1ms para %2ms").arg(currentPos).arg(m_regionStartMs));
            
            // IMPORTANTE: Só reposicionar se mídia está pronta
            if (mediaStatus == QMediaPlayer::LoadedMedia || mediaStatus == QMediaPlayer::BufferedMedia) {
                m_player->setPosition(m_regionStartMs);
                m_lastPosition = m_regionStartMs;
                LOG_PLAYER("Reposicionamento OK");
            } else {
                LOG_PLAYER(QString("AVISO: Não pode reposicionar, MediaStatus: %1").arg(mediaStatus));
            }
        } else {
            LOG_PLAYER("Posição OK, não precisa reposicionar");
        }
    }
    
    LOG_PLAYER("Chamando m_player->play()...");
    m_player->play();
    LOG_PLAYER("play() retornou. Aguardando mudança de estado...");
}

void AudioPlayer::pause()
{
    LOG_PLAYER(QString("pause() - Estado antes: %1").arg(m_player->playbackState()));
    m_player->pause();
    LOG_PLAYER("pause() - Comando enviado");
}

void AudioPlayer::stop()
{
    // Log com informação de quem chamou
    LOG_PLAYER(QString(">>> stop() CHAMADO - Estado antes: %1, Loop: %2, Região: %3")
        .arg(m_player->playbackState())
        .arg(m_loopEnabled ? "SIM" : "NÃO")
        .arg(m_hasPlaybackRegion ? "SIM" : "NÃO"));
    
    m_player->stop();
    LOG_PLAYER(">>> stop() EXECUTADO");
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
    LOG_PLAYER(QString("setLoop(%1) - Tem região: %2").arg(loop).arg(m_hasPlaybackRegion));
}

bool AudioPlayer::isLooping() const
{
    return m_loopEnabled;
}

void AudioPlayer::onPositionChanged(qint64 position)
{
    // Proteção contra loop infinito: ignorar se estamos reposicionando
    if (m_isRepositioning) {
        LOG_PLAYER(QString("onPositionChanged IGNORADO (reposicionando): %1ms").arg(position));
        return;
    }
    
    // Proteção adicional: ignorar se posição não mudou significativamente
    if (qAbs(position - m_lastPosition) < 10) {
        return;
    }
    
    // Log de posição apenas a cada 500ms para reduzir verbosidade
    if (position % 500 < 60) {
        LOG_PLAYER(QString("Pos: %1ms").arg(position));
    }
    
    m_lastPosition = position;
    
    // Emitir sinal para UI
    emit positionChanged(position);
    
    // Verificar se ultrapassou o fim da região
    if (m_hasPlaybackRegion && m_player->playbackState() == QMediaPlayer::PlayingState) {
        if (position >= m_regionEndMs) {
            m_isRepositioning = true; // Ativar flag
            
            LOG_PLAYER(QString(">>> DETECTADO FIM DE REGIÃO <<<"));
            LOG_PLAYER(QString("    Posição atual: %1ms").arg(position));
            LOG_PLAYER(QString("    Fim da região: %2ms").arg(m_regionEndMs));
            LOG_PLAYER(QString("    Loop ativo: %1").arg(m_loopEnabled ? "SIM" : "NÃO"));
            
            if (m_loopEnabled) {
                // Voltar ao início da região SEM pausar
                LOG_PLAYER(QString(">>> LOOP: %1ms -> %2ms").arg(position).arg(m_regionStartMs));
                m_player->setPosition(m_regionStartMs);
                m_lastPosition = m_regionStartMs;
                LOG_PLAYER(QString(">>> LOOP OK - Continuando reprodução"));
            } else {
                // Parar reprodução
                LOG_PLAYER(QString(">>> SEM LOOP: Parando reprodução"));
                m_player->pause();
                m_player->setPosition(m_regionStartMs);
                m_lastPosition = m_regionStartMs;
                LOG_PLAYER(QString(">>> REPRODUÇÃO PARADA"));
            }
            
            m_isRepositioning = false; // Desativar flag
        }
    }
}

void AudioPlayer::onDurationChanged(qint64 duration)
{
    emit durationChanged(duration);
}

void AudioPlayer::onPlaybackStateChanged(QMediaPlayer::PlaybackState state)
{
    QString stateStr;
    if (state == QMediaPlayer::StoppedState) {
        stateStr = "STOPPED";
        qDebug() << "AudioPlayer: Reprodução parada";
    } else if (state == QMediaPlayer::PlayingState) {
        stateStr = "PLAYING";
        qDebug() << "AudioPlayer: Reproduzindo";
    } else if (state == QMediaPlayer::PausedState) {
        stateStr = "PAUSED";
        qDebug() << "AudioPlayer: Pausado";
    }
    
    LOG_PLAYER(QString("Estado mudou para: %1 (Loop: %2, Região: %3)")
        .arg(stateStr)
        .arg(m_loopEnabled ? "SIM" : "NÃO")
        .arg(m_hasPlaybackRegion ? "SIM" : "NÃO"));
    
    emit playbackStateChanged(state);
}

void AudioPlayer::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    LOG_PLAYER(QString("MediaStatus mudou para: %1").arg(status));
    
    if (status == QMediaPlayer::EndOfMedia) {
        LOG_PLAYER("EndOfMedia detectado");
        emit playbackFinished();
        
        // Se tem região, o loop é tratado em onPositionChanged
        // Aqui só trata loop de arquivo completo
        if (m_loopEnabled && !m_hasPlaybackRegion) {
            LOG_PLAYER("Loop de arquivo completo - reiniciando");
            m_player->setPosition(0);
            m_player->play();
        } else {
            LOG_PLAYER("Fim da reprodução");
        }
    } else if (status == QMediaPlayer::LoadedMedia) {
        LOG_PLAYER("Mídia carregada com sucesso");
    } else if (status == QMediaPlayer::InvalidMedia) {
        LOG_PLAYER("ERRO: Mídia inválida");
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

void AudioPlayer::setPlaybackRegion(qint64 startMs, qint64 endMs)
{
    m_hasPlaybackRegion = true;
    m_regionStartMs = startMs;
    m_regionEndMs = endMs;
    LOG_PLAYER(QString("Região definida: %1ms - %2ms (duração: %3ms)")
        .arg(startMs).arg(endMs).arg(endMs - startMs));
}

void AudioPlayer::clearPlaybackRegion()
{
    m_hasPlaybackRegion = false;
    m_regionStartMs = 0;
    m_regionEndMs = 0;
    LOG_PLAYER("Região de reprodução limpa");
}

bool AudioPlayer::hasPlaybackRegion() const
{
    return m_hasPlaybackRegion;
}
