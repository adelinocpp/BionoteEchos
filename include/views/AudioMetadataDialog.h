#ifndef AUDIOMETADATADIALOG_H
#define AUDIOMETADATADIALOG_H

#include <QDialog>
#include <memory>

class AudioFile;
class QLabel;
class QTableWidget;

/**
 * @brief Diálogo para exibir metadados detalhados de um arquivo de áudio
 */
class AudioMetadataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AudioMetadataDialog(std::shared_ptr<AudioFile> audioFile, QWidget *parent = nullptr);
    ~AudioMetadataDialog();

private:
    void setupUI();
    void populateMetadata();
    QString formatFileSize(qint64 bytes) const;
    QString formatDuration(double seconds) const;

private:
    std::shared_ptr<AudioFile> m_audioFile;
    QTableWidget *m_metadataTable;
};

#endif // AUDIOMETADATADIALOG_H
