#ifndef AUDIOFILE_H
#define AUDIOFILE_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QImage>
#include <memory>

/**
 * @brief Representa um arquivo de áudio com seus metadados e dados de amostra
 * 
 * Esta classe armazena informações sobre um arquivo de áudio, incluindo:
 * - Caminho do arquivo
 * - Metadados (taxa de amostragem, número de canais, duração, codec)
 * - Dados de amostra (waveform)
 * - Objetos associados (pitch, intensidade, etc.)
 */
class AudioFile : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Construtor padrão
     * @param parent Objeto pai Qt
     */
    explicit AudioFile(QObject *parent = nullptr);

    /**
     * @brief Construtor com caminho de arquivo
     * @param filePath Caminho do arquivo de áudio
     * @param parent Objeto pai Qt
     */
    explicit AudioFile(const QString &filePath, QObject *parent = nullptr);

    /**
     * @brief Destrutor
     */
    ~AudioFile();

    // Getters
    QString getFilePath() const { return m_filePath; }
    QString getFileName() const;
    int getSampleRate() const { return m_sampleRate; }
    int getNumChannels() const { return m_numChannels; }
    int getNumSamples() const { return m_numSamples; }
    double getDuration() const { return m_duration; }
    QString getCodec() const { return m_codec; }
    int getBitDepth() const { return m_bitDepth; }
    qint64 getFileSize() const { return m_fileSize; }
    
    const QVector<float>& getSamples(int channel = 0) const;
    QVector<float> getMixedSamples() const;
    
    bool isLoaded() const { return m_loaded; }
    bool hasPitchData() const { return m_hasPitchData; }
    bool hasIntensityData() const { return m_hasIntensityData; }
    
    const QVector<float>& getPitchData() const { return m_pitchData; }
    const QVector<float>& getIntensityData() const { return m_intensityData; }
    
    // Setters
    void setFilePath(const QString &filePath) { m_filePath = filePath; }
    void setSampleRate(int sampleRate) { m_sampleRate = sampleRate; }
    void setNumChannels(int numChannels);
    void setNumSamples(int numSamples) { m_numSamples = numSamples; }
    void setDuration(double duration) { m_duration = duration; }
    void setCodec(const QString &codec) { m_codec = codec; }
    void setBitDepth(int bitDepth) { m_bitDepth = bitDepth; }
    void setFileSize(qint64 fileSize) { m_fileSize = fileSize; }
    
    void setSamples(int channel, const QVector<float> &samples);
    void setPitchData(const QVector<float> &pitchData);
    void setIntensityData(const QVector<float> &intensityData);
    
    // Spectrogram cache
    bool hasSpectrogramCache(const QString &settingsHash) const { 
        return !m_spectrogramCache.isNull() && m_spectrogramCacheHash == settingsHash; 
    }
    QImage getSpectrogramCache() const { return m_spectrogramCache; }
    void setSpectrogramCache(const QImage &spectrogram, const QString &settingsHash) { 
        m_spectrogramCache = spectrogram; 
        m_spectrogramCacheHash = settingsHash;
    }
    void clearSpectrogramCache() { 
        m_spectrogramCache = QImage(); 
        m_spectrogramCacheHash.clear();
    }
    
    /**
     * @brief Carrega o arquivo de áudio
     * @return true se carregado com sucesso, false caso contrário
     */
    bool load();
    
    /**
     * @brief Descarrega os dados de amostra da memória
     */
    void unload();
    
    /**
     * @brief Obtém amostra em uma posição temporal específica
     * @param timeSeconds Tempo em segundos
     * @param channel Canal (0 = esquerdo, 1 = direito, etc.)
     * @return Valor da amostra
     */
    float getSampleAtTime(double timeSeconds, int channel = 0) const;
    
    /**
     * @brief Obtém pitch em uma posição temporal específica
     * @param timeSeconds Tempo em segundos
     * @return Valor do pitch em Hz (0 se unvoiced)
     */
    float getPitchAtTime(double timeSeconds) const;
    
    /**
     * @brief Obtém intensidade em uma posição temporal específica
     * @param timeSeconds Tempo em segundos
     * @return Valor da intensidade em dB
     */
    float getIntensityAtTime(double timeSeconds) const;

signals:
    /**
     * @brief Sinal emitido quando o arquivo é carregado
     */
    void loaded();
    
    /**
     * @brief Sinal emitido quando o arquivo é descarregado
     */
    void unloaded();
    
    /**
     * @brief Sinal emitido quando os dados de pitch são calculados
     */
    void pitchDataCalculated();
    
    /**
     * @brief Sinal emitido quando os dados de intensidade são calculados
     */
    void intensityDataCalculated();

private:
    QString m_filePath;
    int m_sampleRate;
    int m_numChannels;
    int m_numSamples;
    double m_duration;
    QString m_codec;
    int m_bitDepth;
    qint64 m_fileSize;
    
    bool m_loaded;
    QVector<QVector<float>> m_channelSamples;
    
    bool m_hasPitchData;
    QVector<float> m_pitchData;
    
    bool m_hasIntensityData;
    QVector<float> m_intensityData;
    
    // Spectrogram cache
    QImage m_spectrogramCache;
    QString m_spectrogramCacheHash;
};

#endif // AUDIOFILE_H

