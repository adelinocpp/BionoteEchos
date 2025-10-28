#ifndef SPECTROGRAMWIDGET_H
#define SPECTROGRAMWIDGET_H

#include <QObject>

class SpectrogramWidget : public QObject
{
    Q_OBJECT
public:
    explicit SpectrogramWidget(QObject *parent = nullptr);
    ~SpectrogramWidget();
};

#endif // SPECTROGRAMWIDGET_H
