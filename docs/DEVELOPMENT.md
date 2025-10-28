# Guia de Desenvolvimento - AudioAnnotator

Este documento fornece orientações para desenvolvedores que desejam contribuir ou estender o projeto AudioAnnotator.

## Configuração do Ambiente de Desenvolvimento

### Requisitos

O desenvolvimento do AudioAnnotator requer as seguintes ferramentas e bibliotecas instaladas no sistema:

**Ferramentas de Build:**
- CMake 3.16 ou superior
- Compilador C++17 (GCC 7+, Clang 5+, MSVC 2017+)
- Make ou Ninja

**Bibliotecas:**
- Qt6 (Core, Widgets, Multimedia)
- FFTW3 (para cálculo de FFT)
- libsndfile (para decodificação de áudio)

**IDE Recomendado:**
- Visual Studio Code com extensões:
  - C/C++ (Microsoft)
  - CMake Tools (Microsoft)
  - Qt tools (opcional)

### Instalação no Linux (Ubuntu/Debian)

Execute os seguintes comandos para instalar todas as dependências:

```bash
sudo apt-get update
sudo apt-get install -y \
    qt6-base-dev \
    qt6-multimedia-dev \
    libfftw3-dev \
    libsndfile1-dev \
    cmake \
    build-essential \
    git
```

### Instalação no Windows

**Opção 1: vcpkg (Recomendado)**

```powershell
# Instalar vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat

# Instalar dependências
.\vcpkg install qt6:x64-windows fftw3:x64-windows libsndfile:x64-windows

# Integrar com Visual Studio
.\vcpkg integrate install
```

**Opção 2: Instalação Manual**

1. Baixe e instale Qt6 de https://www.qt.io/download
2. Baixe FFTW3 de https://www.fftw.org/install/windows.html
3. Baixe libsndfile de https://github.com/libsndfile/libsndfile/releases

### Instalação no macOS

Use Homebrew para instalar as dependências:

```bash
brew install qt6 fftw libsndfile cmake
```

## Compilação do Projeto

### Compilação Básica

```bash
# Clone o repositório (se ainda não tiver)
git clone <repository-url>
cd AudioAnnotator

# Crie o diretório de build
mkdir build
cd build

# Configure com CMake
cmake ..

# Compile
make -j$(nproc)

# Execute
./bin/AudioAnnotator
```

### Compilação com Opções

**Build de Debug (com símbolos de debug):**

```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)
```

**Build de Release (otimizado):**

```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
```

**Especificar caminho do Qt6 (se necessário):**

```bash
cmake -DCMAKE_PREFIX_PATH=/path/to/qt6 ..
```

### Limpeza e Recompilação

Para limpar completamente o build e recompilar:

```bash
rm -rf build
mkdir build
cd build
cmake ..
make -j$(nproc)
```

## Desenvolvimento no VS Code

O projeto inclui configurações pré-definidas para o Visual Studio Code na pasta `.vscode/`.

### Tarefas Disponíveis

Pressione `Ctrl+Shift+P` (ou `Cmd+Shift+P` no macOS) e digite "Tasks: Run Task" para acessar:

- **Build AudioAnnotator**: Compila o projeto
- **Clean Build**: Limpa e reconfigura o build
- **Run AudioAnnotator**: Compila e executa a aplicação

### Debug

Para iniciar o debug:

1. Pressione `F5` ou vá em Run → Start Debugging
2. O projeto será compilado automaticamente
3. O debugger será anexado à aplicação

Breakpoints podem ser adicionados clicando à esquerda dos números de linha no editor.

## Estrutura do Código

### Adicionando uma Nova Classe

Para adicionar uma nova classe ao projeto, siga estes passos:

**1. Crie o arquivo de cabeçalho (.h)**

Exemplo: `include/models/NewClass.h`

```cpp
#ifndef NEWCLASS_H
#define NEWCLASS_H

#include <QObject>

class NewClass : public QObject
{
    Q_OBJECT

public:
    explicit NewClass(QObject *parent = nullptr);
    ~NewClass();
    
    // Métodos públicos
    void doSomething();
    
signals:
    // Sinais Qt
    void somethingHappened();
    
private:
    // Membros privados
    int m_value;
};

#endif // NEWCLASS_H
```

**2. Crie o arquivo de implementação (.cpp)**

Exemplo: `src/models/NewClass.cpp`

```cpp
#include "models/NewClass.h"

NewClass::NewClass(QObject *parent)
    : QObject(parent)
    , m_value(0)
{
}

NewClass::~NewClass()
{
}

void NewClass::doSomething()
{
    m_value++;
    emit somethingHappened();
}
```

**3. Adicione ao CMakeLists.txt**

Edite `CMakeLists.txt` e adicione os novos arquivos às listas `SOURCES` e `HEADERS`:

```cmake
set(SOURCES
    # ... arquivos existentes ...
    src/models/NewClass.cpp
)

set(HEADERS
    # ... arquivos existentes ...
    include/models/NewClass.h
)
```

**4. Recompile o projeto**

```bash
cd build
make -j$(nproc)
```

### Convenções de Código

**Nomenclatura:**

- **Classes**: `PascalCase` (ex: `AudioFile`, `MainWindow`)
- **Métodos**: `camelCase` (ex: `getFileName()`, `setAudioFile()`)
- **Variáveis membro**: `m_camelCase` (ex: `m_audioFile`, `m_sampleRate`)
- **Variáveis locais**: `camelCase` (ex: `audioFile`, `sampleRate`)
- **Constantes**: `UPPER_SNAKE_CASE` (ex: `MAX_CHANNELS`, `DEFAULT_SAMPLE_RATE`)

**Formatação:**

- Indentação: 4 espaços (não tabs)
- Chaves: Estilo Allman (chave de abertura em nova linha)
- Linhas: Máximo de 120 caracteres

**Exemplo:**

```cpp
void AudioFile::setSamples(int channel, const QVector<float> &samples)
{
    if (channel >= 0 && channel < m_numChannels) {
        m_channelSamples[channel] = samples;
        if (m_numSamples == 0) {
            m_numSamples = samples.size();
            if (m_sampleRate > 0) {
                m_duration = static_cast<double>(m_numSamples) / m_sampleRate;
            }
        }
    }
}
```

## Implementando Funcionalidades

### Implementando Decodificação de Áudio

A classe `AudioDecoder` é responsável por decodificar arquivos de áudio. Para implementá-la:

**1. Use Qt Multimedia para decodificação básica:**

```cpp
#include <QAudioDecoder>
#include <QAudioBuffer>

void AudioDecoder::decode(const QString &filePath, AudioFile *audioFile)
{
    QAudioDecoder decoder;
    decoder.setSource(QUrl::fromLocalFile(filePath));
    
    connect(&decoder, &QAudioDecoder::bufferReady, [&]() {
        QAudioBuffer buffer = decoder.read();
        // Processar buffer e preencher audioFile
    });
    
    decoder.start();
}
```

**2. Use libsndfile para formatos específicos:**

```cpp
#include <sndfile.h>

void AudioDecoder::decodeWithLibsndfile(const QString &filePath, AudioFile *audioFile)
{
    SF_INFO sfInfo;
    SNDFILE *file = sf_open(filePath.toStdString().c_str(), SFM_READ, &sfInfo);
    
    if (file) {
        audioFile->setSampleRate(sfInfo.samplerate);
        audioFile->setNumChannels(sfInfo.channels);
        
        // Ler amostras...
        
        sf_close(file);
    }
}
```

### Implementando Visualização de Waveform

A classe `AudioVisualizationWidget` deve desenhar a forma de onda. Implementação sugerida:

```cpp
void AudioVisualizationWidget::drawWaveform(QPainter &painter)
{
    if (!m_audioFile || !m_audioFile->isLoaded()) {
        return;
    }
    
    QVector<float> samples = m_audioFile->getMixedSamples();
    int numSamples = samples.size();
    
    double samplesPerPixel = (m_viewDuration * m_audioFile->getSampleRate()) / width();
    
    painter.setPen(QPen(Qt::blue, 1));
    
    for (int x = 0; x < width(); ++x) {
        double time = m_viewStartTime + (x / (double)width()) * m_viewDuration;
        int sampleIndex = (int)(time * m_audioFile->getSampleRate());
        
        if (sampleIndex >= 0 && sampleIndex < numSamples) {
            float value = samples[sampleIndex];
            int y = height() / 2 - (int)(value * height() / 2);
            
            if (x == 0) {
                painter.drawLine(x, height() / 2, x, y);
            } else {
                painter.drawLine(x - 1, lastY, x, y);
            }
            
            lastY = y;
        }
    }
}
```

### Implementando Cálculo de Espectrograma

A classe `SpectrogramCalculator` deve usar FFTW3 para calcular o espectrograma:

```cpp
#include <fftw3.h>

QVector<QVector<float>> SpectrogramCalculator::calculate(const QVector<float> &samples,
                                                          int sampleRate,
                                                          int windowSize,
                                                          int hopSize)
{
    int numFrames = (samples.size() - windowSize) / hopSize + 1;
    int fftSize = windowSize;
    int numBins = fftSize / 2 + 1;
    
    QVector<QVector<float>> spectrogram(numFrames);
    
    // Alocar buffers FFTW
    double *in = (double*)fftw_malloc(sizeof(double) * fftSize);
    fftw_complex *out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * numBins);
    
    fftw_plan plan = fftw_plan_dft_r2c_1d(fftSize, in, out, FFTW_ESTIMATE);
    
    // Janela de Hamming
    QVector<double> window(windowSize);
    for (int i = 0; i < windowSize; ++i) {
        window[i] = 0.54 - 0.46 * cos(2.0 * M_PI * i / (windowSize - 1));
    }
    
    // Calcular espectrograma
    for (int frame = 0; frame < numFrames; ++frame) {
        int startSample = frame * hopSize;
        
        // Aplicar janela
        for (int i = 0; i < windowSize; ++i) {
            in[i] = samples[startSample + i] * window[i];
        }
        
        // Zero padding
        for (int i = windowSize; i < fftSize; ++i) {
            in[i] = 0.0;
        }
        
        // Executar FFT
        fftw_execute(plan);
        
        // Calcular magnitudes
        spectrogram[frame].resize(numBins);
        for (int bin = 0; bin < numBins; ++bin) {
            double real = out[bin][0];
            double imag = out[bin][1];
            spectrogram[frame][bin] = sqrt(real * real + imag * imag);
        }
    }
    
    // Limpar
    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);
    
    return spectrogram;
}
```

### Implementando Exportação TextGrid

A classe `TextGridExporter` deve exportar anotações no formato Praat:

```cpp
bool TextGridExporter::exportToFile(const QString &filePath,
                                     const QVector<std::shared_ptr<AnnotationTier>> &tiers,
                                     double xmin,
                                     double xmax,
                                     bool shortFormat)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    
    QTextStream out(&file);
    
    // Cabeçalho
    out << "File type = \"ooTextFile\"\n";
    out << "Object class = \"TextGrid\"\n";
    out << xmin << "\n";
    out << xmax << "\n";
    out << "<exists>\n";
    out << tiers.size() << "\n";
    
    // Tiers
    for (int i = 0; i < tiers.size(); ++i) {
        auto tier = tiers[i];
        
        if (tier->getType() == TierType::IntervalTier) {
            out << "\"IntervalTier\"\n";
            out << "\"" << tier->getName() << "\"\n";
            out << tier->getXMin() << "\n";
            out << tier->getXMax() << "\n";
            out << tier->getIntervalCount() << "\n";
            
            for (int j = 0; j < tier->getIntervalCount(); ++j) {
                auto interval = tier->getInterval(j);
                out << interval->getXMin() << "\n";
                out << interval->getXMax() << "\n";
                out << "\"" << interval->getText() << "\"\n";
            }
        } else {
            out << "\"TextTier\"\n";
            out << "\"" << tier->getName() << "\"\n";
            out << tier->getXMin() << "\n";
            out << tier->getXMax() << "\n";
            out << tier->getPointCount() << "\n";
            
            for (int j = 0; j < tier->getPointCount(); ++j) {
                auto point = tier->getPoint(j);
                out << point->getTime() << "\n";
                out << "\"" << point->getText() << "\"\n";
            }
        }
    }
    
    file.close();
    return true;
}
```

## Debugging

### Debugging com GDB (Linux/macOS)

```bash
# Compile com símbolos de debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)

# Execute com gdb
gdb ./bin/AudioAnnotator

# Comandos úteis no gdb:
# run - executa o programa
# break MainWindow::onOpenAudioFiles - adiciona breakpoint
# continue - continua execução
# step - executa próxima linha
# print m_audioFile - imprime variável
# backtrace - mostra stack trace
```

### Debugging com Visual Studio (Windows)

1. Abra o projeto no Visual Studio
2. Configure como Debug
3. Pressione F5 para iniciar o debug
4. Adicione breakpoints clicando à esquerda dos números de linha

### Debugging de Sinais Qt

Para debugar sinais e slots do Qt, use `qDebug()`:

```cpp
#include <QDebug>

void MainWindow::onAudioFileSelected(std::shared_ptr<AudioFile> audioFile)
{
    qDebug() << "Audio file selected:" << audioFile->getFileName();
    qDebug() << "Duration:" << audioFile->getDuration() << "seconds";
}
```

## Testes

### Testes Manuais

Para testar manualmente a aplicação:

1. Compile e execute
2. Abra um arquivo de áudio de teste
3. Verifique se a visualização aparece
4. Adicione uma camada de anotação
5. Crie alguns intervalos
6. Exporte para TextGrid
7. Abra o TextGrid no Praat para verificar compatibilidade

### Testes Automatizados (Futuro)

O projeto está preparado para adicionar testes automatizados usando Qt Test:

```cpp
#include <QtTest/QtTest>
#include "models/AudioFile.h"

class TestAudioFile : public QObject
{
    Q_OBJECT

private slots:
    void testGetFileName();
    void testGetDuration();
};

void TestAudioFile::testGetFileName()
{
    AudioFile audioFile("/path/to/audio.wav");
    QCOMPARE(audioFile.getFileName(), QString("audio.wav"));
}

void TestAudioFile::testGetDuration()
{
    AudioFile audioFile;
    audioFile.setSampleRate(44100);
    audioFile.setNumSamples(44100);
    QCOMPARE(audioFile.getDuration(), 1.0);
}

QTEST_MAIN(TestAudioFile)
#include "test_audiofile.moc"
```

## Resolução de Problemas Comuns

### Erro: "Qt6 not found"

**Solução**: Especifique o caminho do Qt6 ao configurar com CMake:

```bash
cmake -DCMAKE_PREFIX_PATH=/path/to/qt6 ..
```

### Erro: "FFTW3 not found"

**Solução**: Instale FFTW3 ou especifique o caminho:

```bash
# Linux
sudo apt-get install libfftw3-dev

# macOS
brew install fftw

# Windows (vcpkg)
vcpkg install fftw3:x64-windows
```

### Erro de Compilação: "undefined reference to vtable"

**Causa**: Geralmente ocorre quando uma classe com Q_OBJECT não foi processada pelo MOC.

**Solução**: Limpe e recompile o projeto:

```bash
rm -rf build
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### Aplicação não inicia (Linux)

**Causa**: Bibliotecas Qt não encontradas.

**Solução**: Adicione o caminho das bibliotecas Qt ao LD_LIBRARY_PATH:

```bash
export LD_LIBRARY_PATH=/path/to/qt6/lib:$LD_LIBRARY_PATH
./bin/AudioAnnotator
```

## Recursos Adicionais

### Documentação Qt6

- Qt Documentation: https://doc.qt.io/qt-6/
- Qt Widgets: https://doc.qt.io/qt-6/qtwidgets-index.html
- Qt Multimedia: https://doc.qt.io/qt-6/qtmultimedia-index.html

### Documentação FFTW3

- FFTW Manual: https://www.fftw.org/fftw3_doc/
- Tutorial: https://www.fftw.org/fftw2_doc/fftw_2.html

### Documentação Praat TextGrid

- TextGrid File Formats: https://www.fon.hum.uva.nl/praat/manual/TextGrid_file_formats.html

### Algoritmo YAAPT

- YAAPT Paper: S. A. Zahorian and H. Hu, "A spectral/temporal method for robust fundamental frequency tracking," J. Acoust. Soc. Am. 123(6), June 2008
- YAAPT MATLAB: http://ws2.binghamton.edu/zahorian/yaapt.htm
- Python Implementation: https://github.com/bjbschmitt/AMFM_decompy

## Contribuindo

Para contribuir com o projeto:

1. Faça um fork do repositório
2. Crie uma branch para sua feature (`git checkout -b feature/MinhaFeature`)
3. Commit suas mudanças (`git commit -am 'Adiciona MinhaFeature'`)
4. Push para a branch (`git push origin feature/MinhaFeature`)
5. Abra um Pull Request

## Suporte

Para dúvidas ou problemas:

- Abra uma issue no repositório do projeto
- Consulte a documentação em `docs/`
- Entre em contato com os mantenedores do projeto

