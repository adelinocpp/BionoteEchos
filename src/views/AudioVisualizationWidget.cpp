#include "views/AudioVisualizationWidget.h"
#include "models/AudioFile.h"
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <cmath>
#include <cstdlib>

AudioVisualizationWidget::AudioVisualizationWidget(QWidget *parent)
    : QWidget(parent)
    , m_viewStartTime(0.0)
    , m_viewDuration(10.0)
    , m_zoomLevel(1.0)
    , m_spectrogramZoomThreshold(5.0)
    , m_showSpectrogram(false)
    , m_hasSelection(false)
    , m_selectionStart(0.0)
    , m_selectionEnd(0.0)
    , m_playbackPosition(0.0)
    , m_isPlaying(false)
    , m_isDragging(false)
    , m_isSelecting(false)
{
    setMinimumHeight(200);
}

AudioVisualizationWidget::~AudioVisualizationWidget()
{
}

void AudioVisualizationWidget::setAudioFile(std::shared_ptr<AudioFile> audioFile)
{
    m_audioFile = audioFile;
    
    if (m_audioFile) {
        // Ajustar visualização para mostrar todo o áudio
        m_viewStartTime = 0.0;
        m_viewDuration = m_audioFile->getDuration();
        m_zoomLevel = 1.0;
    }
    
    update();
}

void AudioVisualizationWidget::setShowSpectrogram(bool show)
{
    m_showSpectrogram = show;
    update();
}

void AudioVisualizationWidget::setSpectrogramZoomThreshold(double zoomLevel)
{
    m_spectrogramZoomThreshold = zoomLevel;
}

void AudioVisualizationWidget::zoom(double factor)
{
    m_zoomLevel *= factor;
    update();
}

void AudioVisualizationWidget::zoomFit()
{
    if (m_audioFile) {
        m_viewDuration = m_audioFile->getDuration();
        m_viewStartTime = 0.0;
        m_zoomLevel = 1.0;
        update();
    }
}

void AudioVisualizationWidget::scrollToTime(double timeSeconds)
{
    m_viewStartTime = timeSeconds;
    update();
}

void AudioVisualizationWidget::getVisibleTimeRange(double &startTime, double &endTime) const
{
    startTime = m_viewStartTime;
    endTime = m_viewStartTime + m_viewDuration;
}

void AudioVisualizationWidget::setTimeSelection(double startTime, double endTime)
{
    m_hasSelection = true;
    m_selectionStart = startTime;
    m_selectionEnd = endTime;
    emit timeSelectionChanged(startTime, endTime);
    update();
}

void AudioVisualizationWidget::clearTimeSelection()
{
    m_hasSelection = false;
    emit timeSelectionCleared();
    update();
}

bool AudioVisualizationWidget::getTimeSelection(double &startTime, double &endTime) const
{
    if (m_hasSelection) {
        startTime = m_selectionStart;
        endTime = m_selectionEnd;
        return true;
    }
    return false;
}

void AudioVisualizationWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Fundo branco
    painter.fillRect(rect(), Qt::white);
    
    // Desenhar borda
    painter.setPen(QPen(QColor(200, 200, 200), 1));
    painter.drawRect(rect().adjusted(0, 0, -1, -1));
    
    if (!m_audioFile) {
        // Placeholder quando não há áudio
        painter.setPen(QColor(150, 150, 150));
        QFont font = painter.font();
        font.setPointSize(12);
        painter.setFont(font);
        
        QString text = "Selecione um arquivo de áudio na lista à esquerda\n"
                      "ou abra um novo arquivo pelo menu Arquivo";
        painter.drawText(rect(), Qt::AlignCenter, text);
        
        // Desenhar ícone visual
        painter.setPen(QPen(QColor(200, 200, 200), 2));
        int centerX = width() / 2;
        int centerY = height() / 2 - 40;
        painter.drawEllipse(centerX - 30, centerY - 30, 60, 60);
        painter.drawLine(centerX - 15, centerY, centerX + 15, centerY);
        
        return;
    }
    
    // Desenhar informações do arquivo
    painter.setPen(Qt::black);
    QFont font = painter.font();
    font.setPointSize(10);
    font.setBold(true);
    painter.setFont(font);
    
    QString info = QString("%1 | %2 Hz | %3 canal(is) | %4 s")
                   .arg(m_audioFile->getFileName())
                   .arg(m_audioFile->getSampleRate())
                   .arg(m_audioFile->getNumChannels())
                   .arg(m_audioFile->getDuration(), 0, 'f', 2);
    painter.drawText(10, 20, info);
    
    drawWaveform(painter);
    drawTimeLabels(painter);
    if (m_hasSelection) {
        drawSelection(painter);
    }
}

void AudioVisualizationWidget::drawWaveform(QPainter &painter)
{
    if (!m_audioFile) return;
    
    int drawHeight = height() - 60;  // Espaço para labels e info
    int centerY = height() / 2;
    int topMargin = 35;
    int waveHeight = drawHeight - topMargin;
    
    // Desenhar linha central
    painter.setPen(QPen(QColor(220, 220, 220), 1));
    painter.drawLine(0, centerY, width(), centerY);
    
    // Obter amostras do canal 0 (primeiro canal)
    const QVector<float>& samples = m_audioFile->getSamples(0);
    if (samples.isEmpty()) {
        painter.setPen(Qt::red);
        painter.drawText(rect(), Qt::AlignCenter, "Sem dados de áudio");
        return;
    }
    
    // Calcular qual porção do áudio visualizar
    double totalDuration = m_audioFile->getDuration();
    double viewEndTime = m_viewStartTime + m_viewDuration;
    
    // Limitar ao tamanho do áudio
    if (viewEndTime > totalDuration) {
        viewEndTime = totalDuration;
        m_viewDuration = viewEndTime - m_viewStartTime;
    }
    
    // Converter tempo para índices de amostra
    int sampleRate = m_audioFile->getSampleRate();
    int startSample = (int)(m_viewStartTime * sampleRate);
    int endSample = (int)(viewEndTime * sampleRate);
    
    // Garantir limites válidos
    startSample = qMax(0, qMin(startSample, samples.size() - 1));
    endSample = qMax(startSample + 1, qMin(endSample, samples.size()));
    
    int numSamples = endSample - startSample;
    int screenWidth = width();
    
    // Desenhar waveform
    painter.setRenderHint(QPainter::Antialiasing, false);  // Mais rápido sem antialiasing
    
    if (numSamples < screenWidth * 2) {
        // Poucos samples: desenhar cada um
        drawWaveformDirect(painter, samples, startSample, endSample, 
                          topMargin, waveHeight, centerY);
    } else {
        // Muitos samples: usar min/max downsampling
        drawWaveformDownsampled(painter, samples, startSample, endSample,
                               topMargin, waveHeight, centerY);
    }
}

void AudioVisualizationWidget::drawWaveformDirect(QPainter &painter, 
                                                  const QVector<float>& samples,
                                                  int startSample, int endSample,
                                                  int topMargin, int waveHeight, int centerY)
{
    int numSamples = endSample - startSample;
    int screenWidth = width();
    
    painter.setPen(QPen(QColor(0, 100, 200), 1));
    
    for (int x = 0; x < screenWidth - 1; ++x) {
        // Mapear pixel para amostra
        int sampleIdx = startSample + (x * numSamples) / screenWidth;
        int nextSampleIdx = startSample + ((x + 1) * numSamples) / screenWidth;
        
        if (sampleIdx >= samples.size()) break;
        
        float sample1 = samples[sampleIdx];
        float sample2 = (nextSampleIdx < samples.size()) ? samples[nextSampleIdx] : sample1;
        
        // Converter amplitude (-1 a 1) para coordenadas Y
        int y1 = centerY - (int)(sample1 * waveHeight / 2);
        int y2 = centerY - (int)(sample2 * waveHeight / 2);
        
        painter.drawLine(x, y1, x + 1, y2);
    }
}

void AudioVisualizationWidget::drawWaveformDownsampled(QPainter &painter,
                                                       const QVector<float>& samples,
                                                       int startSample, int endSample,
                                                       int topMargin, int waveHeight, int centerY)
{
    int numSamples = endSample - startSample;
    int screenWidth = width();
    
    painter.setPen(QPen(QColor(0, 100, 200), 1));
    
    // Para cada pixel, calcular min e max das amostras correspondentes
    for (int x = 0; x < screenWidth; ++x) {
        // Calcular faixa de amostras para este pixel
        int sampleStart = startSample + (x * numSamples) / screenWidth;
        int sampleEnd = startSample + ((x + 1) * numSamples) / screenWidth;
        
        if (sampleStart >= samples.size()) break;
        sampleEnd = qMin(sampleEnd, samples.size());
        
        // Encontrar min e max nesta faixa
        float minVal = 0.0f;
        float maxVal = 0.0f;
        
        for (int i = sampleStart; i < sampleEnd; ++i) {
            float sample = samples[i];
            minVal = qMin(minVal, sample);
            maxVal = qMax(maxVal, sample);
        }
        
        // Desenhar linha vertical do min ao max
        int yMin = centerY - (int)(maxVal * waveHeight / 2);
        int yMax = centerY - (int)(minVal * waveHeight / 2);
        
        painter.drawLine(x, yMin, x, yMax);
    }
}

void AudioVisualizationWidget::drawTimeLabels(QPainter &painter)
{
    if (!m_audioFile) return;
    
    painter.setPen(Qt::black);
    QFont font = painter.font();
    font.setPointSize(9);
    painter.setFont(font);
    
    int labelY = height() - 10;
    
    // Label inicial
    QString startLabel = QString("%1 s").arg(m_viewStartTime, 0, 'f', 3);
    painter.drawText(10, labelY, startLabel);
    
    // Label final
    double endTime = m_viewStartTime + m_viewDuration;
    QString endLabel = QString("%1 s").arg(endTime, 0, 'f', 3);
    QFontMetrics fm(font);
    int endLabelWidth = fm.horizontalAdvance(endLabel);
    painter.drawText(width() - endLabelWidth - 10, labelY, endLabel);
    
    // Labels intermediários (a cada ~100 pixels)
    int numLabels = width() / 100;
    for (int i = 1; i < numLabels; ++i) {
        double time = m_viewStartTime + (i * m_viewDuration / numLabels);
        int x = (i * width()) / numLabels;
        
        QString label = QString("%1").arg(time, 0, 'f', 2);
        int labelWidth = fm.horizontalAdvance(label);
        
        // Desenhar marca
        painter.setPen(QPen(QColor(180, 180, 180), 1));
        painter.drawLine(x, height() - 25, x, height() - 15);
        
        // Desenhar texto
        painter.setPen(Qt::black);
        painter.drawText(x - labelWidth / 2, labelY, label);
    }
}

void AudioVisualizationWidget::drawSelection(QPainter &painter)
{
    // Stub implementation
}

void AudioVisualizationWidget::drawPlaybackCursor(QPainter &painter)
{
    // Stub implementation
}

void AudioVisualizationWidget::mousePressEvent(QMouseEvent *event)
{
    if (!m_audioFile) return;
    
    if (event->button() == Qt::LeftButton) {
        m_isDragging = true;
        m_lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }
}

void AudioVisualizationWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_audioFile) return;
    
    if (m_isDragging) {
        // Pan horizontal - arrastar para navegar
        int deltaX = event->pos().x() - m_lastMousePos.x();
        m_lastMousePos = event->pos();
        
        // Converter delta em pixels para delta em tempo
        double deltaTime = -(deltaX / (double)width()) * m_viewDuration;
        
        m_viewStartTime += deltaTime;
        
        // Garantir limites
        double maxDuration = m_audioFile->getDuration();
        if (m_viewStartTime < 0) {
            m_viewStartTime = 0;
        }
        if (m_viewStartTime + m_viewDuration > maxDuration) {
            m_viewStartTime = maxDuration - m_viewDuration;
        }
        
        update();
        emit visibleTimeRangeChanged(m_viewStartTime, m_viewStartTime + m_viewDuration);
    }
}

void AudioVisualizationWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDragging = false;
        setCursor(Qt::ArrowCursor);
    }
}

void AudioVisualizationWidget::wheelEvent(QWheelEvent *event)
{
    if (!m_audioFile) return;
    
    // Zoom com scroll do mouse
    double factor = event->angleDelta().y() > 0 ? 0.8 : 1.2;  // Invertido para ser mais intuitivo
    
    // Calcular ponto focal do zoom (posição do mouse)
    double mouseTime = pixelToTime(event->position().x());
    
    // Aplicar zoom
    m_viewDuration *= factor;
    
    // Limitar zoom mínimo (10ms) e máximo (duração total do áudio)
    double minDuration = 0.01;  // 10ms
    double maxDuration = m_audioFile->getDuration();
    m_viewDuration = qMax(minDuration, qMin(m_viewDuration, maxDuration));
    
    // Ajustar viewStartTime para manter o ponto focal
    m_viewStartTime = mouseTime - (event->position().x() / (double)width()) * m_viewDuration;
    
    // Garantir que não saia dos limites
    if (m_viewStartTime < 0) {
        m_viewStartTime = 0;
    }
    if (m_viewStartTime + m_viewDuration > maxDuration) {
        m_viewStartTime = maxDuration - m_viewDuration;
    }
    
    update();
    emit visibleTimeRangeChanged(m_viewStartTime, m_viewStartTime + m_viewDuration);
}

void AudioVisualizationWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}

double AudioVisualizationWidget::pixelToTime(int pixel) const
{
    return m_viewStartTime + (pixel / (double)width()) * m_viewDuration;
}

int AudioVisualizationWidget::timeToPixel(double time) const
{
    return (int)((time - m_viewStartTime) / m_viewDuration * width());
}

void AudioVisualizationWidget::updateSpectrogramVisibility()
{
    // Stub implementation
}
