#include "models/AnnotationPoint.h"
#include <cmath>

AnnotationPoint::AnnotationPoint(double time, const QString &text, QObject *parent)
    : QObject(parent)
    , m_time(time)
    , m_text(text)
{
}

AnnotationPoint::~AnnotationPoint()
{
}

void AnnotationPoint::setTime(double time)
{
    if (m_time != time) {
        m_time = time;
        emit timeChanged(time);
    }
}

void AnnotationPoint::setText(const QString &text)
{
    if (m_text != text) {
        m_text = text;
        emit textChanged(text);
    }
}

double AnnotationPoint::distanceTo(double time) const
{
    return std::abs(m_time - time);
}

