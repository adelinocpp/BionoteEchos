#ifndef SPECTROGRAMSETTINGSDIALOG_H
#define SPECTROGRAMSETTINGSDIALOG_H

#include <QDialog>

class QDoubleSpinBox;
class QSpinBox;
class QComboBox;
class QCheckBox;
class QSlider;
class QLabel;

/**
 * @brief Diálogo para configuração do espectrograma
 */
class SpectrogramSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    struct Settings {
        // Parâmetros de cálculo
        double timeStep;          // Passo de tempo (s)
        double timeWindow;        // Janela de tempo (s)
        int fftSize;              // Tamanho da FFT
        QString windowType;       // Tipo de janela (Hamming, Hanning, etc.)
        
        // Parâmetros de visualização
        double minFrequency;      // Frequência mínima (Hz)
        double maxFrequency;      // Frequência máxima (Hz)
        double dynamicRange;      // Faixa dinâmica (dB)
        QString colorMap;         // Mapa de cores
        bool preEmphasis;         // Pré-ênfase
        double preEmphasisFactor; // Fator de pré-ênfase
    };

    explicit SpectrogramSettingsDialog(const Settings &currentSettings, QWidget *parent = nullptr);
    ~SpectrogramSettingsDialog();
    
    Settings getSettings() const;

private slots:
    void onRestoreDefaults();
    void updatePreEmphasisState(int state);

private:
    void setupUI();
    void loadSettings(const Settings &settings);
    Settings getDefaultSettings() const;

private:
    // Cálculo
    QDoubleSpinBox *m_timeStepSpinBox;
    QDoubleSpinBox *m_timeWindowSpinBox;
    QComboBox *m_fftSizeComboBox;
    QComboBox *m_windowTypeComboBox;
    
    // Visualização
    QDoubleSpinBox *m_minFreqSpinBox;
    QDoubleSpinBox *m_maxFreqSpinBox;
    QDoubleSpinBox *m_dynamicRangeSpinBox;
    QComboBox *m_colorMapComboBox;
    QCheckBox *m_preEmphasisCheckBox;
    QDoubleSpinBox *m_preEmphasisFactorSpinBox;
};

#endif // SPECTROGRAMSETTINGSDIALOG_H
