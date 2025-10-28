#ifndef AUDIOVISUALIZATIONWIDGET_H
#define AUDIOVISUALIZATIONWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <memory>

class AudioFile;
class SpectrogramWidget;

/**
 * @brief Widget para visualização interativa de forma de onda e espectrograma
 * 
 * Funcionalidades:
 * - Visualização de forma de onda (waveform)
 * - Visualização de espectrograma sincronizado
 * - Zoom horizontal (roda do mouse)
 * - Navegação temporal (arrastar)
 * - Seleção de trechos
 * - Labels de início e fim do trecho visualizado
 */
class AudioVisualizationWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Construtor
     * @param parent Widget pai
     */
    explicit AudioVisualizationWidget(QWidget *parent = nullptr);

    /**
     * @brief Destrutor
     */
    ~AudioVisualizationWidget();
    
    /**
     * @brief Define o arquivo de áudio a ser visualizado
     * @param audioFile Ponteiro para o arquivo de áudio
     */
    void setAudioFile(std::shared_ptr<AudioFile> audioFile);
    
    /**
     * @brief Define se o espectrograma deve ser exibido
     * @param show true para exibir, false para ocultar
     */
    void setShowSpectrogram(bool show);
    
    /**
     * @brief Obtém se o espectrograma está sendo exibido
     */
    bool isShowingSpectrogram() const { return m_showSpectrogram; }
    
    /**
     * @brief Define o nível de zoom para exibir o espectrograma
     * @param zoomLevel Nível de zoom (1.0 = sem zoom)
     */
    void setSpectrogramZoomThreshold(double zoomLevel);
    
    /**
     * @brief Aplica zoom horizontal
     * @param factor Fator de zoom (> 1.0 = zoom in, < 1.0 = zoom out)
     */
    void zoom(double factor);
    
    /**
     * @brief Ajusta o zoom para exibir todo o áudio
     */
    void zoomFit();
    
    /**
     * @brief Navega para uma posição temporal específica
     * @param timeSeconds Tempo em segundos
     */
    void scrollToTime(double timeSeconds);
    
    /**
     * @brief Obtém a janela de tempo visível
     * @param startTime Tempo inicial (saída)
     * @param endTime Tempo final (saída)
     */
    void getVisibleTimeRange(double &startTime, double &endTime) const;
    
    /**
     * @brief Define a seleção de tempo
     * @param startTime Tempo inicial da seleção
     * @param endTime Tempo final da seleção
     */
    void setTimeSelection(double startTime, double endTime);
    
    /**
     * @brief Limpa a seleção de tempo
     */
    void clearTimeSelection();
    
    /**
     * @brief Obtém a seleção de tempo atual
     * @param startTime Tempo inicial da seleção (saída)
     * @param endTime Tempo final da seleção (saída)
     * @return true se há seleção ativa
     */
    bool getTimeSelection(double &startTime, double &endTime) const;

signals:
    /**
     * @brief Sinal emitido quando a janela de tempo visível muda
     */
    void visibleTimeRangeChanged(double startTime, double endTime);
    
    /**
     * @brief Sinal emitido quando a seleção de tempo muda
     */
    void timeSelectionChanged(double startTime, double endTime);
    
    /**
     * @brief Sinal emitido quando a seleção é limpa
     */
    void timeSelectionCleared();
    
    /**
     * @brief Sinal emitido quando o usuário clica em uma posição temporal
     */
    void timeClicked(double timeSeconds);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawWaveform(QPainter &painter);
    void drawWaveformDirect(QPainter &painter, const QVector<float>& samples,
                           int startSample, int endSample,
                           int topMargin, int waveHeight, int centerY);
    void drawWaveformDownsampled(QPainter &painter, const QVector<float>& samples,
                                 int startSample, int endSample,
                                 int topMargin, int waveHeight, int centerY);
    void drawTimeLabels(QPainter &painter);
    void drawSelection(QPainter &painter);
    void drawPlaybackCursor(QPainter &painter);
    
    double pixelToTime(int pixel) const;
    int timeToPixel(double time) const;
    
    void updateSpectrogramVisibility();

private:
    std::shared_ptr<AudioFile> m_audioFile;
    SpectrogramWidget *m_spectrogramWidget;
    
    // Visualization parameters
    double m_viewStartTime;
    double m_viewDuration;
    double m_zoomLevel;
    double m_spectrogramZoomThreshold;
    bool m_showSpectrogram;
    
    // Selection
    bool m_hasSelection;
    double m_selectionStart;
    double m_selectionEnd;
    
    // Playback cursor
    double m_playbackPosition;
    bool m_isPlaying;
    
    // Mouse interaction
    bool m_isDragging;
    bool m_isSelecting;
    QPoint m_lastMousePos;
    int m_dragStartX;
    
    // Layout
    int m_waveformHeight;
    int m_spectrogramHeight;
    int m_timeLabelsHeight;
};

#endif // AUDIOVISUALIZATIONWIDGET_H

