#include "views/AudioListWidget.h"
#include "models/Project.h"
#include "models/AudioFile.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>

AudioListWidget::AudioListWidget(QWidget *parent)
    : QWidget(parent)
    , m_treeWidget(nullptr)
{
    setupUI();
}

AudioListWidget::~AudioListWidget()
{
}

void AudioListWidget::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(2, 2, 2, 2);
    layout->setSpacing(0);
    
    // Título
    QLabel *titleLabel = new QLabel("<b>Arquivos de Áudio</b>", this);
    titleLabel->setStyleSheet("padding: 4px; background-color: #e0e0e0;");
    layout->addWidget(titleLabel);
    
    // Árvore de arquivos
    m_treeWidget = new QTreeWidget(this);
    m_treeWidget->setHeaderLabel("Nome");
    m_treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    m_treeWidget->setAlternatingRowColors(true);
    m_treeWidget->setAnimated(true);
    
    layout->addWidget(m_treeWidget);
    
    // Conectar sinais
    connect(m_treeWidget, &QTreeWidget::itemSelectionChanged,
            this, &AudioListWidget::onItemSelectionChanged);
    connect(m_treeWidget, &QTreeWidget::itemDoubleClicked,
            this, &AudioListWidget::onItemDoubleClicked);
    connect(m_treeWidget, &QTreeWidget::customContextMenuRequested,
            this, &AudioListWidget::onCustomContextMenuRequested);
    
    createContextMenu();
}

void AudioListWidget::setProject(std::shared_ptr<Project> project)
{
    m_project = project;
    updateList();
}

void AudioListWidget::updateList()
{
    m_treeWidget->clear();
    if (!m_project) return;
    
    for (int i = 0; i < m_project->getAudioFileCount(); ++i) {
        auto audioFile = m_project->getAudioFile(i);
        if (audioFile) {
            addAudioFileItem(audioFile);
        }
    }
}

void AudioListWidget::addAudioFileItem(std::shared_ptr<AudioFile> audioFile)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(m_treeWidget);
    item->setText(0, audioFile->getFileName());
    item->setData(0, Qt::UserRole, QVariant::fromValue(audioFile.get()));
    item->setData(0, Qt::UserRole + 1, "audio");
    
    // Ícone para arquivo de áudio (usando símbolo Unicode)
    item->setIcon(0, style()->standardIcon(QStyle::SP_MediaPlay));
    
    // Adicionar informações como tooltip
    QString tooltip = QString("Arquivo: %1\n"
                             "Duração: %2 s\n"
                             "Taxa: %3 Hz\n"
                             "Canais: %4\n"
                             "Amostras: %5")
                      .arg(audioFile->getFileName())
                      .arg(audioFile->getDuration(), 0, 'f', 2)
                      .arg(audioFile->getSampleRate())
                      .arg(audioFile->getNumChannels())
                      .arg(audioFile->getNumSamples());
    item->setToolTip(0, tooltip);
    
    // Adicionar sub-itens para objetos associados
    updateAudioFileItem(item, audioFile);
    
    // Expandir item por padrão
    item->setExpanded(true);
}

std::shared_ptr<AudioFile> AudioListWidget::getSelectedAudioFile() const
{
    QList<QTreeWidgetItem*> selectedItems = m_treeWidget->selectedItems();
    if (selectedItems.isEmpty()) {
        return nullptr;
    }
    
    QTreeWidgetItem *item = selectedItems.first();
    
    // Se for um sub-item, pegar o pai
    if (item->parent()) {
        item = item->parent();
    }
    
    AudioFile *rawPtr = item->data(0, Qt::UserRole).value<AudioFile*>();
    if (!rawPtr || !m_project) {
        return nullptr;
    }
    
    // Encontrar o shared_ptr correspondente
    for (int i = 0; i < m_project->getAudioFileCount(); ++i) {
        auto audioFile = m_project->getAudioFile(i);
        if (audioFile.get() == rawPtr) {
            return audioFile;
        }
    }
    
    return nullptr;
}

int AudioListWidget::getSelectedAudioFileIndex() const
{
    auto audioFile = getSelectedAudioFile();
    if (!audioFile || !m_project) {
        return -1;
    }
    
    for (int i = 0; i < m_project->getAudioFileCount(); ++i) {
        if (m_project->getAudioFile(i) == audioFile) {
            return i;
        }
    }
    
    return -1;
}

void AudioListWidget::onItemSelectionChanged()
{
    QList<QTreeWidgetItem*> selectedItems = m_treeWidget->selectedItems();
    
    if (selectedItems.isEmpty()) {
        emit selectionCleared();
        return;
    }
    
    QTreeWidgetItem *item = selectedItems.first();
    QString itemType = item->data(0, Qt::UserRole + 1).toString();
    
    if (itemType == "audio") {
        // Arquivo de áudio selecionado
        auto audioFile = getSelectedAudioFile();
        if (audioFile) {
            emit audioFileSelected(audioFile);
        }
    } else if (itemType == "pitch" || itemType == "intensity") {
        // Objeto associado selecionado
        auto audioFile = getSelectedAudioFile();
        if (audioFile) {
            emit associatedObjectSelected(audioFile, itemType);
        }
    }
}

void AudioListWidget::onItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    
    QString itemType = item->data(0, Qt::UserRole + 1).toString();
    
    if (itemType == "audio") {
        // Duplo clique no arquivo - selecionar e focar
        auto audioFile = getSelectedAudioFile();
        if (audioFile) {
            emit audioFileSelected(audioFile);
        }
    }
}

void AudioListWidget::onCustomContextMenuRequested(const QPoint &pos)
{
    QTreeWidgetItem *item = m_treeWidget->itemAt(pos);
    if (!item) {
        return;
    }
    
    QString itemType = item->data(0, Qt::UserRole + 1).toString();
    
    // Mostrar menu apropriado baseado no tipo
    if (itemType == "audio") {
        // Menu para arquivo de áudio
        QMenu menu(this);
        menu.addAction("Ver Metadados");
        menu.addSeparator();
        menu.addAction("Calcular Pitch (F0)");
        menu.addAction("Calcular Intensidade");
        menu.addSeparator();
        menu.addAction("Remover Arquivo");
        menu.exec(m_treeWidget->viewport()->mapToGlobal(pos));
    }
}

void AudioListWidget::onAudioFileAdded(int index)
{
    updateList();
}

void AudioListWidget::onAudioFileRemoved(int index)
{
    updateList();
}

void AudioListWidget::createContextMenu()
{
    // Context menu será criado dinamicamente em onCustomContextMenuRequested
}

void AudioListWidget::updateAudioFileItem(QTreeWidgetItem *item, std::shared_ptr<AudioFile> audioFile)
{
    // Remover sub-itens existentes
    QList<QTreeWidgetItem*> children = item->takeChildren();
    qDeleteAll(children);
    
    // Adicionar sub-item para pitch se disponível
    if (audioFile->hasPitchData()) {
        QTreeWidgetItem *pitchItem = new QTreeWidgetItem(item);
        pitchItem->setText(0, "Pitch (F0)");
        pitchItem->setData(0, Qt::UserRole + 1, "pitch");
        pitchItem->setIcon(0, style()->standardIcon(QStyle::SP_FileDialogDetailedView));
        pitchItem->setForeground(0, QColor(0, 100, 200));
    }
    
    // Adicionar sub-item para intensidade se disponível
    if (audioFile->hasIntensityData()) {
        QTreeWidgetItem *intensityItem = new QTreeWidgetItem(item);
        intensityItem->setText(0, "Intensidade");
        intensityItem->setData(0, Qt::UserRole + 1, "intensity");
        intensityItem->setIcon(0, style()->standardIcon(QStyle::SP_FileDialogDetailedView));
        intensityItem->setForeground(0, QColor(200, 100, 0));
    }
}
