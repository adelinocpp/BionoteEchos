#include "views/MainWindow.h"
#include "views/AudioListWidget.h"
#include "views/AudioVisualizationWidget.h"
#include "views/SpectrogramWidget.h"
#include "views/SpectrogramSettingsDialog.h"
#include "views/AnnotationLayerWidget.h"
#include "views/AudioControlWidget.h"
#include "views/AboutDialog.h"
#include "controllers/ProjectController.h"
#include "controllers/AudioController.h"
#include "controllers/AnnotationController.h"
#include "models/Project.h"
#include "models/AudioFile.h"
#include "audio/AudioDecoder.h"
#include "audio/AudioDecoderWorker.h"
#include "audio/CustomAudioPlayer.h"

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QStatusBar>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>
#include <QSettings>
#include <QApplication>
#include <QThread>
#include <QProgressDialog>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_mainSplitter(nullptr)
    , m_centralSplitter(nullptr)
    , m_audioListWidget(nullptr)
    , m_audioVisualizationWidget(nullptr)
    , m_spectrogramWidget(nullptr)
    , m_annotationLayerWidget(nullptr)
    , m_audioControlWidget(nullptr)
    , m_audioPlayer(nullptr)
{
    // Create project and controllers
    m_project = std::make_shared<Project>();
    m_projectController = std::make_shared<ProjectController>(m_project);
    m_audioController = std::make_shared<AudioController>(m_project);
    m_annotationController = std::make_shared<AnnotationController>(m_project);
    
    // Create audio player
    m_audioPlayer = new CustomAudioPlayer(this);
    
    // Setup UI
    createActions();
    createMenus();
    createToolBars();
    setupLayout();
    createStatusBar();
    connectSignals();
    
    // Load settings
    loadSettings();
    
    // Set window properties
    setWindowTitle("BionoteEchos");
    setWindowIcon(QIcon(":/icons/Bionote_logo_black.png"));
    resize(1200, 800);
    
    updateStatusBar("Pronto");
}

MainWindow::~MainWindow()
{
    saveSettings();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        saveSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::setupLayout()
{
    // Create central widget
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // Create main horizontal splitter (left panel | central panel)
    m_mainSplitter = new QSplitter(Qt::Horizontal, this);
    
    // Create left panel (audio list)
    m_audioListWidget = new AudioListWidget(this);
    m_audioListWidget->setProject(m_project);
    m_audioListWidget->setMinimumWidth(180);
    m_audioListWidget->setMaximumWidth(400);
    
    // Create central panel container
    QWidget *centralPanel = new QWidget(this);
    QVBoxLayout *centralLayout = new QVBoxLayout(centralPanel);
    centralLayout->setContentsMargins(0, 0, 0, 0);
    centralLayout->setSpacing(0);
    
    // Create central vertical splitter (waveform | spectrogram | annotation)
    m_centralSplitter = new QSplitter(Qt::Vertical, this);
    
    // Create visualization widget (waveform)
    m_audioVisualizationWidget = new AudioVisualizationWidget(this);
    m_audioVisualizationWidget->setMinimumHeight(150);
    
    // Create spectrogram widget
    m_spectrogramWidget = new SpectrogramWidget(this);
    m_spectrogramWidget->setMinimumHeight(150);
    
    // Create annotation layer widget
    m_annotationLayerWidget = new AnnotationLayerWidget(this);
    m_annotationLayerWidget->setMinimumHeight(100);
    
    m_centralSplitter->addWidget(m_audioVisualizationWidget);
    m_centralSplitter->addWidget(m_spectrogramWidget);
    m_centralSplitter->addWidget(m_annotationLayerWidget);
    m_centralSplitter->setStretchFactor(0, 2);  // Waveform
    m_centralSplitter->setStretchFactor(1, 2);  // Spectrogram
    m_centralSplitter->setStretchFactor(1, 1);  // Annotations
    
    // Add splitter to central panel
    centralLayout->addWidget(m_centralSplitter);
    
    // Create audio control widget dentro do painel central
    m_audioControlWidget = new AudioControlWidget(this);
    centralLayout->addWidget(m_audioControlWidget);
    
    // Add to main splitter
    m_mainSplitter->addWidget(m_audioListWidget);
    m_mainSplitter->addWidget(centralPanel);
    
    // Define proporções: lista 15%, central 85%
    m_mainSplitter->setStretchFactor(0, 1);
    m_mainSplitter->setStretchFactor(1, 6);
    
    // Definir tamanhos iniciais em pixels
    QList<int> sizes;
    sizes << 220 << 1060;  // Lista: 220px, Central: resto
    m_mainSplitter->setSizes(sizes);
    
    // Add main splitter to layout
    mainLayout->addWidget(m_mainSplitter);
    
    setCentralWidget(centralWidget);
}

void MainWindow::createActions()
{
    // File menu actions
    m_newProjectAction = new QAction("&Novo Projeto", this);
    m_newProjectAction->setShortcut(QKeySequence::New);
    m_newProjectAction->setStatusTip("Criar um novo projeto");
    connect(m_newProjectAction, &QAction::triggered, this, &MainWindow::onNewProject);
    
    m_openProjectAction = new QAction("&Abrir Projeto...", this);
    m_openProjectAction->setShortcut(QKeySequence::Open);
    m_openProjectAction->setStatusTip("Abrir um projeto existente");
    connect(m_openProjectAction, &QAction::triggered, this, &MainWindow::onOpenProject);
    
    m_saveProjectAction = new QAction("&Salvar Projeto", this);
    m_saveProjectAction->setShortcut(QKeySequence::Save);
    m_saveProjectAction->setStatusTip("Salvar o projeto atual");
    connect(m_saveProjectAction, &QAction::triggered, this, &MainWindow::onSaveProject);
    
    m_saveProjectAsAction = new QAction("Salvar Projeto &Como...", this);
    m_saveProjectAsAction->setShortcut(QKeySequence::SaveAs);
    m_saveProjectAsAction->setStatusTip("Salvar o projeto com um novo nome");
    connect(m_saveProjectAsAction, &QAction::triggered, this, &MainWindow::onSaveProjectAs);
    
    m_openAudioFilesAction = new QAction("Abrir Arquivos de Á&udio...", this);
    m_openAudioFilesAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));
    m_openAudioFilesAction->setStatusTip("Abrir um ou mais arquivos de áudio");
    connect(m_openAudioFilesAction, &QAction::triggered, this, &MainWindow::onOpenAudioFiles);
    
    m_closeProjectAction = new QAction("&Fechar Projeto", this);
    m_closeProjectAction->setShortcut(QKeySequence::Close);
    m_closeProjectAction->setStatusTip("Fechar o projeto atual");
    connect(m_closeProjectAction, &QAction::triggered, this, &MainWindow::onCloseProject);
    
    m_exportTextGridAction = new QAction("&Exportar TextGrid...", this);
    m_exportTextGridAction->setStatusTip("Exportar anotações para formato TextGrid do Praat");
    connect(m_exportTextGridAction, &QAction::triggered, this, &MainWindow::onExportTextGrid);
    
    m_importTextGridAction = new QAction("&Importar TextGrid...", this);
    m_importTextGridAction->setStatusTip("Importar anotações do formato TextGrid do Praat");
    connect(m_importTextGridAction, &QAction::triggered, this, &MainWindow::onImportTextGrid);
    
    m_exitAction = new QAction("Sai&r", this);
    m_exitAction->setShortcut(QKeySequence::Quit);
    m_exitAction->setStatusTip("Sair da aplicação");
    connect(m_exitAction, &QAction::triggered, this, &MainWindow::onExit);
    
    // View menu actions
    m_showSpectrogramAction = new QAction("Mostrar &Espectrograma", this);
    m_showSpectrogramAction->setCheckable(true);
    m_showSpectrogramAction->setChecked(false);
    m_showSpectrogramAction->setStatusTip("Mostrar ou ocultar espectrograma");
    connect(m_showSpectrogramAction, &QAction::triggered, this, &MainWindow::onShowSpectrogram);
    
    m_spectrogramSettingsAction = new QAction("&Configurações do Espectrograma...", this);
    m_spectrogramSettingsAction->setStatusTip("Configurar parâmetros do espectrograma");
    connect(m_spectrogramSettingsAction, &QAction::triggered, this, &MainWindow::onSpectrogramSettings);
    
    m_zoomInAction = new QAction("Ampliar (&+)", this);
    m_zoomInAction->setShortcut(QKeySequence::ZoomIn);
    m_zoomInAction->setStatusTip("Ampliar visualização da forma de onda");
    connect(m_zoomInAction, &QAction::triggered, this, &MainWindow::onZoomIn);
    
    m_zoomOutAction = new QAction("Reduzir (&-)", this);
    m_zoomOutAction->setShortcut(QKeySequence::ZoomOut);
    m_zoomOutAction->setStatusTip("Reduzir visualização da forma de onda");
    connect(m_zoomOutAction, &QAction::triggered, this, &MainWindow::onZoomOut);
    
    m_zoomFitAction = new QAction("Ajustar à &Janela", this);
    m_zoomFitAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_0));
    m_zoomFitAction->setStatusTip("Ajustar toda a forma de onda na visualização");
    connect(m_zoomFitAction, &QAction::triggered, this, &MainWindow::onZoomFit);
    
    // Annotation menu actions
    m_addIntervalTierAction = new QAction("Adicionar Camada de &Intervalos...", this);
    m_addIntervalTierAction->setStatusTip("Adicionar uma nova camada de intervalos");
    connect(m_addIntervalTierAction, &QAction::triggered, this, &MainWindow::onAddIntervalTier);
    
    m_addPointTierAction = new QAction("Adicionar Camada de &Pontos...", this);
    m_addPointTierAction->setStatusTip("Adicionar uma nova camada de pontos");
    connect(m_addPointTierAction, &QAction::triggered, this, &MainWindow::onAddPointTier);
    
    m_removeTierAction = new QAction("&Remover Camada", this);
    m_removeTierAction->setStatusTip("Remover a camada selecionada");
    connect(m_removeTierAction, &QAction::triggered, this, &MainWindow::onRemoveTier);
    
    m_renameTierAction = new QAction("Re&nomear Camada...", this);
    m_renameTierAction->setStatusTip("Renomear a camada selecionada");
    connect(m_renameTierAction, &QAction::triggered, this, &MainWindow::onRenameTier);
    
    // Analysis menu actions
    m_calculatePitchAction = new QAction("Calcular &Pitch (F0)...", this);
    m_calculatePitchAction->setStatusTip("Calcular frequência fundamental usando YAAPT");
    connect(m_calculatePitchAction, &QAction::triggered, this, &MainWindow::onCalculatePitch);
    
    m_calculateIntensityAction = new QAction("Calcular &Intensidade...", this);
    m_calculateIntensityAction->setStatusTip("Calcular intensidade (dB)");
    connect(m_calculateIntensityAction, &QAction::triggered, this, &MainWindow::onCalculateIntensity);
    
    m_pitchSettingsAction = new QAction("&Configurações de Pitch...", this);
    m_pitchSettingsAction->setStatusTip("Configurar parâmetros de detecção de pitch");
    connect(m_pitchSettingsAction, &QAction::triggered, this, &MainWindow::onPitchSettings);
    
    // Help menu actions
    m_aboutAction = new QAction("&Sobre AudioAnnotator", this);
    m_aboutAction->setStatusTip("Mostrar informações sobre AudioAnnotator");
    connect(m_aboutAction, &QAction::triggered, this, &MainWindow::onAbout);
    
    m_helpAction = new QAction("&Ajuda", this);
    m_helpAction->setShortcut(QKeySequence::HelpContents);
    m_helpAction->setStatusTip("Mostrar documentação de ajuda");
    connect(m_helpAction, &QAction::triggered, this, &MainWindow::onHelp);
}

void MainWindow::createMenus()
{
    // File menu
    m_fileMenu = menuBar()->addMenu("&Arquivo");
    m_fileMenu->addAction(m_newProjectAction);
    m_fileMenu->addAction(m_openProjectAction);
    m_fileMenu->addAction(m_saveProjectAction);
    m_fileMenu->addAction(m_saveProjectAsAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_openAudioFilesAction);
    m_fileMenu->addAction(m_closeProjectAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exportTextGridAction);
    m_fileMenu->addAction(m_importTextGridAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAction);
    
    // View menu
    m_viewMenu = menuBar()->addMenu("&Visualizar");
    m_viewMenu->addAction(m_showSpectrogramAction);
    m_viewMenu->addAction(m_spectrogramSettingsAction);
    m_viewMenu->addSeparator();
    m_viewMenu->addAction(m_zoomInAction);
    m_viewMenu->addAction(m_zoomOutAction);
    m_viewMenu->addAction(m_zoomFitAction);
    
    // Annotation menu
    m_annotationMenu = menuBar()->addMenu("&Anotação");
    m_annotationMenu->addAction(m_addIntervalTierAction);
    m_annotationMenu->addAction(m_addPointTierAction);
    m_annotationMenu->addSeparator();
    m_annotationMenu->addAction(m_removeTierAction);
    m_annotationMenu->addAction(m_renameTierAction);
    
    // Analysis menu
    m_analysisMenu = menuBar()->addMenu("&Análise");
    m_analysisMenu->addAction(m_calculatePitchAction);
    m_analysisMenu->addAction(m_calculateIntensityAction);
    m_analysisMenu->addSeparator();
    m_analysisMenu->addAction(m_pitchSettingsAction);
    
    // Help menu
    m_helpMenu = menuBar()->addMenu("Aj&uda");
    m_helpMenu->addAction(m_helpAction);
    m_helpMenu->addSeparator();
    m_helpMenu->addAction(m_aboutAction);
}

void MainWindow::createToolBars()
{
    // File toolbar
    m_fileToolBar = addToolBar("Arquivo");
    m_fileToolBar->setObjectName("FileToolBar");
    m_fileToolBar->addAction(m_newProjectAction);
    m_fileToolBar->addAction(m_openProjectAction);
    m_fileToolBar->addAction(m_saveProjectAction);
    m_fileToolBar->addSeparator();
    m_fileToolBar->addAction(m_openAudioFilesAction);
    
    // View toolbar
    m_viewToolBar = addToolBar("Visualizar");
    m_viewToolBar->setObjectName("ViewToolBar");
    m_viewToolBar->addAction(m_showSpectrogramAction);
    m_viewToolBar->addSeparator();
    m_viewToolBar->addAction(m_zoomInAction);
    m_viewToolBar->addAction(m_zoomOutAction);
    m_viewToolBar->addAction(m_zoomFitAction);
    
    // Annotation toolbar
    m_annotationToolBar = addToolBar("Anotação");
    m_annotationToolBar->setObjectName("AnnotationToolBar");
    m_annotationToolBar->addAction(m_addIntervalTierAction);
    m_annotationToolBar->addAction(m_addPointTierAction);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage("Pronto");
}

void MainWindow::connectSignals()
{
    // Connect project signals
    connect(m_project.get(), &Project::projectModified, this, &MainWindow::updateWindowTitle);
    connect(m_project.get(), &Project::projectPathChanged, this, &MainWindow::updateWindowTitle);
    
    // Connect project to audio list widget
    connect(m_project.get(), &Project::audioFileAdded,
            m_audioListWidget, &AudioListWidget::onAudioFileAdded);
    connect(m_project.get(), &Project::audioFileRemoved,
            m_audioListWidget, &AudioListWidget::onAudioFileRemoved);
    
    // Connect audio list signals
    connect(m_audioListWidget, &AudioListWidget::audioFileSelected,
            [this](std::shared_ptr<AudioFile> audioFile) {
                // Limpar estado anterior
                m_audioControlWidget->setPosition(0.0);
                m_audioControlWidget->setPlaying(false);
                
                // Configurar novo arquivo
                m_audioVisualizationWidget->setAudioFile(audioFile);
                m_spectrogramWidget->setAudioFile(audioFile);
                m_audioPlayer->setAudioFile(audioFile);
                
                if (audioFile) {
                    m_audioControlWidget->setDuration(audioFile->getDuration());
                } else {
                    m_audioControlWidget->setDuration(0.0);
                }
            });
    
    // Connect visualization signals
    connect(m_audioVisualizationWidget, &AudioVisualizationWidget::visibleTimeRangeChanged,
            m_annotationLayerWidget, &AnnotationLayerWidget::setVisibleTimeRange);
    
    // Sincronizar zoom/pan entre waveform e espectrograma
    connect(m_audioVisualizationWidget, &AudioVisualizationWidget::visibleTimeRangeChanged,
            [this](double startTime, double endTime) {
                m_spectrogramWidget->setVisibleTimeRange(startTime, endTime - startTime);
            });
    
    connect(m_spectrogramWidget, &SpectrogramWidget::visibleTimeRangeChanged,
            [this](double startTime, double duration) {
                m_audioVisualizationWidget->setVisibleTimeRange(startTime, duration);
                m_annotationLayerWidget->setVisibleTimeRange(startTime, startTime + duration);
            });
    
    // Connect selection to player region
    connect(m_audioVisualizationWidget, &AudioVisualizationWidget::timeSelectionChanged,
            [this](double startTime, double endTime) {
                m_audioPlayer->setPlaybackRegion((qint64)(startTime * 1000), (qint64)(endTime * 1000));
            });
    connect(m_audioVisualizationWidget, &AudioVisualizationWidget::timeSelectionCleared,
            [this]() {
                m_audioPlayer->clearPlaybackRegion();
            });
    
    // Connect click to position player (Ctrl+Click)
    connect(m_audioVisualizationWidget, &AudioVisualizationWidget::timeClicked,
            [this](double timeSeconds) {
                m_audioPlayer->setPosition((qint64)(timeSeconds * 1000));
            });
    
    // Connect audio control signals to player
    connect(m_audioControlWidget, &AudioControlWidget::playClicked,
            m_audioPlayer, &CustomAudioPlayer::play);
    connect(m_audioControlWidget, &AudioControlWidget::pauseClicked,
            m_audioPlayer, &CustomAudioPlayer::pause);
    connect(m_audioControlWidget, &AudioControlWidget::stopClicked,
            m_audioPlayer, &CustomAudioPlayer::stop);
    connect(m_audioControlWidget, &AudioControlWidget::volumeChanged,
            [this](int volume) {
                m_audioPlayer->setVolume(volume / 100.0f);
            });
    connect(m_audioControlWidget, &AudioControlWidget::loopModeChanged,
            m_audioPlayer, &CustomAudioPlayer::setLoop);
    connect(m_audioControlWidget, &AudioControlWidget::positionChanged,
            [this](double seconds) {
                m_audioPlayer->setPosition((qint64)(seconds * 1000));
            });
    
    // Connect player signals to control widget
    connect(m_audioPlayer, &CustomAudioPlayer::positionChanged,
            [this](qint64 positionMs) {
                double positionSec = positionMs / 1000.0;
                m_audioControlWidget->setPosition(positionSec);
                m_audioVisualizationWidget->setPlaybackPosition(positionSec);
                m_spectrogramWidget->setPlaybackPosition(positionSec);
            });
    connect(m_audioPlayer, &CustomAudioPlayer::playbackStateChanged,
            [this](int state) {
                bool isPlaying = (state == 1); // 0=Stopped, 1=Playing, 2=Paused
                m_audioControlWidget->setPlaying(isPlaying);
                m_audioVisualizationWidget->setPlaying(isPlaying);
            });
    
    // Set initial volume
    m_audioPlayer->setVolume(m_audioControlWidget->getVolume() / 100.0f);
}

// Project management implementations
void MainWindow::onNewProject() {
    if (!maybeSave()) {
        return;
    }
    
    m_project->newProject();
    updateWindowTitle();
    updateStatusBar("Novo projeto criado");
}

void MainWindow::onOpenProject() {
    if (!maybeSave()) {
        return;
    }
    
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Abrir Projeto",
        QString(),
        "Projetos BionoteEchos (*.becho);;Todos os Arquivos (*)"
    );
    
    if (fileName.isEmpty()) {
        return;
    }
    
    if (m_project->loadProject(fileName)) {
        updateWindowTitle();
        updateStatusBar("Projeto carregado: " + fileName);
        
        // TODO: Recarregar arquivos de áudio do projeto
        // Por enquanto, apenas atualizar a lista
        m_audioListWidget->updateList();
    } else {
        QMessageBox::critical(this, "Erro", "Não foi possível carregar o projeto.");
    }
}

void MainWindow::onSaveProject() {
    if (m_project->getProjectPath().isEmpty()) {
        onSaveProjectAs();
        return;
    }
    
    if (m_project->saveProject()) {
        updateWindowTitle();
        updateStatusBar("Projeto salvo");
    } else {
        QMessageBox::critical(this, "Erro", "Não foi possível salvar o projeto.");
    }
}

void MainWindow::onSaveProjectAs() {
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Salvar Projeto Como",
        QString(),
        "Projetos BionoteEchos (*.becho);;Todos os Arquivos (*)"
    );
    
    if (fileName.isEmpty()) {
        return;
    }
    
    // Adicionar extensão se não tiver
    if (!fileName.endsWith(".becho", Qt::CaseInsensitive)) {
        fileName += ".becho";
    }
    
    if (m_project->saveProject(fileName)) {
        updateWindowTitle();
        updateStatusBar("Projeto salvo como: " + fileName);
    } else {
        QMessageBox::critical(this, "Erro", "Não foi possível salvar o projeto.");
    }
}
void MainWindow::onOpenAudioFiles() {
    // Abrir diálogo para seleção de arquivos
    QStringList fileNames = QFileDialog::getOpenFileNames(
        this,
        tr("Abrir Arquivos de Áudio"),
        QString(),
        AudioDecoder::getFileDialogFilter()
    );
    
    if (fileNames.isEmpty()) {
        return;
    }
    
    // Processar arquivos em thread separada
    for (const QString &fileName : fileNames) {
        // Verificar se o arquivo já está no projeto
        if (m_project->findAudioFile(fileName) >= 0) {
            QMessageBox::information(this, tr("AudioAnnotator"),
                tr("O arquivo já está aberto no projeto:\n%1").arg(fileName));
            continue;
        }
        
        // Criar novo objeto AudioFile
        auto audioFile = std::make_shared<AudioFile>(fileName);
        
        // Criar thread e worker
        QThread *thread = new QThread(this);
        AudioDecoderWorker *worker = new AudioDecoderWorker();
        worker->moveToThread(thread);
        
        // Criar diálogo de progresso
        QProgressDialog *progress = new QProgressDialog(
            tr("Carregando %1...").arg(QFileInfo(fileName).fileName()),
            tr("Cancelar"), 0, 100, this);
        progress->setWindowModality(Qt::WindowModal);
        progress->setMinimumDuration(500);
        
        // Conectar sinais
        connect(thread, &QThread::started, [worker, fileName, audioFile]() {
            worker->decodeFile(fileName, audioFile);
        });
        
        connect(worker, &AudioDecoderWorker::progressUpdated, progress, &QProgressDialog::setValue);
        
        connect(worker, &AudioDecoderWorker::decodingFinished,
                this, [this, audioFile, fileName, thread, worker, progress](bool success, const QString &error) {
            progress->close();
            progress->deleteLater();
            
            if (success) {
                m_project->addAudioFile(audioFile);
                updateStatusBar(tr("Arquivo carregado: %1").arg(audioFile->getFileName()));
            } else {
                QMessageBox::warning(this, tr("Erro ao Carregar Arquivo"),
                    tr("Não foi possível carregar o arquivo:\n%1\n\nErro: %2")
                    .arg(fileName).arg(error));
            }
            
            thread->quit();
        });
        
        connect(thread, &QThread::finished, worker, &QObject::deleteLater);
        connect(thread, &QThread::finished, thread, &QObject::deleteLater);
        
        thread->start();
    }
}
void MainWindow::onCloseProject() { /* TODO */ }
void MainWindow::onExportTextGrid() { /* TODO */ }
void MainWindow::onImportTextGrid() { /* TODO */ }
void MainWindow::onExit() { close(); }

void MainWindow::onUndo() { /* TODO */ }
void MainWindow::onRedo() { /* TODO */ }
void MainWindow::onCut() { /* TODO */ }
void MainWindow::onCopy() { /* TODO */ }
void MainWindow::onPaste() { /* TODO */ }
void MainWindow::onDelete() { /* TODO */ }
void MainWindow::onPreferences() { /* TODO */ }

void MainWindow::onShowSpectrogram(bool show) {
    m_audioVisualizationWidget->setShowSpectrogram(show);
}

void MainWindow::onSpectrogramSettings()
{
    if (!m_spectrogramWidget) {
        return;
    }
    
    // Obter configurações atuais
    SpectrogramWidget::Settings currentSettings = m_spectrogramWidget->getSettings();
    
    // Converter para formato do diálogo
    SpectrogramSettingsDialog::Settings dialogSettings;
    dialogSettings.timeStep = currentSettings.timeStep;
    dialogSettings.timeWindow = currentSettings.timeWindow;
    dialogSettings.fftSize = currentSettings.fftSize;
    dialogSettings.windowType = currentSettings.windowType;
    dialogSettings.minFrequency = currentSettings.minFrequency;
    dialogSettings.maxFrequency = currentSettings.maxFrequency;
    dialogSettings.dynamicRange = currentSettings.dynamicRange;
    dialogSettings.colorMap = currentSettings.colorMap;
    dialogSettings.preEmphasis = currentSettings.preEmphasis;
    dialogSettings.preEmphasisFactor = currentSettings.preEmphasisFactor;
    
    // Mostrar diálogo
    SpectrogramSettingsDialog dialog(dialogSettings, this);
    if (dialog.exec() == QDialog::Accepted) {
        // Aplicar novas configurações
        SpectrogramSettingsDialog::Settings newSettings = dialog.getSettings();
        
        SpectrogramWidget::Settings widgetSettings;
        widgetSettings.timeStep = newSettings.timeStep;
        widgetSettings.timeWindow = newSettings.timeWindow;
        widgetSettings.fftSize = newSettings.fftSize;
        widgetSettings.windowType = newSettings.windowType;
        widgetSettings.minFrequency = newSettings.minFrequency;
        widgetSettings.maxFrequency = newSettings.maxFrequency;
        widgetSettings.dynamicRange = newSettings.dynamicRange;
        widgetSettings.colorMap = newSettings.colorMap;
        widgetSettings.preEmphasis = newSettings.preEmphasis;
        widgetSettings.preEmphasisFactor = newSettings.preEmphasisFactor;
        
        m_spectrogramWidget->setSettings(widgetSettings);
        
        // Recalcular espectrograma se houver áudio carregado
        if (m_spectrogramWidget->isCalculating() == false) {
            m_spectrogramWidget->calculateSpectrogram();
        }
    }
}

void MainWindow::onZoomIn() {
    m_audioVisualizationWidget->zoom(1.5);
}

void MainWindow::onZoomOut() {
    m_audioVisualizationWidget->zoom(0.67);
}

void MainWindow::onZoomFit() {
    m_audioVisualizationWidget->zoomFit();
}

void MainWindow::onAddIntervalTier() { /* TODO */ }
void MainWindow::onAddPointTier() { /* TODO */ }
void MainWindow::onRemoveTier() { /* TODO */ }
void MainWindow::onRenameTier() { /* TODO */ }

void MainWindow::onCalculatePitch() { /* TODO */ }
void MainWindow::onCalculateIntensity() { /* TODO */ }
void MainWindow::onPitchSettings() { /* TODO */ }

void MainWindow::onAbout() {
    AboutDialog dialog(this);
    dialog.exec();
}

void MainWindow::onHelp() { /* TODO */ }

void MainWindow::updateWindowTitle() {
    QString title = "BionoteEchos";
    if (!m_project->getProjectPath().isEmpty()) {
        title += " - " + m_project->getProjectName();
    }
    if (m_project->isModified()) {
        title += " *";
    }
    setWindowTitle(title);
}

void MainWindow::updateStatusBar(const QString &message) {
    statusBar()->showMessage(message);
}

bool MainWindow::maybeSave() {
    if (!m_project->isModified()) {
        return true;
    }
    
    QMessageBox::StandardButton ret = QMessageBox::warning(this, tr("AudioAnnotator"),
        tr("The project has been modified.\nDo you want to save your changes?"),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    
    if (ret == QMessageBox::Save) {
        onSaveProject();
        return true;
    } else if (ret == QMessageBox::Cancel) {
        return false;
    }
    return true;
}

void MainWindow::loadSettings() {
    QSettings settings("AudioAnnotator", "AudioAnnotator");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
    m_mainSplitter->restoreState(settings.value("mainSplitter").toByteArray());
    m_centralSplitter->restoreState(settings.value("centralSplitter").toByteArray());
}

void MainWindow::saveSettings() {
    QSettings settings("AudioAnnotator", "AudioAnnotator");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    settings.setValue("mainSplitter", m_mainSplitter->saveState());
    settings.setValue("centralSplitter", m_centralSplitter->saveState());
}

