# Diagnóstico de Problemas de Áudio

## Status dos Logs ✅
- **Localização**: `build-debug/logs/` (link simbólico criado em `logs/`)
- **Arquivos criados**: player.log, audio.log, app.log
- **Sistema funcionando**: printf/fprintf gravando corretamente

## Problemas Identificados

### 1. Loop Não Executa Automaticamente ❌
**Evidência nos logs**:
```
[15:50:21] play() - Loop: 1, Região: 2898ms - 4111ms
[15:50:21] Posição: 3005ms
[15:50:21] Posição: 3117ms
[15:50:23] pause() - Estado antes: 1  ← Usuário pausou manualmente
```

**Problema**: O áudio não está tocando até o fim da região (4111ms) para detectar o loop.
Ele para em 3117ms, muito antes do fim.

### 2. Reprodução Interrompida
O áudio toca apenas ~200ms e para. Possíveis causas:
- Buffer do QMediaPlayer no Linux
- Problema com QAudioOutput
- Taxa de atualização do positionChanged muito alta

### 3. Logs Mostram Comportamento Correto da Lógica
- Detecção de fim de região: ✅ Funciona (quando chega lá)
- Proteção contra loop infinito: ✅ Funciona
- Reposicionamento: ✅ Funciona

## Solução Proposta

### Opção 1: Usar QMediaPlayer::setNotifyInterval()
Reduzir frequência de positionChanged para evitar overhead.

### Opção 2: Verificar Backend de Áudio
QMediaPlayer no Linux pode ter problemas com alguns backends.

### Opção 3: Implementar Timer de Verificação
Usar QTimer separado para verificar posição ao invés de onPositionChanged.

## Comandos para Debug

```bash
# Ver logs completos
cat build-debug/logs/player.log

# Ver apenas loops
cat build-debug/logs/player.log | grep -i "loop\|região"

# Ver posições
cat build-debug/logs/player.log | grep "Posição:"

# Monitorar em tempo real
tail -f build-debug/logs/player.log
```

## Próximos Passos
1. Reduzir logs de posição (muito verboso)
2. Adicionar setNotifyInterval(100) no QMediaPlayer
3. Testar com arquivo de áudio diferente
4. Verificar se é problema específico do Linux/PulseAudio
