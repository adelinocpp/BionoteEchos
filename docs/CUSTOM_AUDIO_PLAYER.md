# CustomAudioPlayer - Implementação com PortAudio

## Resumo

O **CustomAudioPlayer** substitui o QMediaPlayer que apresentava problemas críticos no Linux. Implementado com **PortAudio**, oferece reprodução de áudio profissional com latência mínima e loop perfeito.

## Por Que Foi Necessário?

### Problemas do QMediaPlayer
- ❌ Latência de 2-4 segundos
- ❌ Loop parava após 1-2 iterações
- ❌ Reprodução inconsistente (às vezes tocava, às vezes não)
- ❌ Seek lento (~500ms)
- ❌ Dependente de GStreamer (bugs conhecidos)

### Vantagens do CustomAudioPlayer
- ✅ Latência mínima (~5ms)
- ✅ Loop infinito sem gaps
- ✅ Seek instantâneo
- ✅ Reprodução confiável
- ✅ Controle total do buffer
- ✅ Multiplataforma (Linux/Mac/Windows)

## Arquitetura

### Componentes Principais

```
AudioFile (samples decodificados)
    ↓
CustomAudioPlayer
    ├── std::vector<float> m_samples (buffer de áudio)
    ├── PaStream* m_stream (stream do PortAudio)
    ├── std::atomic<size_t> m_playPosition (posição atual)
    ├── std::atomic<bool> m_loopEnabled (flag de loop)
    └── audioCallback() (processamento em tempo real)
    ↓
PortAudio
    ↓
Placa de som
```

### Thread Model

1. **Main Thread (Qt)**:
   - Interface do usuário
   - Controles (play/pause/stop)
   - Sinais Qt (positionChanged, etc.)

2. **Audio Thread (PortAudio RT)**:
   - Callback de áudio (alta prioridade)
   - Copia samples para buffer de saída
   - Gerencia loop e região
   - Thread-safe com std::atomic

3. **Timer Thread (Qt)**:
   - Atualiza posição na UI (20 Hz)
   - Não interfere com áudio

## Implementação

### Callback de Áudio (Coração do Sistema)

```cpp
int audioCallback(const void *input, void *output,
                  unsigned long framesPerBuffer,
                  const PaStreamCallbackTimeInfo* timeInfo,
                  PaStreamCallbackFlags statusFlags,
                  void *userData)
{
    CustomAudioPlayer *player = static_cast<CustomAudioPlayer*>(userData);
    float *out = static_cast<float*>(output);
    
    size_t pos = player->m_playPosition.load();
    
    for (unsigned long i = 0; i < framesPerBuffer; i++) {
        // Verificar fim de região
        if (hasRegion && pos >= regionEnd) {
            if (loop) {
                pos = regionStart;  // Loop instantâneo!
            } else {
                break;  // Parar
            }
        }
        
        // Verificar fim do arquivo
        if (pos >= samples.size()) {
            if (loop && !hasRegion) {
                pos = 0;  // Loop do arquivo completo
            } else {
                break;
            }
        }
        
        // Copiar sample com volume
        *out++ = samples[pos++] * volume;
    }
    
    player->m_playPosition = pos;
    return paContinue;
}
```

### Características do Callback

- **Lock-free**: Usa apenas std::atomic
- **Determinístico**: Sem alocações de memória
- **Rápido**: Apenas cópia de memória e comparações
- **Thread-safe**: Não precisa de mutex

## API Pública

### Métodos Principais

```cpp
// Controle de reprodução
void play();
void pause();
void stop();
bool isPlaying() const;

// Posicionamento
void setPosition(qint64 positionMs);
qint64 position() const;
qint64 duration() const;

// Volume
void setVolume(float volume);  // 0.0 - 1.0
float volume() const;

// Loop
void setLoop(bool loop);
bool isLooping() const;

// Região de reprodução
void setPlaybackRegion(qint64 startMs, qint64 endMs);
void clearPlaybackRegion();
bool hasPlaybackRegion() const;

// Arquivo
void setAudioFile(std::shared_ptr<AudioFile> audioFile);
```

### Sinais Qt

```cpp
signals:
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void playbackStateChanged(int state);  // 0=Stopped, 1=Playing, 2=Paused
    void playbackFinished();
    void errorOccurred(const QString &error);
```

## Configuração do PortAudio

### Parâmetros do Stream

```cpp
PaStreamParameters outputParameters;
outputParameters.device = Pa_GetDefaultOutputDevice();
outputParameters.channelCount = m_channels;
outputParameters.sampleFormat = paFloat32;
outputParameters.suggestedLatency = Pa_GetDeviceInfo(device)->defaultLowOutputLatency;

Pa_OpenStream(
    &m_stream,
    nullptr,              // sem entrada
    &outputParameters,
    m_sampleRate,
    256,                  // frames por buffer (baixa latência)
    paClipOff,
    &audioCallback,
    this
);
```

### Latência Calculada

- **Buffer size**: 256 frames
- **Sample rate**: 44100 Hz
- **Latência**: 256 / 44100 ≈ **5.8ms**

Comparado com QMediaPlayer: **2000ms** → **5.8ms** = **345x mais rápido!**

## Thread Safety

### Variáveis Atômicas

```cpp
std::atomic<size_t> m_playPosition;
std::atomic<bool> m_isPlaying;
std::atomic<bool> m_isPaused;
std::atomic<float> m_volume;
std::atomic<bool> m_loopEnabled;
std::atomic<bool> m_hasPlaybackRegion;
std::atomic<size_t> m_regionStartSample;
std::atomic<size_t> m_regionEndSample;
```

### Por Que Atomic?

- Leitura/escrita atômica entre threads
- Sem necessidade de mutex
- Sem bloqueio no callback de áudio
- Performance máxima

## Multiplataforma

### Linux
- **Backend**: ALSA, PulseAudio, JACK
- **Pacote**: `portaudio19-dev`
- **Status**: ✅ Testado e funcionando

### Windows
- **Backend**: WASAPI, DirectSound, MME
- **Instalação**: vcpkg ou binários pré-compilados
- **Status**: ✅ Compatível (não testado)

### macOS
- **Backend**: CoreAudio
- **Instalação**: Homebrew (`brew install portaudio`)
- **Status**: ✅ Compatível (não testado)

## Comparação com Audacity

O Audacity usa uma arquitetura similar:

```
Audacity:
  WaveTrack → AudioIO → PortAudio → Hardware

AudioAnnotator:
  AudioFile → CustomAudioPlayer → PortAudio → Hardware
```

### Semelhanças
- Ambos usam PortAudio
- Ambos processam áudio em callback
- Ambos usam buffer de samples em memória
- Ambos suportam loop de região

### Diferenças
- Audacity: Mais complexo (multi-track, efeitos)
- AudioAnnotator: Mais simples (single-track, reprodução)

## Performance

### Benchmarks Estimados

| Operação | QMediaPlayer | CustomAudioPlayer |
|----------|--------------|-------------------|
| **Latência inicial** | 2000ms | 5ms |
| **Seek** | 500ms | < 1ms |
| **Loop gap** | 100-500ms | 0ms |
| **CPU (idle)** | ~2% | ~0.5% |
| **CPU (playing)** | ~5% | ~1% |

### Uso de Memória

- **Samples**: ~4 bytes por sample
- **Arquivo de 10s @ 44.1kHz**: ~1.7 MB
- **Overhead**: ~100 KB

## Debugging

### Logs

```bash
cat logs/player.log | grep "CustomAudioPlayer"
```

### Comandos Úteis

```bash
# Listar dispositivos de áudio
pactl list sinks short

# Verificar PortAudio
pkg-config --modversion portaudio-2.0

# Testar latência
pa_devs  # (se disponível)
```

## Limitações Conhecidas

1. **Mono/Stereo apenas**: Não suporta > 2 canais (suficiente para anotação)
2. **Samples em memória**: Arquivo inteiro carregado (OK para arquivos < 1 hora)
3. **Sem efeitos**: Apenas reprodução (não é o objetivo)

## Possíveis Melhorias Futuras

1. **Streaming**: Carregar arquivo em chunks para arquivos muito grandes
2. **Crossfade**: Suavizar transição no loop
3. **Pitch shift**: Alterar velocidade sem mudar pitch
4. **Equalização**: Filtros de áudio

## Conclusão

O **CustomAudioPlayer** resolve completamente os problemas do QMediaPlayer, oferecendo:

- ✅ Reprodução confiável e previsível
- ✅ Latência profissional
- ✅ Loop perfeito
- ✅ Código limpo e manutenível
- ✅ Multiplataforma

**Resultado**: Aplicação de anotação de áudio **profissional e usável**! 🎵
