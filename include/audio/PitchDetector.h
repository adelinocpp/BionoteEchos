#ifndef PITCHDETECTOR_H
#define PITCHDETECTOR_H

#include <QObject>

class PitchDetector : public QObject
{
    Q_OBJECT
public:
    explicit PitchDetector(QObject *parent = nullptr);
    ~PitchDetector();
};

#endif // PITCHDETECTOR_H
