#include "views/AudioVisualizationWidget.h"
#include "models/AudioFile.h"
#include "utils/Logger.h"
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTimer>
#include <QKeyEvent>
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
    , m_mouseInWidget(false)
{
    setMinimumHeight(200);
    setFocusPolicy(Qt::StrongFocus); // Permitir receber eventos de teclado
    setMouseTracking(true); // Rastrear movimento do mouse
    
    // Timer para atualizar visualização durante reprodução
    m_updateTimer = new QTimer(this);
    m_updateTimer->setInterval(16); // ~60 FPS para melhor sincronização
    connect(m_updateTimer, &QTimer::timeout, this, [this]() {
        if (m_isPlaying) {
            update();
        }
    });
}

AudioVisualizationWidget::~AudioVisualizationWidget()
{
}

void AudioVisualizationWidget::setAudioFile(std::shared_ptr<AudioFile> audioFile)
{
    m_audioFile = audioFile;
    
    // Limpar estado anterior
    m_hasSelection = false;
    m_selectionStart = 0.0;
    m_selectionEnd = 0.0;
    m_playbackPosition = 0.0;
    m_isPlaying = false;
    
    // Parar timer de atualização se estiver rodando
    if (m_updateTimer && m_updateTimer->isActive()) {
        m_updateTimer->stop();
        LOG_AUDIO("Timer de atualização PARADO ao trocar arquivo");
    }
    
    if (m_audioFile) {
        // Ajustar visualização para mostrar todo o áudio
        m_viewStartTime = 0.0;
        m_viewDuration = m_audioFile->getDuration();
        m_zoomLevel = 1.0;
        LOG_AUDIO(QString("Novo arquivo carregado na visualização: duração %.2f s").arg(m_viewDuration));
    } else {
        LOG_AUDIO("Arquivo removido da visualização");
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

void AudioVisualizationWidget::setVisibleTimeRange(double startTime, double duration)
{
    m_viewStartTime = startTime;
    m_viewDuration = duration;
    update();
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
    
    QString info = QString("%1 | %2 Hz | %3 canal(is) | Duração: %4 s")
                   .arg(m_audioFile->getFileName())
                   .arg(m_audioFile->getSampleRate())
                   .arg(m_audioFile->getNumChannels())
                   .arg(m_audioFile->getDuration(), 0, 'f', 2);
    painter.drawText(10, 20, info);
    
    // Desenhar informações da janela visível e cursor
    font.setPointSize(9);
    font.setBold(false);
    painter.setFont(font);
    
    QString windowInfo;
    if (m_mouseInWidget) {
        double cursorTime = pixelToTime(m_currentMousePos.x());
        windowInfo = QString("Janela: %1 s - %2 s (duração: %3 s) | Cursor: %4 s")
                            .arg(m_viewStartTime, 0, 'f', 3)
                            .arg(m_viewStartTime + m_viewDuration, 0, 'f', 3)
                            .arg(m_viewDuration, 0, 'f', 3)
                            .arg(cursorTime, 0, 'f', 3);
    } else {
        windowInfo = QString("Janela: %1 s - %2 s (duração: %3 s)")
                            .arg(m_viewStartTime, 0, 'f', 3)
                            .arg(m_viewStartTime + m_viewDuration, 0, 'f', 3)
                            .arg(m_viewDuration, 0, 'f', 3);
    }
    painter.drawText(width() - 500, 20, windowInfo);
    
    drawAmplitudeAxis(painter);
    drawWaveform(painter);
    drawTimeLabels(painter);
    if (m_hasSelection) {
        drawSelection(painter);
    }
    if (m_isPlaying) {
        drawPlaybackCursor(painter);
    }
    if (m_mouseInWidget) {
        drawMouseCursor(painter);
    }
}

void AudioVisualizationWidget::drawWaveform(QPainter &painter)
{
    if (!m_audioFile) return;
    
    int leftMargin = 50;  // Mesma largura da escala do espectrograma
    int topMargin = 35;
    int bottomMargin = 25;
    int drawHeight = height() - topMargin - bottomMargin;
    int centerY = topMargin + drawHeight / 2;
    int waveHeight = drawHeight;
    
    // Desenhar linha central
    painter.setPen(QPen(QColor(220, 220, 220), 1));
    painter.drawLine(leftMargin, centerY, width(), centerY);
    
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
    int screenWidth = width() - leftMargin;  // Largura disponível para desenho
    
    // Desenhar waveform
    painter.setRenderHint(QPainter::Antialiasing, false);  // Mais rápido sem antialiasing
    
    if (numSamples < screenWidth * 2) {
        // Poucos samples: desenhar cada um
        drawWaveformDirect(painter, samples, startSample, endSample, 
                          leftMargin, topMargin, waveHeight, centerY);
    } else {
        // Muitos samples: usar min/max downsampling
        drawWaveformDownsampled(painter, samples, startSample, endSample,
                               leftMargin, topMargin, waveHeight, centerY);
    }
}

void AudioVisualizationWidget::drawWaveformDirect(QPainter &painter, 
                                                  const QVector<float>& samples,
                                                  int startSample, int endSample,
                                                  int leftMargin, int topMargin, int waveHeight, int centerY)
{
    int numSamples = endSample - startSample;
    int screenWidth = width() - leftMargin;
    
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
        
        painter.drawLine(leftMargin + x, y1, leftMargin + x + 1, y2);
    }
}

void AudioVisualizationWidget::drawWaveformDownsampled(QPainter &painter,
                                                       const QVector<float>& samples,
                                                       int startSample, int endSample,
                                                       int leftMargin, int topMargin, int waveHeight, int centerY)
{
    int numSamples = endSample - startSample;
    int screenWidth = width() - leftMargin;
    
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
        
        painter.drawLine(leftMargin + x, yMin, leftMargin + x, yMax);
    }
}

void AudioVisualizationWidget::drawAmplitudeAxis(QPainter &painter)
{
    if (!m_audioFile) return;
    
    int leftMargin = 50;
    int topMargin = 35;
    int bottomMargin = 25;
    int drawHeight = height() - topMargin - bottomMargin;
    int centerY = topMargin + drawHeight / 2;
    
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 8));
    
    // Desenhar escala de amplitude (-1.0 a 1.0)
    int numTicks = 5;
    
    for (int i = 0; i <= numTicks; ++i) {
        // Amplitude de -1.0 (topo) a 1.0 (baixo)
        float amplitude = 1.0f - (2.0f * i / numTicks);
        int y = topMargin + (i * drawHeight / numTicks);
        
        // Linha de marcação
        painter.drawLine(leftMargin - 5, y, leftMargin, y);
        
        // Texto da amplitude
        QString label = QString::number(amplitude, 'f', 1);
        painter.drawText(QRect(0, y - 10, leftMargin - 10, 20), 
                        Qt::AlignRight | Qt::AlignVCenter,
                        label);
    }
}

void AudioVisualizationWidget::drawTimeLabels(QPainter &painter)
{
    if (!m_audioFile) return;
    
    int leftMargin = 50;
    int drawWidth = width() - leftMargin;
    
    painter.setPen(Qt::black);
    QFont font = painter.font();
    font.setPointSize(9);
    painter.setFont(font);
    
    int labelY = height() - 10;
    QFontMetrics fm(font);
    
    // 9 ticks com intervalo de 10% (0%, 10%, 20%, ..., 80%, 90%, 100%)
    // Mas vamos desenhar apenas 0%, 10%, 20%, ..., 90%, 100% = 11 labels
    // Ou 9 ticks intermediários: 10%, 20%, 30%, 40%, 50%, 60%, 70%, 80%, 90%
    // Vou interpretar como 10 posições (0% a 100% em intervalos de 10%)
    for (int i = 0; i <= 10; ++i) {
        double fraction = i / 10.0;
        double time = m_viewStartTime + fraction * m_viewDuration;
        int x = leftMargin + static_cast<int>(fraction * drawWidth);
        
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
    if (!m_hasSelection || !m_audioFile) return;
    
    // Converter tempos de seleção para pixels
    int startX = timeToPixel(m_selectionStart);
    int endX = timeToPixel(m_selectionEnd);
    
    // Garantir que startX < endX
    if (startX > endX) {
        std::swap(startX, endX);
    }
    
    // Desenhar retângulo de seleção semi-transparente
    QColor selectionColor(100, 150, 255, 80);
    painter.fillRect(startX, 0, endX - startX, height(), selectionColor);
    
    // Desenhar bordas da seleção
    painter.setPen(QPen(QColor(0, 100, 200), 2));
    painter.drawLine(startX, 0, startX, height());
    painter.drawLine(endX, 0, endX, height());
    
    // Desenhar labels de tempo da seleção
    painter.setPen(Qt::black);
    QFont font = painter.font();
    font.setPointSize(8);
    font.setBold(true);
    painter.setFont(font);
    
    QString durationText = QString("Δ: %1 s").arg(qAbs(m_selectionEnd - m_selectionStart), 0, 'f', 3);
    QFontMetrics fm(font);
    int textWidth = fm.horizontalAdvance(durationText);
    
    // Desenhar texto no centro da seleção
    int textX = (startX + endX) / 2 - textWidth / 2;
    int textY = 15;
    
    // Fundo branco para o texto
    painter.fillRect(textX - 3, textY - 12, textWidth + 6, 15, QColor(255, 255, 255, 200));
    painter.drawText(textX, textY, durationText);
}

void AudioVisualizationWidget::drawPlaybackCursor(QPainter &painter)
{
    if (!m_isPlaying || !m_audioFile) return;
    
    // Converter posição de reprodução para pixel
    int cursorX = timeToPixel(m_playbackPosition);
    
    // Verificar se o cursor está visível
    if (cursorX < 0 || cursorX > width()) return;
    
    // Desenhar linha vertical vermelha
    painter.setPen(QPen(QColor(255, 0, 0), 2));
    painter.drawLine(cursorX, 0, cursorX, height());
    
    // Desenhar triângulo no topo
    QPolygon triangle;
    triangle << QPoint(cursorX, 0)
             << QPoint(cursorX - 5, 10)
             << QPoint(cursorX + 5, 10);
    painter.setBrush(QColor(255, 0, 0));
    painter.drawPolygon(triangle);
}

void AudioVisualizationWidget::drawMouseCursor(QPainter &painter)
{
    if (!m_audioFile) return;
    
    int leftMargin = 50;
    int cursorX = m_currentMousePos.x();
    
    // Desenhar apenas se estiver na área de desenho
    if (cursorX < leftMargin || cursorX > width()) return;
    
    // Linha vertical cinza pontilhada
    painter.setPen(QPen(QColor(100, 100, 100), 1, Qt::DashLine));
    painter.drawLine(cursorX, 35, cursorX, height() - 25);
}

void AudioVisualizationWidget::mousePressEvent(QMouseEvent *event)
{
    if (!m_audioFile) return;
    
    if (event->button() == Qt::LeftButton) {
        // Verificar se Shift está pressionado para seleção
        if (event->modifiers() & Qt::ShiftModifier) {
            m_isSelecting = true;
            m_dragStartX = event->pos().x();
            double clickTime = pixelToTime(event->pos().x());
            m_selectionStart = clickTime;
            m_selectionEnd = clickTime;
            m_hasSelection = true;
        } else if (event->modifiers() & Qt::ControlModifier) {
            // Ctrl + Clique: Posicionar reprodução
            double clickTime = pixelToTime(event->pos().x());
            LOG_AUDIO(QString("Ctrl+Clique: Posicionando em %1s").arg(clickTime));
            emit timeClicked(clickTime);
        } else {
            // Pan normal
            m_isDragging = true;
            m_lastMousePos = event->pos();
            setCursor(Qt::ClosedHandCursor);
        }
    }
}

void AudioVisualizationWidget::mouseMoveEvent(QMouseEvent *event)
{
    // Atualizar posição do mouse
    m_currentMousePos = event->pos();
    
    if (!m_audioFile) {
        update();
        return;
    }
    
    if (m_isSelecting) {
        // Atualizar seleção
        double currentTime = pixelToTime(event->pos().x());
        m_selectionEnd = currentTime;
        
        update();
        // NÃO emitir sinal aqui - só no mouseRelease
    } else if (m_isDragging) {
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
    } else {
        // Apenas atualizar para redesenhar o cursor
        update();
    }
}

void AudioVisualizationWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (m_isSelecting) {
            m_isSelecting = false;
            // Garantir ordem correta da seleção
            if (m_selectionEnd < m_selectionStart) {
                std::swap(m_selectionStart, m_selectionEnd);
            }
            // Se a seleção for muito pequena (< 10ms), limpar
            if (qAbs(m_selectionEnd - m_selectionStart) < 0.01) {
                clearTimeSelection();
            } else {
                // Emitir sinal apenas uma vez ao finalizar seleção
                emit timeSelectionChanged(m_selectionStart, m_selectionEnd);
            }
        }
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

void AudioVisualizationWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Escape) {
        if (m_hasSelection) {
            clearTimeSelection();
        }
    }
    QWidget::keyPressEvent(event);
}

double AudioVisualizationWidget::pixelToTime(int pixel) const
{
    int leftMargin = 50;
    int drawWidth = width() - leftMargin;
    int adjustedPixel = pixel - leftMargin;
    return m_viewStartTime + (adjustedPixel / (double)drawWidth) * m_viewDuration;
}

int AudioVisualizationWidget::timeToPixel(double time) const
{
    int leftMargin = 50;
    int drawWidth = width() - leftMargin;
    return leftMargin + (int)((time - m_viewStartTime) / m_viewDuration * drawWidth);
}

void AudioVisualizationWidget::updateSpectrogramVisibility()
{
    // Stub implementation
}

void AudioVisualizationWidget::enterEvent(QEnterEvent *event)
{
    m_mouseInWidget = true;
    update();
    QWidget::enterEvent(event);
}

void AudioVisualizationWidget::leaveEvent(QEvent *event)
{
    m_mouseInWidget = false;
    update();
    QWidget::leaveEvent(event);
}

void AudioVisualizationWidget::setPlaybackPosition(double timeSeconds)
{
    m_playbackPosition = timeSeconds;
    // Não fazer update aqui - o timer já faz isso
    // update();
}

void AudioVisualizationWidget::setPlaying(bool playing)
{
    LOG_AUDIO(QString("setPlaying(%1) - Timer estava: %2").arg(playing).arg(m_updateTimer->isActive()));
    m_isPlaying = playing;
    if (playing) {
        m_updateTimer->start();
        LOG_AUDIO("Timer de atualização INICIADO");
    } else {
        m_updateTimer->stop();
        LOG_AUDIO("Timer de atualização PARADO");
    }
    update();
}
