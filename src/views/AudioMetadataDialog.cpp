#include "views/AudioMetadataDialog.h"
#include "models/AudioFile.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QLabel>
#include <QFileInfo>
#include <QDateTime>

AudioMetadataDialog::AudioMetadataDialog(std::shared_ptr<AudioFile> audioFile, QWidget *parent)
    : QDialog(parent)
    , m_audioFile(audioFile)
    , m_metadataTable(nullptr)
{
    setupUI();
    populateMetadata();
    
    setWindowTitle("Metadados do Áudio");
    setModal(true);
    resize(600, 500);
}

AudioMetadataDialog::~AudioMetadataDialog()
{
}

void AudioMetadataDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    
    // Título
    QLabel *titleLabel = new QLabel(this);
    if (m_audioFile) {
        QFileInfo fileInfo(m_audioFile->getFilePath());
        titleLabel->setText("<h2>" + fileInfo.fileName() + "</h2>");
    }
    mainLayout->addWidget(titleLabel);
    
    // Tabela de metadados
    m_metadataTable = new QTableWidget(this);
    m_metadataTable->setColumnCount(2);
    m_metadataTable->setHorizontalHeaderLabels({"Propriedade", "Valor"});
    m_metadataTable->horizontalHeader()->setStretchLastSection(true);
    m_metadataTable->verticalHeader()->setVisible(false);
    m_metadataTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_metadataTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_metadataTable->setAlternatingRowColors(true);
    
    mainLayout->addWidget(m_metadataTable);
    
    // Botão Fechar
    QPushButton *closeButton = new QPushButton("Fechar", this);
    closeButton->setFixedWidth(100);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);
    
    mainLayout->addLayout(buttonLayout);
    
    setLayout(mainLayout);
}

void AudioMetadataDialog::populateMetadata()
{
    if (!m_audioFile) {
        return;
    }
    
    QFileInfo fileInfo(m_audioFile->getFilePath());
    
    QList<QPair<QString, QString>> metadata;
    
    // Informações do arquivo
    metadata.append({"Nome do Arquivo", fileInfo.fileName()});
    metadata.append({"Caminho Completo", fileInfo.absoluteFilePath()});
    metadata.append({"Tamanho do Arquivo", formatFileSize(fileInfo.size())});
    metadata.append({"Data de Modificação", fileInfo.lastModified().toString("dd/MM/yyyy HH:mm:ss")});
    
    // Separador
    metadata.append(QPair<QString, QString>("", ""));
    
    // Informações de áudio
    metadata.append({"Formato", m_audioFile->getCodec()});
    metadata.append({"Taxa de Amostragem", QString::number(m_audioFile->getSampleRate()) + " Hz"});
    metadata.append({"Número de Canais", QString::number(m_audioFile->getNumChannels())});
    metadata.append({"Duração", formatDuration(m_audioFile->getDuration())});
    metadata.append({"Número de Amostras", QString::number(m_audioFile->getNumSamples())});
    
    // Cálculos derivados
    double bitrate = (fileInfo.size() * 8.0) / m_audioFile->getDuration() / 1000.0; // kbps
    metadata.append({"Bitrate Médio", QString::number(bitrate, 'f', 1) + " kbps"});
    
    int bitsPerSample = 16; // Assumindo 16-bit (pode ser ajustado)
    metadata.append({"Bits por Amostra", QString::number(bitsPerSample) + " bits"});
    
    // Separador
    metadata.append(QPair<QString, QString>("", ""));
    
    // Estatísticas de amplitude
    const QVector<float> &samples = m_audioFile->getSamples();
    if (!samples.isEmpty()) {
        float minSample = *std::min_element(samples.begin(), samples.end());
        float maxSample = *std::max_element(samples.begin(), samples.end());
        float avgSample = std::accumulate(samples.begin(), samples.end(), 0.0f) / samples.size();
        
        metadata.append({"Amplitude Mínima", QString::number(minSample, 'f', 4)});
        metadata.append({"Amplitude Máxima", QString::number(maxSample, 'f', 4)});
        metadata.append({"Amplitude Média", QString::number(avgSample, 'f', 4)});
        
        // RMS (Root Mean Square)
        float rms = 0.0f;
        for (float sample : samples) {
            rms += sample * sample;
        }
        rms = std::sqrt(rms / samples.size());
        metadata.append({"RMS", QString::number(rms, 'f', 4)});
        
        // Peak dB
        float peakDb = 20.0f * std::log10(std::max(std::abs(minSample), std::abs(maxSample)));
        metadata.append({"Pico (dB)", QString::number(peakDb, 'f', 2) + " dB"});
    }
    
    // Preencher tabela
    m_metadataTable->setRowCount(metadata.size());
    
    for (int i = 0; i < metadata.size(); ++i) {
        QTableWidgetItem *keyItem = new QTableWidgetItem(metadata[i].first);
        QTableWidgetItem *valueItem = new QTableWidgetItem(metadata[i].second);
        
        // Separadores em negrito
        if (metadata[i].first.isEmpty()) {
            QFont font = keyItem->font();
            font.setBold(true);
            keyItem->setFont(font);
            valueItem->setFont(font);
            keyItem->setBackground(QColor(240, 240, 240));
            valueItem->setBackground(QColor(240, 240, 240));
        } else {
            QFont font = keyItem->font();
            font.setBold(true);
            keyItem->setFont(font);
        }
        
        m_metadataTable->setItem(i, 0, keyItem);
        m_metadataTable->setItem(i, 1, valueItem);
    }
    
    m_metadataTable->resizeColumnToContents(0);
}

QString AudioMetadataDialog::formatFileSize(qint64 bytes) const
{
    const qint64 KB = 1024;
    const qint64 MB = KB * 1024;
    const qint64 GB = MB * 1024;
    
    if (bytes >= GB) {
        return QString::number(bytes / (double)GB, 'f', 2) + " GB";
    } else if (bytes >= MB) {
        return QString::number(bytes / (double)MB, 'f', 2) + " MB";
    } else if (bytes >= KB) {
        return QString::number(bytes / (double)KB, 'f', 2) + " KB";
    } else {
        return QString::number(bytes) + " bytes";
    }
}

QString AudioMetadataDialog::formatDuration(double seconds) const
{
    int hours = static_cast<int>(seconds) / 3600;
    int minutes = (static_cast<int>(seconds) % 3600) / 60;
    int secs = static_cast<int>(seconds) % 60;
    int ms = static_cast<int>((seconds - static_cast<int>(seconds)) * 1000);
    
    if (hours > 0) {
        return QString("%1:%2:%3.%4")
            .arg(hours, 2, 10, QChar('0'))
            .arg(minutes, 2, 10, QChar('0'))
            .arg(secs, 2, 10, QChar('0'))
            .arg(ms, 3, 10, QChar('0'));
    } else {
        return QString("%1:%2.%3")
            .arg(minutes, 2, 10, QChar('0'))
            .arg(secs, 2, 10, QChar('0'))
            .arg(ms, 3, 10, QChar('0'));
    }
}
