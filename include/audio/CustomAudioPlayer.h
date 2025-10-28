#ifndef CUSTOMAUDIOPLAYER_H
#define CUSTOMAUDIOPLAYER_H

#include <QObject>
#include <memory>
#include <vector>
#include <atomic>
#include <portaudio.h>

class AudioFile;

/**
 * @brief Player de áudio customizado usando PortAudio
 * 
 * Solução robusta e multiplataforma para reprodução de áudio com:
 * - Latência mínima
 * - Loop perfeito
 * - Seek instantâneo
 * - Controle total do buffer
 */
class CustomAudioPlayer : public QObject
{
    Q_OBJECT

public:
    explicit CustomAudioPlayer(QObject *parent = nullptr);
    ~CustomAudioPlayer();

    /**
     * @brief Define o arquivo de áudio a ser reproduzido
     */
    void setAudioFile(std::shared_ptr<AudioFile> audioFile);

    /**
     * @brief Inicia reprodução
     */
    void play();

    /**
     * @brief Pausa reprodução
     */
    void pause();

    /**
     * @brief Para reprodução e volta ao início
     */
    void stop();

    /**
     * @brief Verifica se está tocando
     */
    bool isPlaying() const;

    /**
     * @brief Define posição de reprodução em milissegundos
     */
    void setPosition(qint64 positionMs);

    /**
     * @brief Obtém posição atual em milissegundos
     */
    qint64 position() const;

    /**
     * @brief Obtém duração total em milissegundos
     */
    qint64 duration() const;

    /**
     * @brief Define volume (0.0 a 1.0)
     */
    void setVolume(float volume);

    /**
     * @brief Obtém volume atual
     */
    float volume() const;

    /**
     * @brief Ativa/desativa loop
     */
    void setLoop(bool loop);

    /**
     * @brief Verifica se loop está ativo
     */
    bool isLooping() const;

    /**
     * @brief Define região de reprodução para loop
     */
    void setPlaybackRegion(qint64 startMs, qint64 endMs);

    /**
     * @brief Limpa região de reprodução
     */
    void clearPlaybackRegion();

    /**
     * @brief Verifica se há região definida
     */
    bool hasPlaybackRegion() const;

signals:
    /**
     * @brief Emitido quando posição muda
     */
    void positionChanged(qint64 position);

    /**
     * @brief Emitido quando duração muda
     */
    void durationChanged(qint64 duration);

    /**
     * @brief Emitido quando estado de reprodução muda
     */
    void playbackStateChanged(int state); // 0=Stopped, 1=Playing, 2=Paused

    /**
     * @brief Emitido quando reprodução termina
     */
    void playbackFinished();

    /**
     * @brief Emitido em caso de erro
     */
    void errorOccurred(const QString &error);

private:
    /**
     * @brief Callback de áudio do PortAudio
     */
    static int audioCallback(const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData);

    /**
     * @brief Inicializa PortAudio
     */
    bool initializePortAudio();

    /**
     * @brief Finaliza PortAudio
     */
    void terminatePortAudio();

    /**
     * @brief Emite sinal de posição (thread-safe)
     */
    void emitPositionUpdate();

    // Dados do áudio
    std::shared_ptr<AudioFile> m_audioFile;
    std::vector<float> m_samples;
    int m_sampleRate;
    int m_channels;

    // PortAudio
    PaStream *m_stream;
    bool m_portAudioInitialized;

    // Controle de reprodução (atomic para thread-safety)
    std::atomic<size_t> m_playPosition;
    std::atomic<bool> m_isPlaying;
    std::atomic<bool> m_isPaused;
    std::atomic<float> m_volume;

    // Loop
    std::atomic<bool> m_loopEnabled;
    std::atomic<bool> m_hasPlaybackRegion;
    std::atomic<size_t> m_regionStartSample;
    std::atomic<size_t> m_regionEndSample;

    // Timer para atualizar UI
    class QTimer *m_positionTimer;
};

#endif // CUSTOMAUDIOPLAYER_H
