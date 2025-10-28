#ifndef AUDIODECODER_H
#define AUDIODECODER_H

#include <QObject>
#include <QString>
#include <QVector>
#include <memory>

class AudioFile;

/**
 * @brief Decodificador de arquivos de áudio usando libsndfile
 * 
 * Suporta múltiplos formatos: WAV, FLAC, OGG, OPUS, MP3, etc.
 */
class AudioDecoder : public QObject
{
    Q_OBJECT

public:
    explicit AudioDecoder(QObject *parent = nullptr);
    ~AudioDecoder();
    
    /**
     * @brief Decodifica um arquivo de áudio
     * @param filePath Caminho do arquivo
     * @param audioFile Objeto AudioFile para preencher com os dados
     * @return true se decodificado com sucesso, false caso contrário
     */
    bool decode(const QString &filePath, std::shared_ptr<AudioFile> audioFile);
    
    /**
     * @brief Obtém informações sobre um arquivo sem decodificar
     * @param filePath Caminho do arquivo
     * @param audioFile Objeto AudioFile para preencher apenas metadados
     * @return true se obtido com sucesso
     */
    bool getInfo(const QString &filePath, std::shared_ptr<AudioFile> audioFile);
    
    /**
     * @brief Verifica se um formato é suportado
     * @param filePath Caminho do arquivo
     * @return true se suportado
     */
    static bool isFormatSupported(const QString &filePath);
    
    /**
     * @brief Obtém lista de formatos suportados
     * @return Lista de extensões suportadas
     */
    static QStringList getSupportedFormats();
    
    /**
     * @brief Obtém filtro para QFileDialog
     * @return String de filtro para seleção de arquivos
     */
    static QString getFileDialogFilter();
    
    QString getLastError() const { return m_lastError; }

signals:
    void decodingStarted(const QString &filePath);
    void decodingProgress(int percentage);
    void decodingFinished(bool success);
    void error(const QString &message);

private:
    QString m_lastError;
};

#endif // AUDIODECODER_H
