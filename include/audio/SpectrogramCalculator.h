#ifndef SPECTROGRAMCALCULATOR_H
#define SPECTROGRAMCALCULATOR_H

#include <QObject>

class SpectrogramCalculator : public QObject
{
    Q_OBJECT
public:
    explicit SpectrogramCalculator(QObject *parent = nullptr);
    ~SpectrogramCalculator();
};

#endif // SPECTROGRAMCALCULATOR_H
