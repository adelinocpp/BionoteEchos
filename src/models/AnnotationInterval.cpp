#include "models/AnnotationInterval.h"

AnnotationInterval::AnnotationInterval(double xmin, double xmax, const QString &text, QObject *parent)
    : QObject(parent)
    , m_xmin(xmin)
    , m_xmax(xmax)
    , m_text(text)
{
}

AnnotationInterval::~AnnotationInterval()
{
}

void AnnotationInterval::setXMin(double xmin)
{
    if (m_xmin != xmin) {
        m_xmin = xmin;
        emit boundsChanged();
    }
}

void AnnotationInterval::setXMax(double xmax)
{
    if (m_xmax != xmax) {
        m_xmax = xmax;
        emit boundsChanged();
    }
}

void AnnotationInterval::setText(const QString &text)
{
    if (m_text != text) {
        m_text = text;
        emit textChanged(text);
    }
}

void AnnotationInterval::setBounds(double xmin, double xmax)
{
    if (m_xmin != xmin || m_xmax != xmax) {
        m_xmin = xmin;
        m_xmax = xmax;
        emit boundsChanged();
    }
}

bool AnnotationInterval::contains(double time) const
{
    return time >= m_xmin && time <= m_xmax;
}

