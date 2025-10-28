# Arquitetura do AudioAnnotator

## Visão Geral

O **AudioAnnotator** é uma aplicação desktop multiplataforma desenvolvida em **C++17** com **Qt6**, seguindo o padrão arquitetural **Model-View-Controller (MVC)** para garantir separação de responsabilidades, manutenibilidade e extensibilidade.

## Estrutura de Diretórios

```
AudioAnnotator/
├── CMakeLists.txt              # Configuração do sistema de build CMake
├── README.md                   # Documentação principal do projeto
├── .gitignore                  # Arquivos ignorados pelo Git
├── .vscode/                    # Configurações do Visual Studio Code
│   ├── settings.json          # Configurações gerais do editor
│   ├── launch.json            # Configurações de debug
│   ├── tasks.json             # Tarefas de build e execução
│   └── c_cpp_properties.json  # Configurações do IntelliSense C++
├── include/                    # Arquivos de cabeçalho (.h)
│   ├── models/                # Classes de modelo de dados
│   ├── views/                 # Classes de interface gráfica
│   ├── controllers/           # Classes de controle
│   ├── audio/                 # Classes de processamento de áudio
│   └── utils/                 # Classes utilitárias
├── src/                        # Arquivos de implementação (.cpp)
│   ├── main.cpp               # Ponto de entrada da aplicação
│   ├── models/
│   ├── views/
│   ├── controllers/
│   ├── audio/
│   └── utils/
├── resources/                  # Recursos (ícones, imagens, etc.)
├── docs/                       # Documentação adicional
│   └── ARCHITECTURE.md        # Este arquivo
└── build/                      # Diretório de compilação (gerado)
    └── bin/                   # Binários compilados
```

## Padrão MVC

### Models (Modelos)

Os modelos representam os dados da aplicação e a lógica de negócio. São independentes da interface gráfica.

#### AudioFile
- **Responsabilidade**: Representa um arquivo de áudio com seus metadados e dados de amostra
- **Atributos principais**:
  - Caminho do arquivo
  - Taxa de amostragem, número de canais, duração
  - Dados de amostra (waveform) por canal
  - Dados de pitch (F0) e intensidade (quando calculados)
- **Métodos principais**:
  - `load()`: Carrega o arquivo de áudio
  - `getSamples()`: Obtém amostras de um canal
  - `getPitchAtTime()`: Obtém pitch em um tempo específico
  - `getIntensityAtTime()`: Obtém intensidade em um tempo específico

#### AnnotationTier
- **Responsabilidade**: Representa uma camada de anotação (IntervalTier ou PointTier)
- **Tipos**:
  - **IntervalTier**: Contém intervalos com início, fim e texto
  - **PointTier (TextTier)**: Contém pontos no tempo com texto
- **Métodos principais**:
  - `addInterval()`: Adiciona um intervalo
  - `addPoint()`: Adiciona um ponto
  - `findIntervalAtTime()`: Encontra intervalo em uma posição temporal

#### AnnotationInterval
- **Responsabilidade**: Representa um intervalo de anotação
- **Atributos**: xmin, xmax, text
- **Métodos**: `contains()`, `setBounds()`

#### AnnotationPoint
- **Responsabilidade**: Representa um ponto de anotação
- **Atributos**: time, text
- **Métodos**: `distanceTo()`

#### Project
- **Responsabilidade**: Gerencia o projeto completo (arquivos de áudio e camadas de anotação)
- **Atributos**:
  - Lista de arquivos de áudio
  - Lista de camadas de anotação
  - Caminho do projeto
  - Estado de modificação
- **Métodos principais**:
  - `addAudioFile()`: Adiciona arquivo de áudio ao projeto
  - `addTier()`: Adiciona camada de anotação
  - `clear()`: Limpa todos os dados

### Views (Visualizações)

As visualizações são responsáveis pela apresentação dos dados ao usuário e captura de interações.

#### MainWindow
- **Responsabilidade**: Janela principal da aplicação
- **Componentes**:
  - Barra de menu (File, View, Annotation, Analysis, Help)
  - Barras de ferramentas
  - Splitters para organização do layout
  - Barra de status
- **Layout**:
  - Painel esquerdo: AudioListWidget
  - Painel central: AudioVisualizationWidget + AnnotationLayerWidget
  - Painel inferior: AudioControlWidget

#### AudioListWidget
- **Responsabilidade**: Lista de arquivos de áudio e objetos associados
- **Estrutura**: QTreeWidget hierárquico
  - Arquivo de áudio
    - Pitch data
    - Intensity data
- **Interações**:
  - Seleção de arquivo
  - Menu de contexto
  - Duplo clique para ações

#### AudioVisualizationWidget
- **Responsabilidade**: Visualização interativa de forma de onda e espectrograma
- **Funcionalidades**:
  - Desenho de waveform
  - Visualização de espectrograma (quando habilitado)
  - Zoom horizontal (roda do mouse)
  - Navegação temporal (arrastar)
  - Seleção de trechos
  - Labels de tempo (início e fim)
  - Cursor de reprodução
- **Interações**:
  - Mouse wheel: zoom
  - Drag: navegação
  - Click: posicionar cursor
  - Shift+drag: seleção de trecho

#### AnnotationLayerWidget
- **Responsabilidade**: Visualização e edição de até 2 camadas de anotação
- **Funcionalidades**:
  - Desenho de intervalos e pontos
  - Edição inline de texto
  - Movimentação de fronteiras de intervalos
  - Adição/remoção de intervalos e pontos
  - Sincronização com AudioVisualizationWidget
- **Interações**:
  - Click: selecionar intervalo/ponto
  - Double-click: editar texto
  - Drag boundary: mover fronteira
  - Ctrl+click: adicionar intervalo/ponto

#### AudioControlWidget
- **Responsabilidade**: Controles de reprodução de áudio
- **Componentes**:
  - Botões: Play/Pause, Stop
  - Checkbox: Loop
  - Slider de posição
  - Labels de tempo (atual / total)
  - Slider de volume
- **Funcionalidades**:
  - Reprodução normal
  - Reprodução em loop de trecho selecionado
  - Controle de volume

#### SpectrogramWidget
- **Responsabilidade**: Renderização do espectrograma
- **Funcionalidades**:
  - Cálculo de FFT usando FFTW3
  - Renderização com paleta de cores
  - Sincronização com waveform

### Controllers (Controladores)

Os controladores fazem a mediação entre modelos e visualizações, implementando a lógica de aplicação.

#### ProjectController
- **Responsabilidade**: Gerencia operações de projeto
- **Operações**:
  - Novo projeto
  - Abrir projeto
  - Salvar projeto
  - Salvar projeto como
  - Fechar projeto
- **Formato de arquivo**: JSON ou binário (a ser definido)

#### AudioController
- **Responsabilidade**: Gerencia operações de áudio
- **Operações**:
  - Abrir arquivos de áudio
  - Fechar arquivos de áudio
  - Reproduzir/pausar/parar
  - Controle de volume
  - Reprodução em loop
- **Integração**: Qt Multimedia (QAudioDecoder, QMediaPlayer)

#### AnnotationController
- **Responsabilidade**: Gerencia operações de anotação
- **Operações**:
  - Adicionar/remover camadas
  - Renomear camadas
  - Adicionar/editar/remover intervalos
  - Adicionar/editar/remover pontos
  - Validação de anotações

### Audio (Processamento de Áudio)

Classes especializadas em processamento de sinais de áudio.

#### AudioDecoder
- **Responsabilidade**: Decodifica arquivos de áudio
- **Formatos suportados**: Opus, WAV, MP3, OGG, FLAC, etc.
- **Biblioteca**: Qt Multimedia (QAudioDecoder) + libsndfile
- **Saída**: Amostras PCM em float32

#### AudioPlayer
- **Responsabilidade**: Reproduz áudio
- **Biblioteca**: Qt Multimedia (QMediaPlayer, QAudioSink)
- **Funcionalidades**:
  - Reprodução normal
  - Reprodução de trecho
  - Loop
  - Controle de volume

#### SpectrogramCalculator
- **Responsabilidade**: Calcula espectrograma
- **Algoritmo**: Short-Time Fourier Transform (STFT)
- **Biblioteca**: FFTW3
- **Parâmetros configuráveis**:
  - Tamanho da janela (window size)
  - Passo de tempo (hop size)
  - Tipo de janela (Hamming, Hann, Blackman, etc.)
  - Número de pontos FFT (com zero padding)
- **Saída**: Matriz de magnitudes espectrais

#### PitchDetector
- **Responsabilidade**: Detecta frequência fundamental (F0/pitch)
- **Algoritmo**: YAAPT (Yet Another Algorithm for Pitch Tracking)
- **Parâmetros configuráveis**:
  - F0 mínimo e máximo
  - Frame length e frame space
  - Parâmetros de NLFER, SHC, NCCF
- **Alternativas**: Autocorrelação, YIN, SWIPE (para implementação futura)
- **Saída**: Vetor de valores de F0 por frame (0 = unvoiced)

#### IntensityCalculator
- **Responsabilidade**: Calcula intensidade do sinal
- **Algoritmo**: RMS (Root Mean Square) em janelas deslizantes
- **Parâmetros**:
  - Tamanho da janela
  - Passo de tempo
- **Saída**: Vetor de valores de intensidade em dB

### Utils (Utilitários)

Classes utilitárias para funcionalidades auxiliares.

#### TextGridExporter
- **Responsabilidade**: Exporta anotações para formato TextGrid do Praat
- **Formatos suportados**:
  - Texto longo (long text format)
  - Texto curto (short text format)
  - Binário (binary format) - opcional
- **Compatibilidade**: 100% compatível com Praat

#### TextGridImporter
- **Responsabilidade**: Importa anotações de formato TextGrid do Praat
- **Formatos suportados**: Todos os formatos do Praat
- **Parsing**: Robusto, aceita variações de formatação

## Fluxo de Dados

### Abertura de Arquivo de Áudio

```
1. Usuário: File → Open Audio Files
2. MainWindow → AudioController::openAudioFiles()
3. AudioController → AudioDecoder::decode()
4. AudioDecoder → AudioFile (preenche metadados e amostras)
5. AudioController → Project::addAudioFile()
6. Project → emit audioFileAdded()
7. AudioListWidget → updateList()
8. MainWindow → AudioVisualizationWidget::setAudioFile()
```

### Criação de Anotação

```
1. Usuário: Annotation → Add Interval Tier
2. MainWindow → AnnotationController::addIntervalTier()
3. AnnotationController → Project::addTier()
4. Project → emit tierAdded()
5. MainWindow → AnnotationLayerWidget::setTier1()
6. Usuário: Double-click em AnnotationLayerWidget
7. AnnotationLayerWidget → startEditingInterval()
8. Usuário: Digita texto
9. AnnotationLayerWidget → AnnotationInterval::setText()
10. AnnotationInterval → emit textChanged()
11. Project → setModified(true)
```

### Cálculo de Pitch

```
1. Usuário: Analysis → Calculate Pitch
2. MainWindow → AudioController::calculatePitch()
3. AudioController → PitchDetector::detect()
4. PitchDetector → YAAPT algorithm
5. PitchDetector → AudioFile::setPitchData()
6. AudioFile → emit pitchDataCalculated()
7. AudioListWidget → updateAudioFileItem() (adiciona "Pitch data")
8. AudioVisualizationWidget → update() (desenha pitch na camada do espectrograma)
```

### Exportação para TextGrid

```
1. Usuário: File → Export TextGrid
2. MainWindow → ProjectController::exportTextGrid()
3. ProjectController → TextGridExporter::export()
4. TextGridExporter → percorre Project::getTiers()
5. TextGridExporter → escreve arquivo .TextGrid
6. TextGridExporter → retorna sucesso/erro
7. MainWindow → updateStatusBar("TextGrid exported successfully")
```

## Dependências Externas

### Qt6 (Core, Widgets, Multimedia)
- **Versão mínima**: 6.2
- **Uso**:
  - Qt Core: Estruturas de dados, sinais/slots, objetos
  - Qt Widgets: Interface gráfica
  - Qt Multimedia: Decodificação e reprodução de áudio

### FFTW3 (Fastest Fourier Transform in the West)
- **Versão**: 3.3+
- **Uso**: Cálculo de FFT para espectrograma
- **Licença**: GPL v2+ (ou comercial)

### libsndfile
- **Versão**: 1.0+
- **Uso**: Decodificação de formatos de áudio (WAV, AIFF, FLAC, OGG)
- **Licença**: LGPL v2.1+

## Padrões de Código

### Nomenclatura

- **Classes**: PascalCase (ex: `AudioFile`, `MainWindow`)
- **Métodos**: camelCase (ex: `getFileName()`, `setAudioFile()`)
- **Variáveis membro**: m_camelCase (ex: `m_audioFile`, `m_sampleRate`)
- **Constantes**: UPPER_SNAKE_CASE (ex: `MAX_CHANNELS`)
- **Arquivos**: PascalCase.h / PascalCase.cpp

### Organização de Código

- **Headers (.h)**: Declarações de classes, métodos públicos, sinais
- **Implementation (.cpp)**: Implementações de métodos
- **Um arquivo por classe** (exceto classes pequenas relacionadas)
- **Include guards**: `#ifndef CLASSNAME_H` / `#define CLASSNAME_H` / `#endif`

### Sinais e Slots (Qt)

- **Sinais**: Comunicação assíncrona entre objetos
- **Slots**: Métodos que respondem a sinais
- **Uso**: Desacoplamento entre componentes

### Memória

- **Smart pointers**: `std::shared_ptr` para objetos compartilhados
- **Qt parent-child**: Gerenciamento automático de memória para QObjects
- **RAII**: Resource Acquisition Is Initialization

## Extensibilidade

### Adicionar Novo Formato de Áudio

1. Implementar decodificador em `AudioDecoder`
2. Adicionar detecção de formato por extensão
3. Integrar com biblioteca de codec apropriada

### Adicionar Novo Algoritmo de Pitch

1. Criar nova classe derivada de `PitchDetector`
2. Implementar método `detect()`
3. Adicionar opção no menu Analysis
4. Permitir seleção de algoritmo nas configurações

### Adicionar Nova Visualização

1. Criar nova classe derivada de `QWidget`
2. Implementar `paintEvent()`
3. Integrar com `AudioVisualizationWidget` ou criar painel separado
4. Adicionar opção no menu View

## Testes

### Testes Unitários (Futuro)

- Framework: Google Test ou Qt Test
- Cobertura: Modelos, controladores, algoritmos de processamento
- Localização: `tests/` directory

### Testes de Integração (Futuro)

- Testes de fluxo completo
- Testes de importação/exportação TextGrid
- Testes de compatibilidade com Praat

## Compilação e Deploy

### Linux

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
./bin/AudioAnnotator
```

### Windows

```bash
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
.\bin\Release\AudioAnnotator.exe
```

### macOS

```bash
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/opt/homebrew/opt/qt6
make -j$(sysctl -n hw.ncpu)
./bin/AudioAnnotator
```

## Roadmap

### Versão 1.0 (Atual)
- ✅ Estrutura básica do projeto
- ✅ Interface gráfica com layout de painéis
- ✅ Modelos de dados (AudioFile, AnnotationTier, Project)
- ⏳ Visualização de waveform (stub)
- ⏳ Visualização de espectrograma (stub)
- ⏳ Sistema de anotações interativo (stub)
- ⏳ Controles de reprodução (stub)
- ⏳ Exportação/importação TextGrid (stub)
- ⏳ Cálculo de pitch e intensidade (stub)

### Versão 1.1 (Futuro)
- Implementação completa de visualização de waveform
- Implementação completa de espectrograma com FFTW3
- Decodificação de áudio com Qt Multimedia e libsndfile
- Reprodução de áudio com Qt Multimedia

### Versão 1.2 (Futuro)
- Sistema de anotações totalmente interativo
- Exportação/importação TextGrid funcional
- Undo/Redo para operações de anotação

### Versão 1.3 (Futuro)
- Implementação de algoritmo de pitch (YAAPT ou alternativa)
- Cálculo de intensidade
- Visualização de pitch e intensidade no espectrograma

### Versão 2.0 (Futuro)
- Suporte a plugins
- Análise acústica avançada (formantes, etc.)
- Integração com Python para scripts personalizados
- Suporte a múltiplos idiomas (i18n)

## Contribuindo

Para contribuir com o projeto:

1. Siga os padrões de código estabelecidos
2. Documente classes e métodos públicos
3. Teste suas alterações
4. Crie pull requests com descrição clara

## Licença

A ser definida.

## Contato

Para dúvidas sobre a arquitetura, consulte a documentação ou entre em contato através do repositório do projeto.

