#ifndef AUDIODECODERWORKER_H
#define AUDIODECODERWORKER_H

#include <QObject>
#include <QThread>
#include <QString>
#include <memory>

class AudioFile;

/**
 * @brief Worker thread para decodificação de áudio sem travar a UI
 */
class AudioDecoderWorker : public QObject
{
    Q_OBJECT

public:
    explicit AudioDecoderWorker(QObject *parent = nullptr);
    ~AudioDecoderWorker();

public slots:
    /**
     * @brief Decodifica um arquivo de áudio em thread separada
     * @param filePath Caminho do arquivo
     * @param audioFile Ponteiro para objeto AudioFile
     */
    void decodeFile(const QString &filePath, std::shared_ptr<AudioFile> audioFile);

signals:
    /**
     * @brief Sinal emitido quando a decodificação é concluída
     * @param success true se sucesso, false se erro
     * @param errorMessage Mensagem de erro (vazia se sucesso)
     */
    void decodingFinished(bool success, const QString &errorMessage);
    
    /**
     * @brief Sinal emitido para atualizar progresso
     * @param progress Progresso de 0 a 100
     */
    void progressUpdated(int progress);
};

#endif // AUDIODECODERWORKER_H
