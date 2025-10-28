#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <memory>

class AudioListWidget;
class AudioVisualizationWidget;
class AnnotationLayerWidget;
class AudioControlWidget;
class ProjectController;
class AudioController;
class AnnotationController;
class Project;

QT_BEGIN_NAMESPACE
class QMenuBar;
class QMenu;
class QAction;
class QToolBar;
class QStatusBar;
QT_END_NAMESPACE

/**
 * @brief Janela principal da aplicação AudioAnnotator
 * 
 * Layout:
 * - Barra de menu superior
 * - Barra de ferramentas
 * - Painel esquerdo: Lista de arquivos de áudio e objetos
 * - Painel central: Visualização de áudio e anotações
 * - Painel inferior: Controles de reprodução
 * - Barra de status
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Construtor
     * @param parent Widget pai
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destrutor
     */
    ~MainWindow();

protected:
    /**
     * @brief Evento de fechamento da janela
     */
    void closeEvent(QCloseEvent *event) override;

private slots:
    // File menu slots
    void onNewProject();
    void onOpenProject();
    void onSaveProject();
    void onSaveProjectAs();
    void onOpenAudioFiles();
    void onCloseProject();
    void onExportTextGrid();
    void onImportTextGrid();
    void onExit();
    
    // Edit menu slots
    void onUndo();
    void onRedo();
    void onCut();
    void onCopy();
    void onPaste();
    void onDelete();
    void onPreferences();
    
    // View menu slots
    void onShowSpectrogram(bool show);
    void onSpectrogramSettings();
    void onZoomIn();
    void onZoomOut();
    void onZoomFit();
    
    // Annotation menu slots
    void onAddIntervalTier();
    void onAddPointTier();
    void onRemoveTier();
    void onRenameTier();
    
    // Analysis menu slots
    void onCalculatePitch();
    void onCalculateIntensity();
    void onPitchSettings();
    
    // Help menu slots
    void onAbout();
    void onHelp();
    
    // Status updates
    void updateWindowTitle();
    void updateStatusBar(const QString &message);

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createDockWidgets();
    void setupLayout();
    void connectSignals();
    
    bool maybeSave();
    void loadSettings();
    void saveSettings();

private:
    // Central widgets
    QSplitter *m_mainSplitter;
    QSplitter *m_centralSplitter;
    AudioListWidget *m_audioListWidget;
    AudioVisualizationWidget *m_audioVisualizationWidget;
    class SpectrogramWidget *m_spectrogramWidget;
    AnnotationLayerWidget *m_annotationLayerWidget;
    AudioControlWidget *m_audioControlWidget;
    
    // Controllers
    std::shared_ptr<Project> m_project;
    std::shared_ptr<ProjectController> m_projectController;
    std::shared_ptr<AudioController> m_audioController;
    std::shared_ptr<AnnotationController> m_annotationController;
    
    // Audio player
    class CustomAudioPlayer *m_audioPlayer;
    
    // Menus
    QMenu *m_fileMenu;
    QMenu *m_editMenu;
    QMenu *m_viewMenu;
    QMenu *m_annotationMenu;
    QMenu *m_analysisMenu;
    QMenu *m_helpMenu;
    
    // File menu actions
    QAction *m_newProjectAction;
    QAction *m_openProjectAction;
    QAction *m_saveProjectAction;
    QAction *m_saveProjectAsAction;
    QAction *m_openAudioFilesAction;
    QAction *m_closeProjectAction;
    QAction *m_exportTextGridAction;
    QAction *m_importTextGridAction;
    QAction *m_exitAction;
    
    // Edit menu actions
    QAction *m_undoAction;
    QAction *m_redoAction;
    QAction *m_cutAction;
    QAction *m_copyAction;
    QAction *m_pasteAction;
    QAction *m_deleteAction;
    QAction *m_preferencesAction;
    
    // View menu actions
    QAction *m_showSpectrogramAction;
    QAction *m_spectrogramSettingsAction;
    QAction *m_zoomInAction;
    QAction *m_zoomOutAction;
    QAction *m_zoomFitAction;
    
    // Annotation menu actions
    QAction *m_addIntervalTierAction;
    QAction *m_addPointTierAction;
    QAction *m_removeTierAction;
    QAction *m_renameTierAction;
    
    // Analysis menu actions
    QAction *m_calculatePitchAction;
    QAction *m_calculateIntensityAction;
    QAction *m_pitchSettingsAction;
    
    // Help menu actions
    QAction *m_aboutAction;
    QAction *m_helpAction;
    
    // Toolbars
    QToolBar *m_fileToolBar;
    QToolBar *m_editToolBar;
    QToolBar *m_viewToolBar;
    QToolBar *m_annotationToolBar;
};

#endif // MAINWINDOW_H

