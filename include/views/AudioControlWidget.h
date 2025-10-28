#ifndef AUDIOCONTROLWIDGET_H
#define AUDIOCONTROLWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QCheckBox>

/**
 * @brief Widget para controles de reprodução de áudio
 * 
 * Controles:
 * - Play/Pause
 * - Stop
 * - Loop (reprodução em loop do trecho selecionado)
 * - Slider de posição
 * - Labels de tempo (posição atual / duração total)
 * - Controle de volume
 */
class AudioControlWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Construtor
     * @param parent Widget pai
     */
    explicit AudioControlWidget(QWidget *parent = nullptr);

    /**
     * @brief Destrutor
     */
    ~AudioControlWidget();
    
    /**
     * @brief Define a duração total do áudio
     * @param duration Duração em segundos
     */
    void setDuration(double duration);
    
    /**
     * @brief Define a posição atual de reprodução
     * @param position Posição em segundos
     */
    void setPosition(double position);
    
    /**
     * @brief Define o estado de reprodução
     * @param playing true se reproduzindo, false caso contrário
     */
    void setPlaying(bool playing);
    
    /**
     * @brief Define o modo loop
     * @param loop true para ativar loop, false para desativar
     */
    void setLoopMode(bool loop);
    
    /**
     * @brief Obtém o modo loop
     */
    bool isLoopMode() const;
    
    /**
     * @brief Define o volume
     * @param volume Volume (0-100)
     */
    void setVolume(int volume);
    
    /**
     * @brief Obtém o volume
     */
    int getVolume() const;

signals:
    /**
     * @brief Sinal emitido quando o botão Play é clicado
     */
    void playClicked();
    
    /**
     * @brief Sinal emitido quando o botão Pause é clicado
     */
    void pauseClicked();
    
    /**
     * @brief Sinal emitido quando o botão Stop é clicado
     */
    void stopClicked();
    
    /**
     * @brief Sinal emitido quando o modo loop muda
     */
    void loopModeChanged(bool loop);
    
    /**
     * @brief Sinal emitido quando o usuário move o slider de posição
     */
    void positionChanged(double position);
    
    /**
     * @brief Sinal emitido quando o volume muda
     */
    void volumeChanged(int volume);

private slots:
    void onPlayPauseClicked();
    void onStopClicked();
    void onLoopCheckBoxToggled(bool checked);
    void onPositionSliderMoved(int value);
    void onVolumeSliderMoved(int value);

private:
    void setupUI();
    void updateTimeLabels();
    QString formatTime(double seconds) const;

private:
    QPushButton *m_playPauseButton;
    QPushButton *m_stopButton;
    QCheckBox *m_loopCheckBox;
    QSlider *m_positionSlider;
    QSlider *m_volumeSlider;
    QLabel *m_currentTimeLabel;
    QLabel *m_totalTimeLabel;
    QLabel *m_volumeLabel;
    
    bool m_isPlaying;
    double m_duration;
    double m_position;
    int m_volume;
    
    bool m_updatingSlider;
};

#endif // AUDIOCONTROLWIDGET_H

