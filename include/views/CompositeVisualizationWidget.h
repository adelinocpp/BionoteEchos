#ifndef COMPOSITEVISUALIZATIONWIDGET_H
#define COMPOSITEVISUALIZATIONWIDGET_H

#include <QWidget>
#include <QSplitter>
#include <memory>

class AudioFile;
class WaveformWidget;
class SpectrogramWidget;
class AnnotationLayerWidget;
class Project;

/**
 * @brief Widget composto com visualização de áudio, espectrograma e anotações
 * 
 * Layout vertical:
 * - Topo: Forma de onda (waveform)
 * - Meio: Espectrograma
 * - Baixo: Camadas de anotação (até 2 visíveis)
 */
class CompositeVisualizationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CompositeVisualizationWidget(QWidget *parent = nullptr);
    ~CompositeVisualizationWidget();
    
    /**
     * @brief Define o arquivo de áudio a ser visualizado
     */
    void setAudioFile(std::shared_ptr<AudioFile> audioFile);
    
    /**
     * @brief Define o projeto (para acessar camadas)
     */
    void setProject(std::shared_ptr<Project> project);
    
    /**
     * @brief Define se o espectrograma deve ser exibido
     */
    void setShowSpectrogram(bool show);
    
    /**
     * @brief Define a posição de reprodução
     */
    void setPlaybackPosition(double timeSeconds);
    
    /**
     * @brief Define se está reproduzindo
     */
    void setPlaying(bool playing);
    
    /**
     * @brief Obtém a seleção de tempo atual
     */
    bool getTimeSelection(double &startTime, double &endTime) const;
    
    /**
     * @brief Limpa a seleção
     */
    void clearTimeSelection();
    
    /**
     * @brief Zoom fit
     */
    void zoomFit();

signals:
    void timeSelectionChanged(double startTime, double endTime);
    void timeSelectionCleared();
    void timeClicked(double timeSeconds);

private slots:
    void onWaveformSelectionChanged(double startTime, double endTime);
    void onWaveformTimeClicked(double timeSeconds);

private:
    void setupUI();
    void syncTimeRange(double startTime, double endTime);

private:
    std::shared_ptr<AudioFile> m_audioFile;
    std::shared_ptr<Project> m_project;
    
    QSplitter *m_mainSplitter;
    WaveformWidget *m_waveformWidget;
    SpectrogramWidget *m_spectrogramWidget;
    AnnotationLayerWidget *m_annotationWidget;
    
    bool m_showSpectrogram;
};

#endif // COMPOSITEVISUALIZATIONWIDGET_H
