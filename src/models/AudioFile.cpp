#include "models/AudioFile.h"
#include <QFileInfo>
#include <QDebug>

AudioFile::AudioFile(QObject *parent)
    : QObject(parent)
    , m_sampleRate(0)
    , m_numChannels(0)
    , m_numSamples(0)
    , m_duration(0.0)
    , m_bitDepth(0)
    , m_fileSize(0)
    , m_loaded(false)
    , m_hasPitchData(false)
    , m_hasIntensityData(false)
{
}

AudioFile::AudioFile(const QString &filePath, QObject *parent)
    : QObject(parent)
    , m_filePath(filePath)
    , m_sampleRate(0)
    , m_numChannels(0)
    , m_numSamples(0)
    , m_duration(0.0)
    , m_bitDepth(0)
    , m_fileSize(0)
    , m_loaded(false)
    , m_hasPitchData(false)
    , m_hasIntensityData(false)
{
    QFileInfo fileInfo(filePath);
    m_fileSize = fileInfo.size();
}

AudioFile::~AudioFile()
{
    unload();
}

QString AudioFile::getFileName() const
{
    QFileInfo fileInfo(m_filePath);
    return fileInfo.fileName();
}

const QVector<float>& AudioFile::getSamples(int channel) const
{
    static QVector<float> empty;
    if (channel >= 0 && channel < m_channelSamples.size()) {
        return m_channelSamples[channel];
    }
    return empty;
}

QVector<float> AudioFile::getMixedSamples() const
{
    if (m_channelSamples.isEmpty()) {
        return QVector<float>();
    }
    
    if (m_channelSamples.size() == 1) {
        return m_channelSamples[0];
    }
    
    // Mix all channels
    QVector<float> mixed(m_numSamples, 0.0f);
    for (int i = 0; i < m_numSamples; ++i) {
        float sum = 0.0f;
        for (int ch = 0; ch < m_numChannels; ++ch) {
            if (i < m_channelSamples[ch].size()) {
                sum += m_channelSamples[ch][i];
            }
        }
        mixed[i] = sum / m_numChannels;
    }
    
    return mixed;
}

void AudioFile::setNumChannels(int numChannels)
{
    m_numChannels = numChannels;
    m_channelSamples.resize(numChannels);
}

void AudioFile::setSamples(int channel, const QVector<float> &samples)
{
    if (channel >= 0 && channel < m_numChannels) {
        m_channelSamples[channel] = samples;
        if (m_numSamples == 0) {
            m_numSamples = samples.size();
            if (m_sampleRate > 0) {
                m_duration = static_cast<double>(m_numSamples) / m_sampleRate;
            }
        }
    }
}

void AudioFile::setPitchData(const QVector<float> &pitchData)
{
    m_pitchData = pitchData;
    m_hasPitchData = !pitchData.isEmpty();
    emit pitchDataCalculated();
}

void AudioFile::setIntensityData(const QVector<float> &intensityData)
{
    m_intensityData = intensityData;
    m_hasIntensityData = !intensityData.isEmpty();
    emit intensityDataCalculated();
}

bool AudioFile::load()
{
    // Esta função será implementada pelo AudioDecoder
    // Por enquanto, apenas marca como carregado
    if (!m_loaded) {
        m_loaded = true;
        emit loaded();
        return true;
    }
    return false;
}

void AudioFile::unload()
{
    if (m_loaded) {
        m_channelSamples.clear();
        m_pitchData.clear();
        m_intensityData.clear();
        m_hasPitchData = false;
        m_hasIntensityData = false;
        m_loaded = false;
        emit unloaded();
    }
}

float AudioFile::getSampleAtTime(double timeSeconds, int channel) const
{
    if (!m_loaded || channel < 0 || channel >= m_numChannels) {
        return 0.0f;
    }
    
    int sampleIndex = static_cast<int>(timeSeconds * m_sampleRate);
    if (sampleIndex >= 0 && sampleIndex < m_channelSamples[channel].size()) {
        return m_channelSamples[channel][sampleIndex];
    }
    
    return 0.0f;
}

float AudioFile::getPitchAtTime(double timeSeconds) const
{
    if (!m_hasPitchData || m_pitchData.isEmpty()) {
        return 0.0f;
    }
    
    // Assumindo que pitch data tem uma taxa de quadros específica
    // Por simplicidade, assumimos 100 Hz (10ms por frame)
    int frameIndex = static_cast<int>(timeSeconds * 100.0);
    if (frameIndex >= 0 && frameIndex < m_pitchData.size()) {
        return m_pitchData[frameIndex];
    }
    
    return 0.0f;
}

float AudioFile::getIntensityAtTime(double timeSeconds) const
{
    if (!m_hasIntensityData || m_intensityData.isEmpty()) {
        return 0.0f;
    }
    
    // Assumindo que intensity data tem uma taxa de quadros específica
    // Por simplicidade, assumimos 100 Hz (10ms por frame)
    int frameIndex = static_cast<int>(timeSeconds * 100.0);
    if (frameIndex >= 0 && frameIndex < m_intensityData.size()) {
        return m_intensityData[frameIndex];
    }
    
    return 0.0f;
}

