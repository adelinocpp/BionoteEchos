#include "views/AnnotationLayerWidget.h"
#include "models/AnnotationTier.h"

AnnotationLayerWidget::AnnotationLayerWidget(QWidget *parent)
    : QWidget(parent)
    , m_viewStartTime(0.0)
    , m_viewEndTime(10.0)
    , m_selectedTierIndex(-1)
    , m_selectedIntervalIndex(-1)
    , m_selectedPointIndex(-1)
    , m_isDraggingBoundary(false)
    , m_isEditing(false)
    , m_tierHeight(80)
    , m_tierSpacing(10)
{
    setMinimumHeight(100);
}

AnnotationLayerWidget::~AnnotationLayerWidget()
{
}

void AnnotationLayerWidget::setTier1(std::shared_ptr<AnnotationTier> tier)
{
    m_tier1 = tier;
    update();
}

void AnnotationLayerWidget::setTier2(std::shared_ptr<AnnotationTier> tier)
{
    m_tier2 = tier;
    update();
}

void AnnotationLayerWidget::setVisibleTimeRange(double startTime, double endTime)
{
    m_viewStartTime = startTime;
    m_viewEndTime = endTime;
    update();
}

void AnnotationLayerWidget::clearTiers()
{
    m_tier1.reset();
    m_tier2.reset();
    update();
}

std::shared_ptr<AnnotationTier> AnnotationLayerWidget::getSelectedTier() const
{
    if (m_selectedTierIndex == 0) return m_tier1;
    if (m_selectedTierIndex == 1) return m_tier2;
    return nullptr;
}

void AnnotationLayerWidget::setSelectedTier(int tierIndex)
{
    m_selectedTierIndex = tierIndex;
    update();
}

void AnnotationLayerWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::white);
    
    if (m_tier1) {
        drawTier(painter, m_tier1, 0, m_tierHeight);
    }
    if (m_tier2) {
        drawTier(painter, m_tier2, m_tierHeight + m_tierSpacing, m_tierHeight);
    }
}

void AnnotationLayerWidget::drawTier(QPainter &painter, std::shared_ptr<AnnotationTier> tier, int yOffset, int height)
{
    // Stub implementation
    painter.setPen(Qt::black);
    painter.drawRect(0, yOffset, width(), height);
    painter.drawText(10, yOffset + 20, tier->getName());
}

void AnnotationLayerWidget::drawIntervalTier(QPainter &painter, std::shared_ptr<AnnotationTier> tier, int yOffset, int height)
{
    // Stub implementation
}

void AnnotationLayerWidget::drawPointTier(QPainter &painter, std::shared_ptr<AnnotationTier> tier, int yOffset, int height)
{
    // Stub implementation
}

void AnnotationLayerWidget::mousePressEvent(QMouseEvent *event)
{
    // Stub implementation
}

void AnnotationLayerWidget::mouseMoveEvent(QMouseEvent *event)
{
    // Stub implementation
}

void AnnotationLayerWidget::mouseReleaseEvent(QMouseEvent *event)
{
    // Stub implementation
}

void AnnotationLayerWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    // Stub implementation
}

void AnnotationLayerWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}

double AnnotationLayerWidget::pixelToTime(int pixel) const
{
    return m_viewStartTime + (pixel / (double)width()) * (m_viewEndTime - m_viewStartTime);
}

int AnnotationLayerWidget::timeToPixel(double time) const
{
    return (int)((time - m_viewStartTime) / (m_viewEndTime - m_viewStartTime) * width());
}

int AnnotationLayerWidget::findIntervalAtPosition(std::shared_ptr<AnnotationTier> tier, int x, int y)
{
    return -1;
}

int AnnotationLayerWidget::findPointAtPosition(std::shared_ptr<AnnotationTier> tier, int x, int y)
{
    return -1;
}

int AnnotationLayerWidget::findBoundaryAtPosition(std::shared_ptr<AnnotationTier> tier, int x, int y)
{
    return -1;
}

void AnnotationLayerWidget::startEditingInterval(std::shared_ptr<AnnotationTier> tier, int index)
{
    // Stub implementation
}

void AnnotationLayerWidget::startEditingPoint(std::shared_ptr<AnnotationTier> tier, int index)
{
    // Stub implementation
}

void AnnotationLayerWidget::addIntervalAtPosition(std::shared_ptr<AnnotationTier> tier, double time)
{
    // Stub implementation
}

void AnnotationLayerWidget::addPointAtPosition(std::shared_ptr<AnnotationTier> tier, double time)
{
    // Stub implementation
}
