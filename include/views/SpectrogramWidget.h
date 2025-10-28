#ifndef SPECTROGRAMWIDGET_H
#define SPECTROGRAMWIDGET_H

#include <QWidget>

class SpectrogramWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SpectrogramWidget(QWidget *parent = nullptr);
    ~SpectrogramWidget();
};

#endif // SPECTROGRAMWIDGET_H
