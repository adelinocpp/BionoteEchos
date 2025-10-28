#include "audio/CustomAudioPlayer.h"
#include "models/AudioFile.h"
#include "utils/Logger.h"
#include <QTimer>
#include <cstring>
#include <algorithm>

CustomAudioPlayer::CustomAudioPlayer(QObject *parent)
    : QObject(parent)
    , m_stream(nullptr)
    , m_portAudioInitialized(false)
    , m_sampleRate(44100)
    , m_channels(1)
    , m_playPosition(0)
    , m_isPlaying(false)
    , m_isPaused(false)
    , m_volume(1.0f)
    , m_loopEnabled(false)
    , m_hasPlaybackRegion(false)
    , m_regionStartSample(0)
    , m_regionEndSample(0)
{
    LOG_PLAYER("CustomAudioPlayer: Construtor iniciado");
    
    // Inicializar PortAudio
    if (!initializePortAudio()) {
        LOG_PLAYER("ERRO: Falha ao inicializar PortAudio");
        emit errorOccurred("Falha ao inicializar sistema de áudio");
    }
    
    // Timer para atualizar posição na UI
    m_positionTimer = new QTimer(this);
    m_positionTimer->setInterval(50); // 20 Hz
    connect(m_positionTimer, &QTimer::timeout, this, &CustomAudioPlayer::emitPositionUpdate);
    
    LOG_PLAYER("CustomAudioPlayer: Inicializado com sucesso");
}

CustomAudioPlayer::~CustomAudioPlayer()
{
    LOG_PLAYER("CustomAudioPlayer: Destrutor");
    stop();
    terminatePortAudio();
}

bool CustomAudioPlayer::initializePortAudio()
{
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        LOG_PLAYER(QString("ERRO Pa_Initialize: %1").arg(Pa_GetErrorText(err)));
        return false;
    }
    
    m_portAudioInitialized = true;
    LOG_PLAYER(QString("PortAudio inicializado. Versão: %1").arg(Pa_GetVersionText()));
    
    // Listar dispositivos disponíveis
    int numDevices = Pa_GetDeviceCount();
    LOG_PLAYER(QString("Dispositivos de áudio disponíveis: %1").arg(numDevices));
    
    return true;
}

void CustomAudioPlayer::terminatePortAudio()
{
    if (m_stream) {
        Pa_StopStream(m_stream);
        Pa_CloseStream(m_stream);
        m_stream = nullptr;
    }
    
    if (m_portAudioInitialized) {
        Pa_Terminate();
        m_portAudioInitialized = false;
        LOG_PLAYER("PortAudio finalizado");
    }
}

void CustomAudioPlayer::setAudioFile(std::shared_ptr<AudioFile> audioFile)
{
    LOG_PLAYER("setAudioFile chamado");
    
    // Parar reprodução e limpar estado
    m_isPlaying = false;
    m_isPaused = false;
    m_positionTimer->stop();
    
    // Fechar stream anterior ANTES de parar (evita double free)
    if (m_stream) {
        if (Pa_IsStreamActive(m_stream)) {
            Pa_StopStream(m_stream);
        }
        Pa_CloseStream(m_stream);
        m_stream = nullptr;
        LOG_PLAYER("Stream anterior fechado");
    }
    
    // Resetar posição e região
    m_playPosition = 0;
    m_hasPlaybackRegion = false;
    m_regionStartSample = 0;
    m_regionEndSample = 0;
    
    emit playbackStateChanged(0); // Stopped
    emit positionChanged(0);
    
    m_audioFile = audioFile;
    
    if (!audioFile) {
        m_samples.clear();
        LOG_PLAYER("Arquivo removido - samples limpos");
        return;
    }
    
    // Copiar samples do AudioFile
    const QVector<float> &qsamples = audioFile->getSamples();
    m_samples.assign(qsamples.begin(), qsamples.end());
    m_sampleRate = audioFile->getSampleRate();
    m_channels = audioFile->getNumChannels();
    
    LOG_PLAYER(QString("Arquivo carregado: %1 samples, %2 Hz, %3 canais")
        .arg(m_samples.size()).arg(m_sampleRate).arg(m_channels));
    
    // Criar novo stream
    PaStreamParameters outputParameters;
    outputParameters.device = Pa_GetDefaultOutputDevice();
    if (outputParameters.device == paNoDevice) {
        LOG_PLAYER("ERRO: Nenhum dispositivo de saída encontrado");
        emit errorOccurred("Nenhum dispositivo de áudio encontrado");
        return;
    }
    
    outputParameters.channelCount = m_channels;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = nullptr;
    
    PaError err = Pa_OpenStream(
        &m_stream,
        nullptr, // sem entrada
        &outputParameters,
        m_sampleRate,
        256, // frames por buffer (baixa latência)
        paClipOff,
        &CustomAudioPlayer::audioCallback,
        this
    );
    
    if (err != paNoError) {
        LOG_PLAYER(QString("ERRO Pa_OpenStream: %1").arg(Pa_GetErrorText(err)));
        emit errorOccurred(QString("Erro ao abrir stream de áudio: %1").arg(Pa_GetErrorText(err)));
        return;
    }
    
    LOG_PLAYER("Stream de áudio criado com sucesso");
    
    m_playPosition = 0;
    emit durationChanged(duration());
}

void CustomAudioPlayer::play()
{
    if (!m_audioFile || m_samples.empty()) {
        LOG_PLAYER("ERRO: play() chamado sem arquivo");
        return;
    }
    
    if (m_isPlaying && !m_isPaused) {
        LOG_PLAYER("Já está tocando");
        return;
    }
    
    LOG_PLAYER(QString("play() - Posição: %1, Loop: %2, Região: %3")
        .arg(m_playPosition.load())
        .arg(m_loopEnabled.load())
        .arg(m_hasPlaybackRegion.load()));
    
    // Se estava pausado, apenas retomar
    if (m_isPaused) {
        m_isPaused = false;
        LOG_PLAYER("Retomando reprodução");
    } else {
        // Se há região e está fora dela, posicionar no início
        if (m_hasPlaybackRegion) {
            size_t pos = m_playPosition.load();
            size_t start = m_regionStartSample.load();
            size_t end = m_regionEndSample.load();
            
            if (pos < start || pos >= end) {
                m_playPosition = start;
                LOG_PLAYER(QString("Posicionado no início da região: %1").arg(start));
            }
        }
    }
    
    // Iniciar stream se não estiver ativo
    if (!Pa_IsStreamActive(m_stream)) {
        PaError err = Pa_StartStream(m_stream);
        if (err != paNoError) {
            LOG_PLAYER(QString("ERRO Pa_StartStream: %1").arg(Pa_GetErrorText(err)));
            emit errorOccurred(QString("Erro ao iniciar reprodução: %1").arg(Pa_GetErrorText(err)));
            return;
        }
    }
    
    m_isPlaying = true;
    m_positionTimer->start();
    emit playbackStateChanged(1); // Playing
    
    LOG_PLAYER("Reprodução iniciada");
}

void CustomAudioPlayer::pause()
{
    if (!m_isPlaying) {
        return;
    }
    
    LOG_PLAYER("pause() chamado");
    
    m_isPaused = true;
    m_isPlaying = false;
    m_positionTimer->stop();
    
    emit playbackStateChanged(2); // Paused
    LOG_PLAYER("Reprodução pausada");
}

void CustomAudioPlayer::stop()
{
    LOG_PLAYER("stop() chamado");
    
    m_isPlaying = false;
    m_isPaused = false;
    m_positionTimer->stop();
    
    if (m_stream && Pa_IsStreamActive(m_stream)) {
        Pa_StopStream(m_stream);
    }
    
    m_playPosition = 0;
    
    emit playbackStateChanged(0); // Stopped
    emit positionChanged(0);
    
    LOG_PLAYER("Reprodução parada");
}

bool CustomAudioPlayer::isPlaying() const
{
    return m_isPlaying.load();
}

void CustomAudioPlayer::setPosition(qint64 positionMs)
{
    if (!m_audioFile) return;
    
    size_t sample = (positionMs * m_sampleRate) / 1000;
    sample = std::min(sample, m_samples.size());
    
    m_playPosition = sample;
    
    LOG_PLAYER(QString("Posição definida: %1ms (%2 samples)").arg(positionMs).arg(sample));
    emit positionChanged(positionMs);
}

qint64 CustomAudioPlayer::position() const
{
    if (!m_audioFile) return 0;
    
    size_t sample = m_playPosition.load();
    return (sample * 1000) / m_sampleRate;
}

qint64 CustomAudioPlayer::duration() const
{
    if (!m_audioFile || m_samples.empty()) return 0;
    
    return (m_samples.size() * 1000) / m_sampleRate;
}

void CustomAudioPlayer::setVolume(float volume)
{
    m_volume = std::max(0.0f, std::min(1.0f, volume));
    LOG_PLAYER(QString("Volume: %1").arg(m_volume.load()));
}

float CustomAudioPlayer::volume() const
{
    return m_volume.load();
}

void CustomAudioPlayer::setLoop(bool loop)
{
    m_loopEnabled = loop;
    LOG_PLAYER(QString("Loop: %1").arg(loop ? "SIM" : "NÃO"));
}

bool CustomAudioPlayer::isLooping() const
{
    return m_loopEnabled.load();
}

void CustomAudioPlayer::setPlaybackRegion(qint64 startMs, qint64 endMs)
{
    size_t startSample = (startMs * m_sampleRate) / 1000;
    size_t endSample = (endMs * m_sampleRate) / 1000;
    
    m_regionStartSample = startSample;
    m_regionEndSample = endSample;
    m_hasPlaybackRegion = true;
    
    LOG_PLAYER(QString("Região: %1ms - %2ms (%3 - %4 samples)")
        .arg(startMs).arg(endMs).arg(startSample).arg(endSample));
}

void CustomAudioPlayer::clearPlaybackRegion()
{
    m_hasPlaybackRegion = false;
    LOG_PLAYER("Região limpa");
}

bool CustomAudioPlayer::hasPlaybackRegion() const
{
    return m_hasPlaybackRegion.load();
}

void CustomAudioPlayer::emitPositionUpdate()
{
    if (m_isPlaying) {
        emit positionChanged(position());
    }
}

int CustomAudioPlayer::audioCallback(const void *inputBuffer, void *outputBuffer,
                                    unsigned long framesPerBuffer,
                                    const PaStreamCallbackTimeInfo* timeInfo,
                                    PaStreamCallbackFlags statusFlags,
                                    void *userData)
{
    CustomAudioPlayer *player = static_cast<CustomAudioPlayer*>(userData);
    float *out = static_cast<float*>(outputBuffer);
    
    // Se pausado ou não está tocando, silêncio
    if (player->m_isPaused || !player->m_isPlaying) {
        std::memset(out, 0, framesPerBuffer * player->m_channels * sizeof(float));
        return paContinue;
    }
    
    size_t pos = player->m_playPosition.load();
    const std::vector<float> &samples = player->m_samples;
    float volume = player->m_volume.load();
    bool hasRegion = player->m_hasPlaybackRegion.load();
    size_t regionEnd = player->m_regionEndSample.load();
    size_t regionStart = player->m_regionStartSample.load();
    bool loop = player->m_loopEnabled.load();
    
    for (unsigned long i = 0; i < framesPerBuffer; i++) {
        // Verificar fim da região
        if (hasRegion && pos >= regionEnd) {
            if (loop) {
                pos = regionStart; // Loop
            } else {
                // Fim da reprodução
                std::memset(out + i * player->m_channels, 0, 
                           (framesPerBuffer - i) * player->m_channels * sizeof(float));
                player->m_isPlaying = false;
                QMetaObject::invokeMethod(player, "playbackFinished", Qt::QueuedConnection);
                QMetaObject::invokeMethod(player, "playbackStateChanged", Qt::QueuedConnection, Q_ARG(int, 0));
                break;
            }
        }
        
        // Verificar fim do arquivo
        if (pos >= samples.size()) {
            if (loop && !hasRegion) {
                pos = 0; // Loop do arquivo completo
            } else {
                // Fim da reprodução
                std::memset(out + i * player->m_channels, 0, 
                           (framesPerBuffer - i) * player->m_channels * sizeof(float));
                player->m_isPlaying = false;
                QMetaObject::invokeMethod(player, "playbackFinished", Qt::QueuedConnection);
                QMetaObject::invokeMethod(player, "playbackStateChanged", Qt::QueuedConnection, Q_ARG(int, 0));
                break;
            }
        }
        
        // Copiar sample com volume
        for (int ch = 0; ch < player->m_channels; ch++) {
            *out++ = samples[pos] * volume;
        }
        pos++;
    }
    
    player->m_playPosition = pos;
    
    return paContinue;
}
