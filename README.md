# AudioAnnotator

**AudioAnnotator** é uma aplicação multiplataforma desenvolvida em C++ e Qt6 para anotação de arquivos de áudio com visualização de forma de onda, espectrograma e camadas de anotação compatíveis com o formato TextGrid do Praat.

## Características Principais

### Gerenciamento de Arquivos de Áudio
- Suporte a múltiplos formatos de áudio: **Opus**, **WAV**, **MP3**, **OGG**, **FLAC**, entre outros
- Abertura simultânea de múltiplos arquivos de áudio
- Visualização de metadados: frequência de amostragem, codec, duração, número de amostras
- Lista lateral para organização dos arquivos e objetos associados

### Visualização de Áudio
- **Forma de onda** (waveform) interativa com zoom e navegação temporal
- **Espectrograma** sincronizado com a forma de onda
- Controle de nível de zoom para exibição do espectrograma
- Labels indicando início e fim do trecho visualizado
- Painel central dedicado para visualização e anotação

### Espectrograma Configurável
- Opções de cálculo:
  - Passo de tempo (time step)
  - Janela de tempo (time window)
  - Janela espectral (window type)
  - Número de pontos FFT com zero padding
- Opções de visualização:
  - Faixa dinâmica (dynamic range)
  - Paleta de cores personalizável
- Comando para adicionar/remover visualização do espectrograma

### Sistema de Anotações
- Camadas de anotação estilo **TextGrid do Praat**
- Suporte a **IntervalTier** (camadas de intervalos) e **TextTier** (camadas de pontos)
- Adição ilimitada de camadas de anotação
- Visualização e edição simultânea de até duas camadas
- Interatividade completa:
  - Adicionar e mover fronteiras de intervalos
  - Editar texto das anotações
  - Criar e editar pontos de anotação
- Seleção de trecho correspondente a intervalo de anotação

### Análise Acústica Avançada
- Cálculo e visualização de **frequência fundamental (F0/pitch)** usando algoritmo YAAPT
- Cálculo e visualização de **intensidade**
- Pré-cálculo e armazenamento de F0 e intensidade como objetos associados ao áudio
- Visualização integrada na camada do espectrograma

### Controles de Reprodução
- Controles básicos: **Play**, **Pause**, **Stop**
- Reprodução em **loop** de trecho selecionado
- Sincronização visual durante reprodução
- Seleção de trecho para reprodução

### Gestão de Projeto
- Salvar projeto completo (áudios, anotações, configurações)
- Carregar projetos salvos
- Limpar dados do projeto atual
- Formato de projeto proprietário para preservar todos os dados

### Exportação e Compatibilidade
- Exportação de anotações para formato **TextGrid** compatível com Praat
- Suporte aos formatos de texto longo e curto do TextGrid
- Preservação de estrutura de camadas e anotações

## Requisitos do Sistema

### Dependências
- **Qt6** (Core, Widgets, Multimedia)
- **FFTW3** (para cálculo de espectrograma)
- **libsndfile** (para suporte a múltiplos formatos de áudio)
- **CMake** 3.16 ou superior
- Compilador C++17 ou superior (GCC, Clang, MSVC)

### Sistemas Operacionais Suportados
- Linux (Ubuntu, Debian, Fedora, etc.)
- Windows (7, 10, 11)
- macOS (10.14+)

## Compilação

### Linux (Ubuntu/Debian)

```bash
# Instalar dependências
sudo apt-get update
sudo apt-get install -y qt6-base-dev qt6-multimedia-dev libfftw3-dev libsndfile1-dev cmake build-essential

# Clonar e compilar
cd AudioAnnotator
mkdir build && cd build
cmake ..
make -j$(nproc)

# Executar
./bin/AudioAnnotator
```

### Windows

```bash
# Instalar Qt6, FFTW3 e libsndfile via vcpkg ou manualmente
# Configurar CMake com Qt6_DIR apontando para instalação do Qt

mkdir build && cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release

# Executar
.\bin\Release\AudioAnnotator.exe
```

### macOS

```bash
# Instalar dependências via Homebrew
brew install qt6 fftw libsndfile cmake

# Compilar
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/opt/homebrew/opt/qt6
make -j$(sysctl -n hw.ncpu)

# Executar
./bin/AudioAnnotator
```

## Estrutura do Projeto

```
AudioAnnotator/
├── CMakeLists.txt          # Configuração do CMake
├── README.md               # Este arquivo
├── include/                # Arquivos de cabeçalho (.h)
│   ├── models/            # Modelos de dados
│   ├── views/             # Interface gráfica
│   ├── controllers/       # Controladores
│   ├── audio/             # Processamento de áudio
│   └── utils/             # Utilitários
├── src/                    # Arquivos de implementação (.cpp)
│   ├── main.cpp           # Ponto de entrada
│   ├── models/
│   ├── views/
│   ├── controllers/
│   ├── audio/
│   └── utils/
├── resources/              # Recursos (ícones, imagens, etc.)
├── docs/                   # Documentação adicional
└── build/                  # Diretório de compilação (gerado)
```

## Arquitetura do Software

O projeto segue o padrão **Model-View-Controller (MVC)**:

### Models (Modelos)
- **AudioFile**: Representa um arquivo de áudio com seus metadados
- **AnnotationTier**: Representa uma camada de anotação (Interval ou Point)
- **AnnotationInterval**: Representa um intervalo de anotação
- **AnnotationPoint**: Representa um ponto de anotação
- **Project**: Representa o projeto completo

### Views (Visualizações)
- **MainWindow**: Janela principal da aplicação
- **AudioListWidget**: Lista de arquivos de áudio e objetos
- **AudioVisualizationWidget**: Visualização de forma de onda e espectrograma
- **AnnotationLayerWidget**: Visualização e edição de camadas de anotação
- **SpectrogramWidget**: Renderização do espectrograma
- **AudioControlWidget**: Controles de reprodução

### Controllers (Controladores)
- **ProjectController**: Gerencia operações de projeto (salvar, carregar, limpar)
- **AudioController**: Gerencia operações de áudio (abrir, fechar, reproduzir)
- **AnnotationController**: Gerencia operações de anotação (adicionar, editar, remover)

### Audio (Processamento de Áudio)
- **AudioDecoder**: Decodifica arquivos de áudio
- **AudioPlayer**: Reproduz áudio
- **SpectrogramCalculator**: Calcula espectrograma usando FFTW3
- **PitchDetector**: Detecta frequência fundamental (F0) usando YAAPT
- **IntensityCalculator**: Calcula intensidade do sinal

### Utils (Utilitários)
- **TextGridExporter**: Exporta anotações para formato TextGrid
- **TextGridImporter**: Importa anotações de formato TextGrid

## Uso

### Abrir Arquivos de Áudio
1. Menu **File** → **Open Audio Files**
2. Selecione um ou mais arquivos de áudio
3. Os arquivos aparecerão na lista lateral

### Visualizar Áudio
1. Clique em um arquivo na lista lateral
2. A forma de onda aparecerá no painel central
3. Use a roda do mouse para zoom horizontal
4. Arraste para navegar pela linha do tempo

### Adicionar Espectrograma
1. Menu **View** → **Show Spectrogram**
2. Configure opções em **View** → **Spectrogram Settings**

### Criar Camadas de Anotação
1. Menu **Annotation** → **Add Tier**
2. Escolha o tipo: **Interval Tier** ou **Point Tier**
3. Nomeie a camada
4. A camada aparecerá abaixo da visualização de áudio

### Anotar Intervalos
1. Selecione uma camada de intervalos
2. Clique e arraste para criar um intervalo
3. Clique duas vezes no intervalo para editar o texto
4. Arraste as bordas para ajustar os limites

### Anotar Pontos
1. Selecione uma camada de pontos
2. Clique na posição desejada para criar um ponto
3. Digite o texto da anotação

### Reproduzir Áudio
1. Use os controles de reprodução na parte inferior
2. **Play**: Reproduz do início ou da posição atual
3. **Pause**: Pausa a reprodução
4. **Loop**: Reproduz trecho selecionado em loop

### Calcular Pitch (F0)
1. Menu **Analysis** → **Calculate Pitch**
2. Configure parâmetros do YAAPT
3. O pitch será calculado e visualizado

### Exportar Anotações
1. Menu **File** → **Export TextGrid**
2. Escolha o formato (longo ou curto)
3. Salve o arquivo .TextGrid

### Salvar Projeto
1. Menu **File** → **Save Project**
2. Escolha o local e nome do arquivo
3. Todas as anotações e configurações serão salvas

## Formato TextGrid

O AudioAnnotator é totalmente compatível com o formato TextGrid do Praat, permitindo:
- Importar anotações criadas no Praat
- Exportar anotações para uso no Praat
- Editar anotações existentes

### Exemplo de TextGrid Exportado

```
File type = "ooTextFile"
Object class = "TextGrid"
xmin = 0
xmax = 5.3
tiers? <exists>
size = 2
item []:
    item [1]:
        class = "IntervalTier"
        name = "words"
        xmin = 0
        xmax = 5.3
        intervals: size = 3
        intervals [1]:
            xmin = 0
            xmax = 1.2
            text = "hello"
        intervals [2]:
            xmin = 1.2
            xmax = 2.5
            text = "world"
        intervals [3]:
            xmin = 2.5
            xmax = 5.3
            text = ""
    item [2]:
        class = "TextTier"
        name = "events"
        xmin = 0
        xmax = 5.3
        points: size = 1
        points [1]:
            number = 1.8
            mark = "peak"
```

## Algoritmo YAAPT

O AudioAnnotator implementa o algoritmo **YAAPT (Yet Another Algorithm for Pitch Tracking)** para detecção robusta de frequência fundamental, oferecendo:
- Alta precisão para fala de alta qualidade e telefônica
- Decisões de voiced/unvoiced
- Parâmetros configuráveis para diferentes cenários

## Desenvolvimento

### Configuração do Ambiente VS Code

O projeto inclui configuração para desenvolvimento no Visual Studio Code:
- Arquivos `.vscode/` com configurações de build e debug
- Integração com CMake Tools
- IntelliSense configurado para Qt6

### Adicionar Novas Funcionalidades

1. Crie os arquivos de cabeçalho em `include/`
2. Crie os arquivos de implementação em `src/`
3. Adicione os arquivos ao `CMakeLists.txt`
4. Recompile o projeto

## Licença

Este projeto é distribuído sob licença a ser definida.

## Autores

Desenvolvido como parte de uma série de aplicações de análise científica em C++ e Qt6.

## Referências

- **Praat**: https://www.fon.hum.uva.nl/praat/
- **YAAPT**: http://ws2.binghamton.edu/zahorian/yaapt.htm
- **Qt6**: https://www.qt.io/
- **FFTW3**: https://www.fftw.org/

## Contato

Para dúvidas, sugestões ou contribuições, entre em contato através do repositório do projeto.

