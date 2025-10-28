#include "views/AudioControlWidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

AudioControlWidget::AudioControlWidget(QWidget *parent)
    : QWidget(parent)
    , m_isPlaying(false)
    , m_duration(0.0)
    , m_position(0.0)
    , m_volume(75)
    , m_updatingSlider(false)
{
    setupUI();
}

AudioControlWidget::~AudioControlWidget()
{
}

void AudioControlWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Position slider
    m_positionSlider = new QSlider(Qt::Horizontal, this);
    m_positionSlider->setRange(0, 1000);
    m_positionSlider->setValue(0);
    connect(m_positionSlider, &QSlider::sliderMoved, this, &AudioControlWidget::onPositionSliderMoved);
    
    // Control buttons layout
    QHBoxLayout *controlLayout = new QHBoxLayout();
    
    m_playPauseButton = new QPushButton("Play", this);
    connect(m_playPauseButton, &QPushButton::clicked, this, &AudioControlWidget::onPlayPauseClicked);
    
    m_stopButton = new QPushButton("Stop", this);
    connect(m_stopButton, &QPushButton::clicked, this, &AudioControlWidget::onStopClicked);
    
    m_loopCheckBox = new QCheckBox("Loop", this);
    connect(m_loopCheckBox, &QCheckBox::toggled, this, &AudioControlWidget::onLoopCheckBoxToggled);
    
    m_currentTimeLabel = new QLabel("0:00.000", this);
    m_totalTimeLabel = new QLabel("0:00.000", this);
    
    m_volumeSlider = new QSlider(Qt::Horizontal, this);
    m_volumeSlider->setRange(0, 100);
    m_volumeSlider->setValue(m_volume);
    m_volumeSlider->setMaximumWidth(100);
    connect(m_volumeSlider, &QSlider::valueChanged, this, &AudioControlWidget::onVolumeSliderMoved);
    
    m_volumeLabel = new QLabel(QString::number(m_volume) + "%", this);
    
    controlLayout->addWidget(m_playPauseButton);
    controlLayout->addWidget(m_stopButton);
    controlLayout->addWidget(m_loopCheckBox);
    controlLayout->addStretch();
    controlLayout->addWidget(m_currentTimeLabel);
    controlLayout->addWidget(new QLabel("/", this));
    controlLayout->addWidget(m_totalTimeLabel);
    controlLayout->addStretch();
    controlLayout->addWidget(new QLabel("Volume:", this));
    controlLayout->addWidget(m_volumeSlider);
    controlLayout->addWidget(m_volumeLabel);
    
    mainLayout->addWidget(m_positionSlider);
    mainLayout->addLayout(controlLayout);
}

void AudioControlWidget::setDuration(double duration)
{
    m_duration = duration;
    updateTimeLabels();
}

void AudioControlWidget::setPosition(double position)
{
    m_position = position;
    if (!m_updatingSlider) {
        m_updatingSlider = true;
        int sliderValue = (int)((position / m_duration) * 1000);
        m_positionSlider->setValue(sliderValue);
        m_updatingSlider = false;
    }
    updateTimeLabels();
}

void AudioControlWidget::setPlaying(bool playing)
{
    m_isPlaying = playing;
    m_playPauseButton->setText(playing ? "Pause" : "Play");
}

void AudioControlWidget::setLoopMode(bool loop)
{
    m_loopCheckBox->setChecked(loop);
}

bool AudioControlWidget::isLoopMode() const
{
    return m_loopCheckBox->isChecked();
}

void AudioControlWidget::setVolume(int volume)
{
    m_volume = volume;
    m_volumeSlider->setValue(volume);
    m_volumeLabel->setText(QString::number(volume) + "%");
}

int AudioControlWidget::getVolume() const
{
    return m_volume;
}

void AudioControlWidget::onPlayPauseClicked()
{
    if (m_isPlaying) {
        emit pauseClicked();
    } else {
        emit playClicked();
    }
}

void AudioControlWidget::onStopClicked()
{
    emit stopClicked();
}

void AudioControlWidget::onLoopCheckBoxToggled(bool checked)
{
    emit loopModeChanged(checked);
}

void AudioControlWidget::onPositionSliderMoved(int value)
{
    if (!m_updatingSlider) {
        double position = (value / 1000.0) * m_duration;
        emit positionChanged(position);
    }
}

void AudioControlWidget::onVolumeSliderMoved(int value)
{
    m_volume = value;
    m_volumeLabel->setText(QString::number(value) + "%");
    emit volumeChanged(value);
}

void AudioControlWidget::updateTimeLabels()
{
    m_currentTimeLabel->setText(formatTime(m_position));
    m_totalTimeLabel->setText(formatTime(m_duration));
}

QString AudioControlWidget::formatTime(double seconds) const
{
    int mins = (int)(seconds / 60);
    double secs = seconds - (mins * 60);
    return QString("%1:%2").arg(mins).arg(secs, 6, 'f', 3, '0');
}
