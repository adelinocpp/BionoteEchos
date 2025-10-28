#include "views/SpectrogramWidget.h"
#include "audio/SpectrogramCalculator.h"
#include "models/AudioFile.h"
#include <QPainter>
#include <QThread>
#include <QWheelEvent>
#include <QMouseEvent>

SpectrogramWidget::SpectrogramWidget(QWidget *parent) 
    : QWidget(parent)
    , m_viewStartTime(0.0)
    , m_viewDuration(10.0)
    , m_playbackPosition(0.0)
    , m_isCalculating(false)
    , m_calculationProgress(0)
    , m_isDragging(false)
    , m_dragStartX(0)
    , m_dragStartTime(0.0)
    , m_calculatorThread(nullptr)
    , m_calculator(nullptr)
{
    setMinimumHeight(150);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    // Criar calculador em thread separada
    m_calculatorThread = new QThread(this);
    m_calculatorThread->setObjectName("SpectrogramCalculatorThread");
    
    m_calculator = new SpectrogramCalculator();
    m_calculator->moveToThread(m_calculatorThread);
    
    // Conectar sinais
    connect(m_calculator, &SpectrogramCalculator::calculationStarted,
            this, &SpectrogramWidget::calculationStarted);
    connect(m_calculator, &SpectrogramCalculator::calculationProgress,
            this, &SpectrogramWidget::onCalculationProgress);
    connect(m_calculator, &SpectrogramCalculator::calculationFinished,
            this, &SpectrogramWidget::onCalculationFinished);
    connect(m_calculator, &SpectrogramCalculator::calculationError,
            this, &SpectrogramWidget::onCalculationError);
    
    // Conectar sinal interno para iniciar cálculo na thread
    connect(m_calculator, &SpectrogramCalculator::startCalculationInThread,
            m_calculator, &SpectrogramCalculator::performCalculation,
            Qt::QueuedConnection);
    
    // Iniciar thread
    m_calculatorThread->start();
}

SpectrogramWidget::~SpectrogramWidget()
{
    if (m_calculator) {
        m_calculator->cancel();
    }
    
    if (m_calculatorThread) {
        m_calculatorThread->quit();
        m_calculatorThread->wait();
    }
}

void SpectrogramWidget::setAudioFile(std::shared_ptr<AudioFile> audioFile)
{
    m_audioFile = audioFile;
    
    if (m_audioFile) {
        // Verificar se já existe cache com as configurações atuais
        QString settingsHash = getSettingsHash();
        if (m_audioFile->hasSpectrogramCache(settingsHash)) {
            m_spectrogramImage = m_audioFile->getSpectrogramCache();
            m_isCalculating = false;
            m_calculationProgress = 100;
        } else {
            m_spectrogramImage = QImage();  // Limpar espectrograma anterior
            calculateSpectrogram();
        }
    } else {
        m_spectrogramImage = QImage();
    }
    
    update();
}

void SpectrogramWidget::setSettings(const Settings &settings)
{
    m_settings = settings;
    
    // Limpar cache pois as configurações mudaram
    if (m_audioFile) {
        m_audioFile->clearSpectrogramCache();
    }
    
    if (m_audioFile) {
        calculateSpectrogram();
    }
}

void SpectrogramWidget::setPlaybackPosition(double timeSeconds)
{
    m_playbackPosition = timeSeconds;
    update();
}

void SpectrogramWidget::setVisibleTimeRange(double startTime, double duration)
{
    m_viewStartTime = startTime;
    m_viewDuration = duration;
    update();
}

void SpectrogramWidget::calculateSpectrogram()
{
    if (!m_audioFile || m_isCalculating) {
        return;
    }
    
    // Converter Settings para Parameters
    SpectrogramCalculator::Parameters params;
    params.timeStep = m_settings.timeStep;
    params.timeWindow = m_settings.timeWindow;
    params.fftSize = m_settings.fftSize;
    params.windowType = m_settings.windowType;
    params.minFrequency = m_settings.minFrequency;
    params.maxFrequency = m_settings.maxFrequency;
    params.dynamicRange = m_settings.dynamicRange;
    params.colorMap = m_settings.colorMap;
    params.preEmphasis = m_settings.preEmphasis;
    params.preEmphasisFactor = m_settings.preEmphasisFactor;
    params.maxDuration = 20.0;
    
    // Se a janela visível for menor que 20s, calcular apenas a janela
    if (m_viewDuration < 20.0 && m_viewDuration > 0) {
        params.startTime = m_viewStartTime;
        params.windowDuration = m_viewDuration;
    } else {
        // Calcular os primeiros 20s do áudio
        params.startTime = 0.0;
        params.windowDuration = 0.0; // 0 = usar maxDuration
    }
    
    m_isCalculating = true;
    m_calculator->calculate(m_audioFile, params);
}

void SpectrogramWidget::onCalculationFinished(QImage spectrogram)
{
    m_spectrogramImage = spectrogram;
    m_isCalculating = false;
    m_calculationProgress = 100;
    
    // Salvar no cache do AudioFile com hash das configurações
    if (m_audioFile && !spectrogram.isNull()) {
        QString settingsHash = getSettingsHash();
        m_audioFile->setSpectrogramCache(spectrogram, settingsHash);
    }
    
    emit calculationFinished();
    update();
}

void SpectrogramWidget::onCalculationProgress(int percent)
{
    m_calculationProgress = percent;
    emit calculationProgress(percent);
    update();
}

void SpectrogramWidget::onCalculationError(QString error)
{
    m_isCalculating = false;
    emit calculationError(error);
    update();
}

void SpectrogramWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Fundo branco quando não calculado, preto quando calculado
    if (m_spectrogramImage.isNull()) {
        painter.fillRect(rect(), Qt::white);
    } else {
        painter.fillRect(rect(), Qt::black);
    }
    
    if (m_isCalculating) {
        // Mostrar progresso (fundo branco, texto preto)
        painter.setPen(Qt::black);
        painter.drawText(rect(), Qt::AlignCenter, 
                        QString("Calculando espectrograma... %1%").arg(m_calculationProgress));
        return;
    }
    
    if (m_spectrogramImage.isNull()) {
        painter.setPen(Qt::black);
        painter.drawText(rect(), Qt::AlignCenter, "Espectrograma não calculado");
        return;
    }
    
    drawSpectrogram(painter);
    drawFrequencyAxis(painter);
    drawPlaybackCursor(painter);
}

void SpectrogramWidget::drawSpectrogram(QPainter &painter)
{
    if (m_spectrogramImage.isNull() || !m_audioFile) {
        return;
    }
    
    int leftMargin = 50;
    int rightMargin = 10;
    int topMargin = 10;
    int bottomMargin = 10;
    
    int drawWidth = width() - leftMargin - rightMargin;
    int drawHeight = height() - topMargin - bottomMargin;
    
    if (drawWidth <= 0 || drawHeight <= 0) {
        return;
    }
    
    // Calcular região visível da imagem
    double audioDuration = std::min(m_audioFile->getDuration(), 20.0);
    double timePerPixel = audioDuration / m_spectrogramImage.width();
    
    int startPixel = static_cast<int>(m_viewStartTime / timePerPixel);
    int endPixel = static_cast<int>((m_viewStartTime + m_viewDuration) / timePerPixel);
    
    startPixel = std::max(0, std::min(startPixel, m_spectrogramImage.width() - 1));
    endPixel = std::max(startPixel + 1, std::min(endPixel, m_spectrogramImage.width()));
    
    int visibleWidth = endPixel - startPixel;
    
    // Extrair região visível
    QImage visibleRegion = m_spectrogramImage.copy(startPixel, 0, visibleWidth, m_spectrogramImage.height());
    
    // Desenhar espectrograma escalado
    QRect targetRect(leftMargin, topMargin, drawWidth, drawHeight);
    painter.drawImage(targetRect, visibleRegion);
}

void SpectrogramWidget::drawFrequencyAxis(QPainter &painter)
{
    int leftMargin = 50;
    int topMargin = 10;
    int bottomMargin = 10;
    int drawHeight = height() - topMargin - bottomMargin;
    
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 8));
    
    // Desenhar escala de frequência
    double maxFreq = m_settings.maxFrequency;
    int numTicks = 5;
    
    for (int i = 0; i <= numTicks; ++i) {
        double freq = (maxFreq * i) / numTicks;
        int y = topMargin + drawHeight - (i * drawHeight / numTicks);
        
        painter.drawLine(leftMargin - 5, y, leftMargin, y);
        painter.drawText(QRect(0, y - 10, leftMargin - 10, 20), 
                        Qt::AlignRight | Qt::AlignVCenter,
                        QString::number(freq / 1000.0, 'f', 1) + " kHz");
    }
}

void SpectrogramWidget::drawPlaybackCursor(QPainter &painter)
{
    if (m_playbackPosition < m_viewStartTime || 
        m_playbackPosition > m_viewStartTime + m_viewDuration) {
        return;
    }
    
    int leftMargin = 50;
    int rightMargin = 10;
    int drawWidth = width() - leftMargin - rightMargin;
    
    double relativeTime = m_playbackPosition - m_viewStartTime;
    int x = leftMargin + static_cast<int>((relativeTime / m_viewDuration) * drawWidth);
    
    painter.setPen(QPen(Qt::red, 2));
    painter.drawLine(x, 0, x, height());
}

QColor SpectrogramWidget::valueToColor(float value) const
{
    // Implementação básica - será substituída pelo calculador
    int gray = static_cast<int>(value * 255);
    return QColor(gray, gray, gray);
}

void SpectrogramWidget::wheelEvent(QWheelEvent *event)
{
    if (!m_audioFile) {
        QWidget::wheelEvent(event);
        return;
    }
    
    // Zoom com scroll do mouse
    double factor = event->angleDelta().y() > 0 ? 0.8 : 1.2;
    
    // Calcular ponto focal do zoom (posição do mouse)
    int leftMargin = 50;
    int drawWidth = width() - leftMargin;
    double mouseRelativePos = (event->position().x() - leftMargin) / (double)drawWidth;
    double mouseTime = m_viewStartTime + mouseRelativePos * m_viewDuration;
    
    // Aplicar zoom
    double newDuration = m_viewDuration * factor;
    double maxDuration = m_audioFile ? m_audioFile->getDuration() : 10.0;
    newDuration = qMax(0.01, qMin(newDuration, maxDuration));
    
    // Ajustar viewStartTime para manter o ponto focal
    m_viewStartTime = mouseTime - mouseRelativePos * newDuration;
    m_viewStartTime = qMax(0.0, qMin(m_viewStartTime, maxDuration - newDuration));
    m_viewDuration = newDuration;
    
    emit visibleTimeRangeChanged(m_viewStartTime, m_viewDuration);
    update();
    event->accept();
}

void SpectrogramWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDragging = true;
        m_dragStartX = event->pos().x();
        m_dragStartTime = m_viewStartTime;
        setCursor(Qt::ClosedHandCursor);
        event->accept();
    } else {
        QWidget::mousePressEvent(event);
    }
}

void SpectrogramWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isDragging && m_audioFile) {
        int leftMargin = 50;
        int drawWidth = width() - leftMargin;
        int dx = event->pos().x() - m_dragStartX;
        
        // Converter deslocamento de pixels para tempo
        double timeDelta = -(dx / (double)drawWidth) * m_viewDuration;
        
        // Atualizar viewStartTime
        m_viewStartTime = m_dragStartTime + timeDelta;
        
        // Limitar aos bounds do áudio
        double maxDuration = m_audioFile->getDuration();
        m_viewStartTime = qMax(0.0, qMin(m_viewStartTime, maxDuration - m_viewDuration));
        
        emit visibleTimeRangeChanged(m_viewStartTime, m_viewDuration);
        update();
        event->accept();
    } else {
        QWidget::mouseMoveEvent(event);
    }
}

void SpectrogramWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_isDragging) {
        m_isDragging = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
    } else {
        QWidget::mouseReleaseEvent(event);
    }
}

QString SpectrogramWidget::getSettingsHash() const
{
    // Criar hash único baseado nas configurações
    return QString("%1_%2_%3_%4_%5_%6_%7_%8_%9_%10")
        .arg(m_settings.timeStep, 0, 'f', 6)
        .arg(m_settings.timeWindow, 0, 'f', 6)
        .arg(m_settings.fftSize)
        .arg(m_settings.windowType)
        .arg(m_settings.minFrequency, 0, 'f', 1)
        .arg(m_settings.maxFrequency, 0, 'f', 1)
        .arg(m_settings.dynamicRange, 0, 'f', 1)
        .arg(m_settings.colorMap)
        .arg(m_settings.preEmphasis ? 1 : 0)
        .arg(m_settings.preEmphasisFactor, 0, 'f', 3);
}
