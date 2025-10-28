#ifndef PROJECT_H
#define PROJECT_H

#include <QString>
#include <QVector>
#include <QObject>
#include <memory>

class AudioFile;
class AnnotationTier;

/**
 * @brief Representa um projeto de anotação de áudio
 * 
 * Um projeto contém:
 * - Lista de arquivos de áudio
 * - Lista de camadas de anotação
 * - Configurações do projeto
 * - Caminho do arquivo de projeto
 */
class Project : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Construtor
     * @param parent Objeto pai Qt
     */
    explicit Project(QObject *parent = nullptr);

    /**
     * @brief Destrutor
     */
    ~Project();

    // Getters
    QString getProjectPath() const { return m_projectPath; }
    QString getProjectName() const;
    bool isModified() const { return m_modified; }
    
    int getAudioFileCount() const { return m_audioFiles.size(); }
    int getTierCount() const { return m_tiers.size(); }
    
    std::shared_ptr<AudioFile> getAudioFile(int index) const;
    std::shared_ptr<AnnotationTier> getTier(int index) const;
    
    const QVector<std::shared_ptr<AudioFile>>& getAudioFiles() const { return m_audioFiles; }
    const QVector<std::shared_ptr<AnnotationTier>>& getTiers() const { return m_tiers; }
    
    // Setters
    void setProjectPath(const QString &path);
    void setModified(bool modified);
    
    /**
     * @brief Adiciona um arquivo de áudio ao projeto
     * @param audioFile Ponteiro para o arquivo de áudio
     * @return Índice do arquivo adicionado
     */
    int addAudioFile(std::shared_ptr<AudioFile> audioFile);
    
    /**
     * @brief Remove um arquivo de áudio do projeto
     * @param index Índice do arquivo
     * @return true se removido com sucesso
     */
    bool removeAudioFile(int index);
    
    /**
     * @brief Adiciona uma camada de anotação ao projeto
     * @param tier Ponteiro para a camada
     * @return Índice da camada adicionada
     */
    int addTier(std::shared_ptr<AnnotationTier> tier);
    
    /**
     * @brief Remove uma camada de anotação do projeto
     * @param index Índice da camada
     * @return true se removido com sucesso
     */
    bool removeTier(int index);
    
    /**
     * @brief Encontra um arquivo de áudio pelo caminho
     * @param filePath Caminho do arquivo
     * @return Índice do arquivo (-1 se não encontrado)
     */
    int findAudioFile(const QString &filePath) const;
    
    /**
     * @brief Encontra uma camada de anotação pelo nome
     * @param name Nome da camada
     * @return Índice da camada (-1 se não encontrado)
     */
    int findTier(const QString &name) const;
    
    /**
     * @brief Limpa todos os dados do projeto
     */
    void clear();
    
    /**
     * @brief Verifica se o projeto está vazio
     */
    bool isEmpty() const;
    
    /**
     * @brief Cria um novo projeto vazio
     */
    void newProject();
    
    /**
     * @brief Salva o projeto em um arquivo
     * @param filePath Caminho do arquivo (se vazio, usa m_projectPath)
     * @return true se salvo com sucesso
     */
    bool saveProject(const QString &filePath = QString());
    
    /**
     * @brief Carrega um projeto de um arquivo
     * @param filePath Caminho do arquivo
     * @return true se carregado com sucesso
     */
    bool loadProject(const QString &filePath);
    
    /**
     * @brief Exporta o projeto para formato JSON
     * @return String JSON do projeto
     */
    QString toJson() const;
    
    /**
     * @brief Importa o projeto de formato JSON
     * @param json String JSON
     * @return true se importado com sucesso
     */
    bool fromJson(const QString &json);

signals:
    /**
     * @brief Sinal emitido quando um arquivo de áudio é adicionado
     */
    void audioFileAdded(int index);
    
    /**
     * @brief Sinal emitido quando um arquivo de áudio é removido
     */
    void audioFileRemoved(int index);
    
    /**
     * @brief Sinal emitido quando uma camada é adicionada
     */
    void tierAdded(int index);
    
    /**
     * @brief Sinal emitido quando uma camada é removida
     */
    void tierRemoved(int index);
    
    /**
     * @brief Sinal emitido quando o projeto é modificado
     */
    void projectModified();
    
    /**
     * @brief Sinal emitido quando o projeto é limpo
     */
    void projectCleared();
    
    /**
     * @brief Sinal emitido quando o caminho do projeto muda
     */
    void projectPathChanged(const QString &newPath);

private:
    QString m_projectPath;
    bool m_modified;
    
    QVector<std::shared_ptr<AudioFile>> m_audioFiles;
    QVector<std::shared_ptr<AnnotationTier>> m_tiers;
};

#endif // PROJECT_H

