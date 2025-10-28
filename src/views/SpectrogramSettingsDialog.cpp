#include "views/SpectrogramSettingsDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>

SpectrogramSettingsDialog::SpectrogramSettingsDialog(const Settings &currentSettings, QWidget *parent)
    : QDialog(parent)
{
    setupUI();
    loadSettings(currentSettings);
    
    setWindowTitle("Configurações do Espectrograma");
    setModal(true);
    resize(500, 600);
}

SpectrogramSettingsDialog::~SpectrogramSettingsDialog()
{
}

void SpectrogramSettingsDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    
    // Grupo: Parâmetros de Cálculo
    QGroupBox *calcGroup = new QGroupBox("Parâmetros de Cálculo", this);
    QFormLayout *calcLayout = new QFormLayout(calcGroup);
    
    m_timeStepSpinBox = new QDoubleSpinBox(this);
    m_timeStepSpinBox->setRange(0.001, 1.0);
    m_timeStepSpinBox->setSingleStep(0.001);
    m_timeStepSpinBox->setDecimals(3);
    m_timeStepSpinBox->setSuffix(" s");
    calcLayout->addRow("Passo de Tempo:", m_timeStepSpinBox);
    
    m_timeWindowSpinBox = new QDoubleSpinBox(this);
    m_timeWindowSpinBox->setRange(0.001, 1.0);
    m_timeWindowSpinBox->setSingleStep(0.001);
    m_timeWindowSpinBox->setDecimals(3);
    m_timeWindowSpinBox->setSuffix(" s");
    calcLayout->addRow("Janela de Tempo:", m_timeWindowSpinBox);
    
    // FFT size - apenas potências de 2
    m_fftSizeComboBox = new QComboBox(this);
    m_fftSizeComboBox->addItem("256", 256);
    m_fftSizeComboBox->addItem("512", 512);
    m_fftSizeComboBox->addItem("1024", 1024);
    m_fftSizeComboBox->addItem("2048", 2048);
    m_fftSizeComboBox->addItem("4096", 4096);
    m_fftSizeComboBox->addItem("8192", 8192);
    m_fftSizeComboBox->addItem("16384", 16384);
    calcLayout->addRow("Tamanho da FFT:", m_fftSizeComboBox);
    
    m_windowTypeComboBox = new QComboBox(this);
    m_windowTypeComboBox->addItems({"Hamming", "Hanning", "Blackman", "Rectangular"});
    calcLayout->addRow("Tipo de Janela:", m_windowTypeComboBox);
    
    mainLayout->addWidget(calcGroup);
    
    // Grupo: Parâmetros de Visualização
    QGroupBox *vizGroup = new QGroupBox("Parâmetros de Visualização", this);
    QFormLayout *vizLayout = new QFormLayout(vizGroup);
    
    m_minFreqSpinBox = new QDoubleSpinBox(this);
    m_minFreqSpinBox->setRange(0, 50000);
    m_minFreqSpinBox->setSingleStep(100);
    m_minFreqSpinBox->setSuffix(" Hz");
    vizLayout->addRow("Frequência Mínima:", m_minFreqSpinBox);
    
    m_maxFreqSpinBox = new QDoubleSpinBox(this);
    m_maxFreqSpinBox->setRange(0, 50000);
    m_maxFreqSpinBox->setSingleStep(100);
    m_maxFreqSpinBox->setSuffix(" Hz");
    vizLayout->addRow("Frequência Máxima:", m_maxFreqSpinBox);
    
    m_dynamicRangeSpinBox = new QDoubleSpinBox(this);
    m_dynamicRangeSpinBox->setRange(20, 120);
    m_dynamicRangeSpinBox->setSingleStep(5);
    m_dynamicRangeSpinBox->setSuffix(" dB");
    vizLayout->addRow("Faixa Dinâmica:", m_dynamicRangeSpinBox);
    
    m_colorMapComboBox = new QComboBox(this);
    m_colorMapComboBox->addItems({"Grayscale", "Jet", "Hot", "Cool", "Viridis"});
    vizLayout->addRow("Mapa de Cores:", m_colorMapComboBox);
    
    mainLayout->addWidget(vizGroup);
    
    // Grupo: Pré-ênfase
    QGroupBox *preEmphGroup = new QGroupBox("Pré-ênfase", this);
    QFormLayout *preEmphLayout = new QFormLayout(preEmphGroup);
    
    m_preEmphasisCheckBox = new QCheckBox("Ativar Pré-ênfase", this);
    connect(m_preEmphasisCheckBox, &QCheckBox::stateChanged, 
            this, &SpectrogramSettingsDialog::updatePreEmphasisState);
    preEmphLayout->addRow(m_preEmphasisCheckBox);
    
    m_preEmphasisFactorSpinBox = new QDoubleSpinBox(this);
    m_preEmphasisFactorSpinBox->setRange(0.0, 1.0);
    m_preEmphasisFactorSpinBox->setSingleStep(0.01);
    m_preEmphasisFactorSpinBox->setDecimals(2);
    m_preEmphasisFactorSpinBox->setEnabled(false);
    preEmphLayout->addRow("Fator:", m_preEmphasisFactorSpinBox);
    
    mainLayout->addWidget(preEmphGroup);
    
    mainLayout->addStretch();
    
    // Botões
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    QPushButton *defaultsButton = new QPushButton("Restaurar Padrões", this);
    connect(defaultsButton, &QPushButton::clicked, this, &SpectrogramSettingsDialog::onRestoreDefaults);
    buttonLayout->addWidget(defaultsButton);
    
    buttonLayout->addStretch();
    
    QPushButton *cancelButton = new QPushButton("Cancelar", this);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    buttonLayout->addWidget(cancelButton);
    
    QPushButton *okButton = new QPushButton("OK", this);
    okButton->setDefault(true);
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    buttonLayout->addWidget(okButton);
    
    mainLayout->addLayout(buttonLayout);
    
    setLayout(mainLayout);
}

void SpectrogramSettingsDialog::loadSettings(const Settings &settings)
{
    m_timeStepSpinBox->setValue(settings.timeStep);
    m_timeWindowSpinBox->setValue(settings.timeWindow);
    
    // Encontrar o índice do FFT size no combo box
    int fftIndex = m_fftSizeComboBox->findData(settings.fftSize);
    if (fftIndex >= 0) {
        m_fftSizeComboBox->setCurrentIndex(fftIndex);
    }
    
    m_windowTypeComboBox->setCurrentText(settings.windowType);
    
    m_minFreqSpinBox->setValue(settings.minFrequency);
    m_maxFreqSpinBox->setValue(settings.maxFrequency);
    m_dynamicRangeSpinBox->setValue(settings.dynamicRange);
    m_colorMapComboBox->setCurrentText(settings.colorMap);
    
    m_preEmphasisCheckBox->setChecked(settings.preEmphasis);
    m_preEmphasisFactorSpinBox->setValue(settings.preEmphasisFactor);
}

SpectrogramSettingsDialog::Settings SpectrogramSettingsDialog::getSettings() const
{
    Settings settings;
    
    settings.timeStep = m_timeStepSpinBox->value();
    settings.timeWindow = m_timeWindowSpinBox->value();
    settings.fftSize = m_fftSizeComboBox->currentData().toInt();
    settings.windowType = m_windowTypeComboBox->currentText();
    
    settings.minFrequency = m_minFreqSpinBox->value();
    settings.maxFrequency = m_maxFreqSpinBox->value();
    settings.dynamicRange = m_dynamicRangeSpinBox->value();
    settings.colorMap = m_colorMapComboBox->currentText();
    
    settings.preEmphasis = m_preEmphasisCheckBox->isChecked();
    settings.preEmphasisFactor = m_preEmphasisFactorSpinBox->value();
    
    return settings;
}

SpectrogramSettingsDialog::Settings SpectrogramSettingsDialog::getDefaultSettings() const
{
    Settings defaults;
    
    // Padrões otimizados
    defaults.timeStep = 0.01;         // 10 ms
    defaults.timeWindow = 0.03;       // 30 ms
    defaults.fftSize = 1024;          // Será ajustado para potência de 2
    defaults.windowType = "Hamming";
    
    defaults.minFrequency = 0.0;
    defaults.maxFrequency = 4000.0;   // 4 kHz
    defaults.dynamicRange = 90.0;     // 90 dB
    defaults.colorMap = "Grayscale";
    
    defaults.preEmphasis = false;
    defaults.preEmphasisFactor = 0.97;
    
    return defaults;
}

void SpectrogramSettingsDialog::onRestoreDefaults()
{
    loadSettings(getDefaultSettings());
}

void SpectrogramSettingsDialog::updatePreEmphasisState(int state)
{
    m_preEmphasisFactorSpinBox->setEnabled(state == Qt::Checked);
}
