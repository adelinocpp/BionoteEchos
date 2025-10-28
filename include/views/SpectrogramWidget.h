#ifndef SPECTROGRAMWIDGET_H
#define SPECTROGRAMWIDGET_H

#include <QWidget>
#include <QImage>
#include <QThread>
#include <memory>

class AudioFile;
class SpectrogramCalculator;

/**
 * @brief Widget para visualização de espectrograma
 */
class SpectrogramWidget : public QWidget
{
    Q_OBJECT

public:
    struct Settings {
        // Parâmetros de cálculo
        double timeStep = 0.01;           // Passo de tempo (s) - 10ms
        double timeWindow = 0.03;         // Janela de tempo (s) - 30ms
        int fftSize = 1024;               // Tamanho da FFT (será ajustado para potência de 2)
        QString windowType = "Hamming";   // Tipo de janela
        
        // Parâmetros de visualização
        double minFrequency = 0.0;        // Frequência mínima (Hz)
        double maxFrequency = 4000.0;     // Frequência máxima (Hz) - 4 kHz
        double dynamicRange = 90.0;       // Faixa dinâmica (dB)
        QString colorMap = "Grayscale";   // Mapa de cores
        bool preEmphasis = false;         // Pré-ênfase
        double preEmphasisFactor = 0.97;
    };

    explicit SpectrogramWidget(QWidget *parent = nullptr);
    ~SpectrogramWidget();
    
    void setAudioFile(std::shared_ptr<AudioFile> audioFile);
    void setSettings(const Settings &settings);
    Settings getSettings() const { return m_settings; }
    void setPlaybackPosition(double timeSeconds);
    void setVisibleTimeRange(double startTime, double duration);
    void calculateSpectrogram();
    bool isCalculating() const { return m_isCalculating; }

signals:
    void calculationStarted();
    void calculationProgress(int percent);
    void calculationFinished();
    void calculationError(QString error);
    void visibleTimeRangeChanged(double startTime, double duration);

protected:
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void onCalculationFinished(QImage spectrogram);
    void onCalculationProgress(int percent);
    void onCalculationError(QString error);

private:
    void drawSpectrogram(QPainter &painter);
    void drawFrequencyAxis(QPainter &painter);
    void drawPlaybackCursor(QPainter &painter);
    QColor valueToColor(float value) const;
    QString getSettingsHash() const;

private:
    std::shared_ptr<AudioFile> m_audioFile;
    Settings m_settings;
    QImage m_spectrogramImage;
    double m_viewStartTime;
    double m_viewDuration;
    double m_playbackPosition;
    bool m_isCalculating;
    int m_calculationProgress;
    
    // Pan/drag control
    bool m_isDragging;
    int m_dragStartX;
    double m_dragStartTime;
    
    QThread *m_calculatorThread;
    SpectrogramCalculator *m_calculator;
};

#endif // SPECTROGRAMWIDGET_H
