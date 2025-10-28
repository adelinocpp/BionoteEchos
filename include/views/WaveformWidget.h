#ifndef WAVEFORMWIDGET_H
#define WAVEFORMWIDGET_H

#include <QWidget>
#include <memory>

class AudioFile;

/**
 * @brief Widget para visualização da forma de onda
 */
class WaveformWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WaveformWidget(QWidget *parent = nullptr);
    ~WaveformWidget();
    
    void setAudioFile(std::shared_ptr<AudioFile> audioFile);
    void setPlaybackPosition(double timeSeconds);
    void setPlaying(bool playing);
    void setTimeSelection(double startTime, double endTime);
    void clearTimeSelection();
    bool getTimeSelection(double &startTime, double &endTime) const;
    void setVisibleTimeRange(double startTime, double duration);
    void getVisibleTimeRange(double &startTime, double &endTime) const;
    void zoom(double factor);
    void zoomFit();

signals:
    void timeSelectionChanged(double startTime, double endTime);
    void timeSelectionCleared();
    void timeClicked(double timeSeconds);
    void visibleTimeRangeChanged(double startTime, double endTime);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    void drawWaveform(QPainter &painter);
    void drawTimeLabels(QPainter &painter);
    void drawSelection(QPainter &painter);
    void drawPlaybackCursor(QPainter &painter);
    double pixelToTime(int pixel) const;
    int timeToPixel(double time) const;

private:
    std::shared_ptr<AudioFile> m_audioFile;
    double m_viewStartTime;
    double m_viewDuration;
    double m_zoomLevel;
    bool m_hasSelection;
    double m_selectionStart;
    double m_selectionEnd;
    double m_playbackPosition;
    bool m_isPlaying;
    bool m_isDragging;
    bool m_isSelecting;
    QPoint m_lastMousePos;
    int m_dragStartX;
};

#endif // WAVEFORMWIDGET_H
