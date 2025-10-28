#ifndef SPECTROGRAMCALCULATOR_H
#define SPECTROGRAMCALCULATOR_H

#include <QObject>
#include <QImage>
#include <QVector>
#include <memory>
#include <complex>

class AudioFile;

/**
 * @brief Calculador de espectrograma em thread separada
 * 
 * Limita o cálculo a 20 segundos de áudio para evitar travamento
 */
class SpectrogramCalculator : public QObject
{
    Q_OBJECT

public:
    struct Parameters {
        double timeStep = 0.005;        // 5 ms
        double timeWindow = 0.025;      // 25 ms
        int fftSize = 1024;
        QString windowType = "Hamming";
        double minFrequency = 0.0;
        double maxFrequency = 8000.0;
        double dynamicRange = 70.0;
        QString colorMap = "Grayscale";
        bool preEmphasis = false;
        double preEmphasisFactor = 0.97;
        double maxDuration = 20.0;      // Limite de 20 segundos
        double startTime = 0.0;         // Tempo inicial da janela
        double windowDuration = 0.0;    // Duração da janela (0 = usar maxDuration)
    };

    explicit SpectrogramCalculator(QObject *parent = nullptr);
    ~SpectrogramCalculator();
    
    /**
     * @brief Calcula o espectrograma (executa em thread)
     */
    void calculate(std::shared_ptr<AudioFile> audioFile, const Parameters &params);
    
    /**
     * @brief Cancela o cálculo em andamento
     */
    void cancel();
    
    /**
     * @brief Verifica se está calculando
     */
    bool isCalculating() const { return m_isCalculating; }

signals:
    void calculationStarted();
    void calculationProgress(int percent);
    void calculationFinished(QImage spectrogram);
    void calculationError(QString error);
    void startCalculationInThread();  // Sinal interno para thread

public slots:
    void performCalculation();

private:
    QVector<float> applyWindow(const QVector<float> &frame, const QString &windowType);
    QVector<std::complex<float>> computeFFT(const QVector<float> &frame);
    QColor valueToColor(float value, const QString &colorMap, float dynamicRange);
    int nextPowerOfTwo(int n);

private:
    std::shared_ptr<AudioFile> m_audioFile;
    Parameters m_params;
    bool m_isCalculating;
    bool m_cancelRequested;
};

#endif // SPECTROGRAMCALCULATOR_H
