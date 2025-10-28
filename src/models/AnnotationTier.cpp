#include "models/AnnotationTier.h"
#include "models/AnnotationInterval.h"
#include "models/AnnotationPoint.h"
#include <algorithm>

AnnotationTier::AnnotationTier(const QString &name, TierType type, double xmin, double xmax, QObject *parent)
    : QObject(parent)
    , m_name(name)
    , m_type(type)
    , m_xmin(xmin)
    , m_xmax(xmax)
{
}

AnnotationTier::~AnnotationTier()
{
}

int AnnotationTier::getIntervalCount() const
{
    return m_intervals.size();
}

int AnnotationTier::getPointCount() const
{
    return m_points.size();
}

std::shared_ptr<AnnotationInterval> AnnotationTier::getInterval(int index) const
{
    if (index >= 0 && index < m_intervals.size()) {
        return m_intervals[index];
    }
    return nullptr;
}

std::shared_ptr<AnnotationPoint> AnnotationTier::getPoint(int index) const
{
    if (index >= 0 && index < m_points.size()) {
        return m_points[index];
    }
    return nullptr;
}

void AnnotationTier::setName(const QString &name)
{
    if (m_name != name) {
        m_name = name;
        emit nameChanged(name);
    }
}

std::shared_ptr<AnnotationInterval> AnnotationTier::addInterval(double xmin, double xmax, const QString &text)
{
    if (m_type != TierType::IntervalTier) {
        return nullptr;
    }
    
    auto interval = std::make_shared<AnnotationInterval>(xmin, xmax, text, this);
    m_intervals.append(interval);
    sortIntervals();
    
    int index = m_intervals.indexOf(interval);
    emit intervalAdded(index);
    
    return interval;
}

std::shared_ptr<AnnotationPoint> AnnotationTier::addPoint(double time, const QString &text)
{
    if (m_type != TierType::PointTier) {
        return nullptr;
    }
    
    auto point = std::make_shared<AnnotationPoint>(time, text, this);
    m_points.append(point);
    sortPoints();
    
    int index = m_points.indexOf(point);
    emit pointAdded(index);
    
    return point;
}

bool AnnotationTier::removeInterval(int index)
{
    if (index >= 0 && index < m_intervals.size()) {
        m_intervals.removeAt(index);
        emit intervalRemoved(index);
        return true;
    }
    return false;
}

bool AnnotationTier::removePoint(int index)
{
    if (index >= 0 && index < m_points.size()) {
        m_points.removeAt(index);
        emit pointRemoved(index);
        return true;
    }
    return false;
}

int AnnotationTier::findIntervalAtTime(double time) const
{
    for (int i = 0; i < m_intervals.size(); ++i) {
        if (m_intervals[i]->contains(time)) {
            return i;
        }
    }
    return -1;
}

int AnnotationTier::findPointNearTime(double time, double tolerance) const
{
    int nearestIndex = -1;
    double minDistance = tolerance;
    
    for (int i = 0; i < m_points.size(); ++i) {
        double distance = m_points[i]->distanceTo(time);
        if (distance < minDistance) {
            minDistance = distance;
            nearestIndex = i;
        }
    }
    
    return nearestIndex;
}

void AnnotationTier::clear()
{
    m_intervals.clear();
    m_points.clear();
}

bool AnnotationTier::isEmpty() const
{
    return m_intervals.isEmpty() && m_points.isEmpty();
}

void AnnotationTier::sortIntervals()
{
    std::sort(m_intervals.begin(), m_intervals.end(),
              [](const std::shared_ptr<AnnotationInterval> &a, const std::shared_ptr<AnnotationInterval> &b) {
                  return a->getXMin() < b->getXMin();
              });
}

void AnnotationTier::sortPoints()
{
    std::sort(m_points.begin(), m_points.end(),
              [](const std::shared_ptr<AnnotationPoint> &a, const std::shared_ptr<AnnotationPoint> &b) {
                  return a->getTime() < b->getTime();
              });
}

