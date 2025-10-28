#include "audio/SpectrogramCalculator.h"
#include "models/AudioFile.h"
#include <QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>
#include <cmath>
#include <complex>

SpectrogramCalculator::SpectrogramCalculator(QObject *parent) 
    : QObject(parent)
    , m_isCalculating(false)
    , m_cancelRequested(false)
{
}

SpectrogramCalculator::~SpectrogramCalculator()
{
    cancel();
}

void SpectrogramCalculator::calculate(std::shared_ptr<AudioFile> audioFile, const Parameters &params)
{
    if (m_isCalculating) {
        emit calculationError("Cálculo já em andamento");
        return;
    }
    
    if (!audioFile) {
        emit calculationError("Arquivo de áudio inválido");
        return;
    }
    
    m_audioFile = audioFile;
    m_params = params;
    m_isCalculating = true;
    m_cancelRequested = false;
    
    emit calculationStarted();
    
    // Emitir sinal para executar na thread do objeto
    emit startCalculationInThread();
}

void SpectrogramCalculator::cancel()
{
    m_cancelRequested = true;
}

void SpectrogramCalculator::performCalculation()
{
    try {
        QVector<float> samples = m_audioFile->getSamples();
        int sampleRate = m_audioFile->getSampleRate();
        double duration = m_audioFile->getDuration();
        
        // Otimização: Downsampling se maxFrequency < Nyquist/2
        // Inspirado no Praat - reduz drasticamente o número de amostras
        double nyquist = sampleRate / 2.0;
        int downsampleFactor = 1;
        if (m_params.maxFrequency < nyquist / 2.0) {
            downsampleFactor = static_cast<int>(nyquist / (m_params.maxFrequency * 2.0));
            downsampleFactor = std::max(1, std::min(downsampleFactor, 8)); // Limitar a 8x
            
            if (downsampleFactor > 1) {
                // Fazer downsampling simples (média)
                QVector<float> downsampledSamples;
                downsampledSamples.reserve(samples.size() / downsampleFactor);
                
                for (int i = 0; i < samples.size(); i += downsampleFactor) {
                    float sum = 0.0f;
                    int count = 0;
                    for (int j = 0; j < downsampleFactor && (i + j) < samples.size(); ++j) {
                        sum += samples[i + j];
                        count++;
                    }
                    downsampledSamples.append(sum / count);
                }
                
                samples = downsampledSamples;
                sampleRate = sampleRate / downsampleFactor;
            }
        }
        
        // Calcular região de tempo
        double startTime = m_params.startTime;
        double calcDuration = m_params.windowDuration > 0 ? m_params.windowDuration : m_params.maxDuration;
        calcDuration = std::min(calcDuration, duration - startTime);
        
        // Converter para índices de amostra
        int startSample = static_cast<int>(startTime * sampleRate);
        int endSample = static_cast<int>((startTime + calcDuration) * sampleRate);
        
        startSample = std::max(0, std::min(startSample, static_cast<int>(samples.size()) - 1));
        endSample = std::max(startSample + 1, std::min(endSample, static_cast<int>(samples.size())));
        
        int numSamples = endSample - startSample;
        
        // Parâmetros do espectrograma
        int windowSize = static_cast<int>(m_params.timeWindow * sampleRate);
        int hopSize = static_cast<int>(m_params.timeStep * sampleRate);
        
        if (windowSize <= 0 || hopSize <= 0) {
            emit calculationError("Parâmetros inválidos");
            m_isCalculating = false;
            return;
        }
        
        // Ajustar FFT size para próxima potência de 2 acima do windowSize
        int fftSize = nextPowerOfTwo(windowSize);
        
        // Número de frames
        int numFrames = (numSamples - windowSize) / hopSize + 1;
        if (numFrames <= 0) {
            emit calculationError("Áudio muito curto");
            m_isCalculating = false;
            return;
        }
        
        // Número de bins de frequência
        int numFreqBins = fftSize / 2 + 1;
        
        // Calcular limites de frequência em bins
        int minBin = static_cast<int>(m_params.minFrequency * fftSize / sampleRate);
        int maxBin = static_cast<int>(m_params.maxFrequency * fftSize / sampleRate);
        minBin = std::max(0, std::min(minBin, numFreqBins - 1));
        maxBin = std::max(minBin + 1, std::min(maxBin, numFreqBins));
        int numDisplayBins = maxBin - minBin;
        
        // Criar imagem do espectrograma
        QImage spectrogram(numFrames, numDisplayBins, QImage::Format_RGB32);
        
        // Matriz para armazenar magnitudes
        QVector<QVector<float>> magnitudes(numFrames);
        
        // Calcular FFT para cada frame
        for (int frameIdx = 0; frameIdx < numFrames; ++frameIdx) {
            if (m_cancelRequested) {
                m_isCalculating = false;
                return;
            }
            
            // Progresso
            if (frameIdx % 100 == 0) {
                int progress = (frameIdx * 100) / numFrames;
                emit calculationProgress(progress);
            }
            
            // Extrair frame
            int frameSample = startSample + frameIdx * hopSize;
            QVector<float> frame(windowSize);
            for (int i = 0; i < windowSize && (frameSample + i) < endSample; ++i) {
                frame[i] = samples[frameSample + i];
            }
            
            // Aplicar janela
            frame = applyWindow(frame, m_params.windowType);
            
            // Pré-ênfase (opcional)
            if (m_params.preEmphasis) {
                for (int i = frame.size() - 1; i > 0; --i) {
                    frame[i] = frame[i] - m_params.preEmphasisFactor * frame[i - 1];
                }
            }
            
            // Zero-padding para FFT size (potência de 2)
            while (frame.size() < fftSize) {
                frame.append(0.0f);
            }
            
            // Computar FFT
            QVector<std::complex<float>> fftResult = computeFFT(frame);
            
            // Calcular magnitude (em dB)
            magnitudes[frameIdx].resize(numDisplayBins);
            for (int bin = 0; bin < numDisplayBins; ++bin) {
                int fftBin = minBin + bin;
                float magnitude = std::abs(fftResult[fftBin]);
                float magnitudeDb = 20.0f * std::log10(magnitude + 1e-10f);
                magnitudes[frameIdx][bin] = magnitudeDb;
            }
        }
        
        // Encontrar min/max para normalização
        float minMag = std::numeric_limits<float>::max();
        float maxMag = std::numeric_limits<float>::lowest();
        for (const auto &frame : magnitudes) {
            for (float mag : frame) {
                minMag = std::min(minMag, mag);
                maxMag = std::max(maxMag, mag);
            }
        }
        
        // Aplicar faixa dinâmica
        float threshold = maxMag - m_params.dynamicRange;
        
        // Preencher imagem
        for (int x = 0; x < numFrames; ++x) {
            for (int y = 0; y < numDisplayBins; ++y) {
                float mag = magnitudes[x][y];
                
                // Normalizar com faixa dinâmica
                float normalized = (mag - threshold) / m_params.dynamicRange;
                normalized = std::max(0.0f, std::min(1.0f, normalized));
                
                // Converter para cor
                QColor color = valueToColor(normalized, m_params.colorMap, m_params.dynamicRange);
                
                // Inverter Y (frequências baixas embaixo)
                spectrogram.setPixelColor(x, numDisplayBins - 1 - y, color);
            }
        }
        
        emit calculationProgress(100);
        emit calculationFinished(spectrogram);
        
    } catch (const std::exception &e) {
        emit calculationError(QString("Erro no cálculo: %1").arg(e.what()));
    }
    
    m_isCalculating = false;
}

QVector<float> SpectrogramCalculator::applyWindow(const QVector<float> &frame, const QString &windowType)
{
    QVector<float> windowed = frame;
    int N = frame.size();
    
    for (int i = 0; i < N; ++i) {
        float w = 1.0f;
        
        if (windowType == "Hamming") {
            w = 0.54f - 0.46f * std::cos(2.0f * M_PI * i / (N - 1));
        } else if (windowType == "Hanning") {
            w = 0.5f * (1.0f - std::cos(2.0f * M_PI * i / (N - 1)));
        } else if (windowType == "Blackman") {
            w = 0.42f - 0.5f * std::cos(2.0f * M_PI * i / (N - 1)) 
                + 0.08f * std::cos(4.0f * M_PI * i / (N - 1));
        }
        // "Rectangular" não aplica janela
        
        windowed[i] *= w;
    }
    
    return windowed;
}

QVector<std::complex<float>> SpectrogramCalculator::computeFFT(const QVector<float> &frame)
{
    int N = frame.size();
    QVector<std::complex<float>> result(N);
    
    // FFT simples (DFT) - pode ser otimizado com FFT real
    for (int k = 0; k < N; ++k) {
        std::complex<float> sum(0.0f, 0.0f);
        for (int n = 0; n < N; ++n) {
            float angle = -2.0f * M_PI * k * n / N;
            sum += frame[n] * std::complex<float>(std::cos(angle), std::sin(angle));
        }
        result[k] = sum;
    }
    
    return result;
}

QColor SpectrogramCalculator::valueToColor(float value, const QString &colorMap, float dynamicRange)
{
    value = std::max(0.0f, std::min(1.0f, value));
    
    if (colorMap == "Grayscale") {
        int gray = static_cast<int>(value * 255);
        return QColor(gray, gray, gray);
    } else if (colorMap == "Jet") {
        // Mapa Jet: azul -> ciano -> verde -> amarelo -> vermelho
        if (value < 0.25f) {
            float t = value / 0.25f;
            return QColor(0, static_cast<int>(t * 255), 255);
        } else if (value < 0.5f) {
            float t = (value - 0.25f) / 0.25f;
            return QColor(0, 255, static_cast<int>((1.0f - t) * 255));
        } else if (value < 0.75f) {
            float t = (value - 0.5f) / 0.25f;
            return QColor(static_cast<int>(t * 255), 255, 0);
        } else {
            float t = (value - 0.75f) / 0.25f;
            return QColor(255, static_cast<int>((1.0f - t) * 255), 0);
        }
    } else if (colorMap == "Hot") {
        // Mapa Hot: preto -> vermelho -> amarelo -> branco
        if (value < 0.33f) {
            float t = value / 0.33f;
            return QColor(static_cast<int>(t * 255), 0, 0);
        } else if (value < 0.66f) {
            float t = (value - 0.33f) / 0.33f;
            return QColor(255, static_cast<int>(t * 255), 0);
        } else {
            float t = (value - 0.66f) / 0.34f;
            return QColor(255, 255, static_cast<int>(t * 255));
        }
    } else if (colorMap == "Cool") {
        // Mapa Cool: ciano -> magenta
        return QColor(static_cast<int>(value * 255), static_cast<int>((1.0f - value) * 255), 255);
    } else if (colorMap == "Viridis") {
        // Aproximação do Viridis
        if (value < 0.5f) {
            float t = value / 0.5f;
            return QColor(static_cast<int>(t * 68), static_cast<int>(t * 1 + (1-t) * 0), static_cast<int>(t * 84 + (1-t) * 68));
        } else {
            float t = (value - 0.5f) / 0.5f;
            return QColor(static_cast<int>(t * 253 + (1-t) * 68), static_cast<int>(t * 231 + (1-t) * 1), static_cast<int>(t * 37 + (1-t) * 84));
        }
    }
    
    // Default: Grayscale
    int gray = static_cast<int>(value * 255);
    return QColor(gray, gray, gray);
}

int SpectrogramCalculator::nextPowerOfTwo(int n)
{
    if (n <= 0) return 1;
    
    // Se já é potência de 2, retorna o próximo
    int power = 1;
    while (power < n) {
        power *= 2;
    }
    return power;
}
