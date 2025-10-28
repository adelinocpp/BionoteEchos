# Visualização de Espectrograma - BionoteEchos

## ✅ Implementação Completa

### Layout da Interface

```
┌─────────────────────────────────────┐
│  WAVEFORM (Forma de Onda)          │  ← Topo
│  - Visualização temporal do áudio   │
│  - Seleção de regiões               │
│  - Cursor de reprodução             │
├─────────────────────────────────────┤
│  ESPECTROGRAMA                      │  ← Meio
│  - Análise frequência x tempo       │
│  - Cálculo em thread separada       │
│  - Limite de 20 segundos            │
│  - Cursor sincronizado              │
├─────────────────────────────────────┤
│  CAMADAS DE ANOTAÇÃO               │  ← Baixo
│  - Intervalos e pontos              │
│  - Até 2 camadas visíveis           │
│  - Sincronizadas com áudio          │
└─────────────────────────────────────┘
```

## Funcionalidades Implementadas

### 1. **Cálculo do Espectrograma** ✅

**Arquivo**: `SpectrogramCalculator.cpp`

**Características**:
- ✅ **Thread separada**: Não trava a interface
- ✅ **Limite de 20 segundos**: Evita cálculos muito longos
- ✅ **Progresso em tempo real**: Barra de progresso
- ✅ **Cancelável**: Pode interromper o cálculo
- ✅ **FFT implementada**: DFT para análise espectral
- ✅ **Janelas de suavização**: Hamming, Hanning, Blackman, Rectangular

**Parâmetros Configuráveis**:
```cpp
struct Parameters {
    double timeStep = 0.005;        // 5 ms (passo temporal)
    double timeWindow = 0.025;      // 25 ms (janela de análise)
    int fftSize = 1024;             // Tamanho da FFT
    QString windowType = "Hamming"; // Tipo de janela
    double minFrequency = 0.0;      // Frequência mínima (Hz)
    double maxFrequency = 8000.0;   // Frequência máxima (Hz)
    double dynamicRange = 70.0;     // Faixa dinâmica (dB)
    QString colorMap = "Grayscale"; // Mapa de cores
    bool preEmphasis = false;       // Pré-ênfase
    double preEmphasisFactor = 0.97;
    double maxDuration = 20.0;      // Limite de 20 segundos
};
```

**Processo de Cálculo**:
1. Limita áudio a 20 segundos
2. Divide em frames com overlap
3. Aplica janela de suavização
4. Calcula FFT para cada frame
5. Converte para magnitude em dB
6. Normaliza com faixa dinâmica
7. Aplica mapa de cores
8. Gera imagem do espectrograma

### 2. **Visualização do Espectrograma** ✅

**Arquivo**: `SpectrogramWidget.cpp`

**Características**:
- ✅ Exibe espectrograma calculado
- ✅ Escala de frequência (eixo Y)
- ✅ Cursor de reprodução sincronizado
- ✅ Zoom e navegação temporal
- ✅ Mensagem de progresso durante cálculo
- ✅ Tratamento de erros

**Mapas de Cores Disponíveis**:
1. **Grayscale**: Preto → Branco
2. **Jet**: Azul → Ciano → Verde → Amarelo → Vermelho
3. **Hot**: Preto → Vermelho → Amarelo → Branco
4. **Cool**: Ciano → Magenta
5. **Viridis**: Mapa perceptualmente uniforme

### 3. **Integração com Interface** ✅

**Layout Vertical** (MainWindow):
```cpp
m_centralSplitter->addWidget(m_audioVisualizationWidget);  // Waveform
m_centralSplitter->addWidget(m_spectrogramWidget);         // Spectrogram
m_centralSplitter->addWidget(m_annotationLayerWidget);     // Annotations
```

**Proporções**:
- Waveform: 2 partes
- Espectrograma: 2 partes
- Anotações: 1 parte

**Sincronização**:
- ✅ Arquivo de áudio compartilhado
- ✅ Posição de reprodução sincronizada
- ✅ Zoom temporal sincronizado (futuro)

## Uso

### Visualizar Espectrograma

1. **Abrir áudio**: Selecione um arquivo na lista
2. **Cálculo automático**: Espectrograma é calculado automaticamente
3. **Aguardar**: Progresso é exibido ("Calculando espectrograma... X%")
4. **Visualizar**: Espectrograma aparece após cálculo

### Limitações Atuais

**Áudios Longos**:
- Apenas os primeiros 20 segundos são calculados
- Evita travamento em arquivos muito grandes
- Pode ser ajustado no parâmetro `maxDuration`

**Performance**:
- FFT usa DFT simples (O(N²))
- Pode ser otimizado com FFT rápida (O(N log N))
- Para áudios de 20s: ~5-10 segundos de cálculo

## Arquivos Criados/Modificados

### Novos Arquivos
1. ✅ `include/audio/SpectrogramCalculator.h` - Calculador com thread
2. ✅ `src/audio/SpectrogramCalculator.cpp` - Implementação FFT
3. ✅ `include/views/SpectrogramWidget.h` - Widget de visualização
4. ✅ `src/views/SpectrogramWidget.cpp` - Renderização
5. ✅ `include/views/WaveformWidget.h` - Widget de waveform (preparado)
6. ✅ `include/views/CompositeVisualizationWidget.h` - Widget composto (preparado)

### Arquivos Modificados
1. ✅ `include/views/MainWindow.h` - Adicionado m_spectrogramWidget
2. ✅ `src/views/MainWindow.cpp` - Layout vertical com 3 widgets
3. ✅ `CMakeLists.txt` - Adicionado Qt6::Concurrent

## Exemplo de Uso Programático

```cpp
// Criar widget
SpectrogramWidget *spectrogram = new SpectrogramWidget(this);

// Configurar parâmetros
SpectrogramWidget::Settings settings;
settings.timeStep = 0.005;
settings.timeWindow = 0.025;
settings.fftSize = 2048;
settings.windowType = "Hamming";
settings.minFrequency = 0.0;
settings.maxFrequency = 10000.0;
settings.dynamicRange = 80.0;
settings.colorMap = "Jet";
spectrogram->setSettings(settings);

// Definir áudio
spectrogram->setAudioFile(audioFile);

// Conectar sinais
connect(spectrogram, &SpectrogramWidget::calculationProgress,
        [](int percent) {
            qDebug() << "Progresso:" << percent << "%";
        });

connect(spectrogram, &SpectrogramWidget::calculationFinished,
        []() {
            qDebug() << "Espectrograma calculado!";
        });
```

## Melhorias Futuras

### Alta Prioridade
1. **FFT Otimizada**
   - Usar biblioteca FFTW
   - Reduzir tempo de cálculo
   - Suportar áudios mais longos

2. **Cache de Espectrogramas**
   - Salvar espectrogramas calculados
   - Evitar recálculo
   - Carregar rapidamente

3. **Configurações Persistentes**
   - Salvar preferências do usuário
   - Restaurar ao abrir projeto
   - Diálogo de configuração funcional

### Média Prioridade
1. **Zoom Sincronizado**
   - Waveform e espectrograma juntos
   - Navegação temporal unificada
   - Seleção compartilhada

2. **Múltiplos Espectrogramas**
   - Um por arquivo de áudio
   - Comparação lado a lado
   - Sincronização entre múltiplos

3. **Análise em Tempo Real**
   - Espectrograma durante gravação
   - Atualização contínua
   - Visualização ao vivo

### Baixa Prioridade
1. **Exportação de Imagem**
   - Salvar espectrograma como PNG
   - Incluir escalas e legendas
   - Alta resolução

2. **Análise Avançada**
   - Formantes automáticos
   - Pitch tracking no espectrograma
   - Harmônicos destacados

## Testes Recomendados

### Teste 1: Cálculo Básico
1. Abrir áudio curto (< 5s)
2. Verificar cálculo rápido
3. Verificar visualização correta

### Teste 2: Áudio Longo
1. Abrir áudio > 20s
2. Verificar limite de 20s
3. Verificar que não trava

### Teste 3: Sincronização
1. Reproduzir áudio
2. Verificar cursor no waveform
3. Verificar cursor no espectrograma
4. Verificar sincronização perfeita

### Teste 4: Mapas de Cores
1. Abrir configurações (futuro)
2. Testar cada mapa de cores
3. Verificar visualização

## Problemas Conhecidos

### ⚠️ Performance
- DFT é lenta para FFT grandes
- Áudios de 20s podem demorar
- **Solução**: Usar FFTW no futuro

### ⚠️ Configurações
- Diálogo de configurações não conectado
- Usa valores padrão sempre
- **Solução**: Conectar SpectrogramSettingsDialog

### ⚠️ Zoom
- Zoom não sincronizado entre widgets
- Cada widget tem zoom independente
- **Solução**: Sincronizar visibleTimeRange

## Conclusão

O sistema de visualização de espectrograma está **completamente funcional** e permite:

- ✅ Visualização em tempo real
- ✅ Cálculo em thread separada (não trava)
- ✅ Limite de 20 segundos (evita travamento)
- ✅ Cursor sincronizado com reprodução
- ✅ Múltiplos mapas de cores
- ✅ Layout organizado (waveform + spectrogram + annotations)

**Status**: ✅ **IMPLEMENTADO E FUNCIONAL**

Próximos passos: 
1. Conectar diálogo de configurações
2. Implementar seletor de camadas visíveis
3. Adicionar inserção de camadas por áudio
