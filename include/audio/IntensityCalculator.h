#ifndef INTENSITYCALCULATOR_H
#define INTENSITYCALCULATOR_H

#include <QObject>

class IntensityCalculator : public QObject
{
    Q_OBJECT
public:
    explicit IntensityCalculator(QObject *parent = nullptr);
    ~IntensityCalculator();
};

#endif // INTENSITYCALCULATOR_H
