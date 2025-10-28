# Problema Crítico de Reprodução

## Sintomas
1. **Primeira reprodução**: Toca mas pula o início (2-4 segundos)
2. **Reproduções seguintes**: Apenas move cursor, sem áudio
3. **Loop**: Não funciona

## Evidências dos Logs

### Padrão Problemático
```
play() - Comando enviado. Aguardando estado PLAYING...
Estado mudou para: PLAYING (Loop: SIM, Região: SIM)
[IMEDIATAMENTE DEPOIS]
stop() - Estado antes: 1
Estado mudou para: STOPPED
```

### Análise
- `play()` é chamado corretamente
- Player muda para PLAYING
- **Algo** chama `stop()` logo depois
- Player volta para STOPPED
- Áudio não toca ou toca muito pouco

## Possíveis Causas

### 1. Usuário Clicando em Stop
**Improvável** - Seria muito rápido (< 1 segundo)

### 2. EndOfMedia Sendo Disparado Incorretamente
```
MediaStatus mudou para: 6  (EndOfMedia)
```
Isso pode estar disparando stop() automaticamente.

### 3. Problema com setPosition()
Ao chamar `setPosition()` logo antes de `play()`, pode estar confundindo o player.

### 4. QMediaPlayer no Linux com GStreamer
Backend pode ter bugs com:
- Reposicionamento rápido
- Loop de regiões pequenas
- Buffer não carregado

## Testes Necessários

### Teste 1: Play Simples (SEM região, SEM loop)
1. Abrir arquivo
2. Clicar Play
3. **NÃO tocar em nada**
4. Verificar se toca até o fim

**Esperado**: Deve tocar completamente
**Se falhar**: Problema fundamental do QMediaPlayer

### Teste 2: Verificar Quem Chama stop()
Adicionar logs mostrando de onde vem a chamada.

### Teste 3: Desabilitar setPosition() Antes de play()
Comentar linha que posiciona no início da região.

## Solução Proposta

### Opção A: Não Usar setPosition() com play()
```cpp
// NÃO fazer:
m_player->setPosition(m_regionStartMs);
m_player->play();

// Fazer:
m_player->play();
// Esperar MediaStatus == BufferedMedia
// DEPOIS chamar setPosition()
```

### Opção B: Usar QTimer para Delay
```cpp
m_player->play();
QTimer::singleShot(100, [this]() {
    if (m_hasPlaybackRegion) {
        m_player->setPosition(m_regionStartMs);
    }
});
```

### Opção C: Abandonar QMediaPlayer
Usar **libsndfile** + **PortAudio** para controle total.

## Próximos Passos
1. ✅ Adicionar mais logs em stop()
2. ⏳ Testar play() sem setPosition()
3. ⏳ Verificar se EndOfMedia está sendo disparado incorretamente
4. ⏳ Considerar implementação customizada
