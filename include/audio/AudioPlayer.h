#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <memory>

class AudioFile;

/**
 * @brief Reprodutor de áudio usando QMediaPlayer (multiplataforma)
 * 
 * Suporta reprodução de arquivos de áudio em Linux, Windows e macOS
 */
class AudioPlayer : public QObject
{
    Q_OBJECT

public:
    explicit AudioPlayer(QObject *parent = nullptr);
    ~AudioPlayer();
    
    /**
     * @brief Define o arquivo de áudio a ser reproduzido
     * @param audioFile Ponteiro para o arquivo de áudio
     */
    void setAudioFile(std::shared_ptr<AudioFile> audioFile);
    
    /**
     * @brief Inicia a reprodução
     */
    void play();
    
    /**
     * @brief Pausa a reprodução
     */
    void pause();
    
    /**
     * @brief Para a reprodução
     */
    void stop();
    
    /**
     * @brief Verifica se está tocando
     */
    bool isPlaying() const;
    
    /**
     * @brief Define a posição de reprodução
     * @param position Posição em milissegundos
     */
    void setPosition(qint64 positionMs);
    
    /**
     * @brief Obtém a posição de reprodução
     * @return Posição em milissegundos
     */
    qint64 getPosition() const;
    
    /**
     * @brief Obtém a duração total
     * @return Duração em milissegundos
     */
    qint64 getDuration() const;
    
    /**
     * @brief Define o volume
     * @param volume Volume de 0.0 a 1.0
     */
    void setVolume(float volume);
    
    /**
     * @brief Obtém o volume
     * @return Volume de 0.0 a 1.0
     */
    float getVolume() const;
    
    /**
     * @brief Define modo loop
     * @param loop true para ativar loop
     */
    void setLoop(bool loop);
    
    /**
     * @brief Verifica se loop está ativo
     */
    bool isLooping() const;

signals:
    /**
     * @brief Sinal emitido quando a posição muda
     * @param position Posição em milissegundos
     */
    void positionChanged(qint64 position);
    
    /**
     * @brief Sinal emitido quando a duração muda
     * @param duration Duração em milissegundos
     */
    void durationChanged(qint64 duration);
    
    /**
     * @brief Sinal emitido quando o estado muda
     */
    void playbackStateChanged(QMediaPlayer::PlaybackState state);
    
    /**
     * @brief Sinal emitido quando há erro
     */
    void errorOccurred(const QString &error);
    
    /**
     * @brief Sinal emitido quando termina a reprodução
     */
    void playbackFinished();

private slots:
    void onPositionChanged(qint64 position);
    void onDurationChanged(qint64 duration);
    void onPlaybackStateChanged(QMediaPlayer::PlaybackState state);
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void onErrorOccurred(QMediaPlayer::Error error, const QString &errorString);

private:
    QMediaPlayer *m_player;
    QAudioOutput *m_audioOutput;
    std::shared_ptr<AudioFile> m_audioFile;
    bool m_loopEnabled;
};

#endif // AUDIOPLAYER_H
