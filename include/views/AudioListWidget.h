#ifndef AUDIOLISTWIDGET_H
#define AUDIOLISTWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <memory>

class Project;
class AudioFile;

/**
 * @brief Widget para exibir lista de arquivos de áudio e objetos associados
 * 
 * Estrutura em árvore:
 * - Arquivo de áudio 1
 *   - Pitch data
 *   - Intensity data
 * - Arquivo de áudio 2
 *   - Pitch data
 * - ...
 */
class AudioListWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Construtor
     * @param parent Widget pai
     */
    explicit AudioListWidget(QWidget *parent = nullptr);

    /**
     * @brief Destrutor
     */
    ~AudioListWidget();
    
    /**
     * @brief Define o projeto a ser exibido
     * @param project Ponteiro para o projeto
     */
    void setProject(std::shared_ptr<Project> project);
    
    /**
     * @brief Atualiza a lista de arquivos
     */
    void updateList();
    
    /**
     * @brief Obtém o arquivo de áudio selecionado
     * @return Ponteiro para o arquivo selecionado (nullptr se nenhum)
     */
    std::shared_ptr<AudioFile> getSelectedAudioFile() const;
    
    /**
     * @brief Obtém o índice do arquivo de áudio selecionado
     * @return Índice do arquivo (-1 se nenhum)
     */
    int getSelectedAudioFileIndex() const;

signals:
    /**
     * @brief Sinal emitido quando um arquivo de áudio é selecionado
     * @param audioFile Ponteiro para o arquivo selecionado
     */
    void audioFileSelected(std::shared_ptr<AudioFile> audioFile);
    
    /**
     * @brief Sinal emitido quando um objeto associado é selecionado
     * @param audioFile Ponteiro para o arquivo de áudio pai
     * @param objectType Tipo do objeto ("pitch" ou "intensity")
     */
    void associatedObjectSelected(std::shared_ptr<AudioFile> audioFile, const QString &objectType);
    
    /**
     * @brief Sinal emitido quando a seleção é limpa
     */
    void selectionCleared();

public slots:
    void onAudioFileAdded(int index);
    void onAudioFileRemoved(int index);

private slots:
    void onItemSelectionChanged();
    void onItemDoubleClicked(QTreeWidgetItem *item, int column);
    void onCustomContextMenuRequested(const QPoint &pos);

private:
    void setupUI();
    void createContextMenu();
    void addAudioFileItem(std::shared_ptr<AudioFile> audioFile);
    void updateAudioFileItem(QTreeWidgetItem *item, std::shared_ptr<AudioFile> audioFile);
    
    QTreeWidget *m_treeWidget;
    std::shared_ptr<Project> m_project;
    
    QMenu *m_contextMenu;
    QAction *m_removeAudioFileAction;
    QAction *m_showMetadataAction;
    QAction *m_calculatePitchAction;
    QAction *m_calculateIntensityAction;
    QAction *m_removePitchDataAction;
    QAction *m_removeIntensityDataAction;
};

#endif // AUDIOLISTWIDGET_H

