# Guia de Teste - Abertura de Arquivos de Áudio

## Como Testar a Funcionalidade

### 1. Compilar e Executar

```bash
# Compilar em modo debug
./scripts/build.sh -d

# Executar
./build-debug/bin/AudioAnnotator

# Ou compilar e executar diretamente
./scripts/build.sh -d -R
```

### 2. Abrir Arquivos de Áudio

#### Método 1: Menu
1. Clique em **Arquivo** → **Abrir Arquivos de Áudio...**
2. Navegue até a pasta com arquivos de áudio
3. Selecione um ou mais arquivos
4. Clique em "Abrir"

#### Método 2: Atalho de Teclado
1. Pressione `Ctrl+O`
2. Selecione arquivos
3. Clique em "Abrir"

#### Método 3: Toolbar
1. Clique no ícone de "Abrir Arquivos de Áudio" na barra de ferramentas
2. Selecione arquivos

### 3. O que Esperar

#### Sucesso
- ✅ Mensagem na barra de status: "Arquivo carregado: nome_do_arquivo.wav"
- ✅ Diálogo de confirmação: "X arquivo(s) carregado(s) com sucesso!"
- ✅ Arquivo aparece na lista lateral (se o widget estiver implementado)
- ✅ Output no console com metadados do arquivo

#### Exemplo de Output no Console
```
Arquivo decodificado: "/path/to/audio.wav"
  Canais: 2
  Taxa de amostragem: 44100 Hz
  Amostras: 2205000
  Duração: 50 s
```

#### Erro
- ❌ Mensagem de erro detalhada explicando o problema
- ❌ Contador de arquivos com erro na mensagem de resumo

### 4. Casos de Teste

#### Teste 1: Arquivo WAV Simples
- **Arquivo**: arquivo_mono_44100.wav
- **Esperado**: Carregamento bem-sucedido
- **Verificar**: Canais=1, Taxa=44100Hz

#### Teste 2: Arquivo Stereo
- **Arquivo**: musica_stereo.flac
- **Esperado**: Carregamento bem-sucedido
- **Verificar**: Canais=2

#### Teste 3: Múltiplos Arquivos
- **Ação**: Selecionar 3-5 arquivos diferentes
- **Esperado**: Todos carregados, mensagem com contagem

#### Teste 4: Arquivo Duplicado
- **Ação**: Tentar abrir o mesmo arquivo duas vezes
- **Esperado**: Mensagem "O arquivo já está aberto no projeto"

#### Teste 5: Arquivo Inválido
- **Arquivo**: arquivo_corrompido.wav ou texto.txt
- **Esperado**: Mensagem de erro clara

#### Teste 6: Arquivo Não Existente
- **Ação**: Tentar abrir arquivo que não existe
- **Esperado**: Erro "Arquivo não encontrado"

#### Teste 7: Formatos Diversos
Testar com:
- ✅ WAV (PCM)
- ✅ FLAC (compressão lossless)
- ✅ OGG/OPUS
- ✅ AIFF

### 5. Verificações Adicionais

#### Metadados
Os seguintes metadados devem ser preenchidos:
- Nome do arquivo
- Caminho completo
- Taxa de amostragem (Hz)
- Número de canais
- Número de amostras
- Duração (segundos)
- Codec
- Bit depth
- Tamanho do arquivo (bytes)

#### Memória
- Verificar que as amostras são carregadas corretamente
- Cada canal deve ter vetor de amostras separado
- Valores devem estar normalizados (-1.0 a 1.0)

### 6. Testes de Interface

#### Mensagens em Português
Verificar que todas as mensagens estão em português:
- ✅ "Abrir Arquivos de Áudio"
- ✅ "Carregando arquivos de áudio..."
- ✅ "Arquivo carregado: ..."
- ✅ "Erro ao Carregar Arquivo"
- ✅ "X arquivo(s) carregado(s) com sucesso"

#### Diálogo de Arquivo
- Deve mostrar filtros por tipo:
  - Arquivos de Áudio
  - WAV
  - FLAC
  - OGG
  - AIFF
  - Todos os Arquivos

### 7. Testes de Performance

#### Arquivo Pequeno (< 1 MB)
- Deve carregar instantaneamente

#### Arquivo Médio (10-50 MB)
- Deve carregar em 1-2 segundos
- Mensagens de progresso na barra de status

#### Arquivo Grande (> 100 MB)
- Pode levar alguns segundos
- Verificar uso de memória

### 8. Criando Arquivos de Teste

#### Com SoX (se disponível)
```bash
# Gerar tom de 440Hz, mono, 5 segundos
sox -n -r 44100 -b 16 test_440hz_mono.wav synth 5 sine 440

# Gerar tom, stereo, 10 segundos
sox -n -r 44100 -c 2 -b 16 test_stereo.wav synth 10 sine 440 sine 880

# Converter para FLAC
sox test.wav test.flac

# Converter para OGG
sox test.wav test.ogg
```

#### Com FFmpeg
```bash
# Gerar arquivo de teste
ffmpeg -f lavfi -i "sine=frequency=440:duration=5" -ar 44100 test.wav

# Converter para diferentes formatos
ffmpeg -i test.wav test.flac
ffmpeg -i test.wav -c:a libopus test.opus
```

### 9. Saída Esperada no Console

```
Tradução carregada: pt_BR
Arquivo decodificado: "/home/user/audio.wav"
  Canais: 2
  Taxa de amostragem: 44100 Hz
  Amostras: 220500
  Duração: 5 s
```

### 10. Solução de Problemas

#### Erro: "Erro ao abrir arquivo"
- Verificar se libsndfile está instalada
- Verificar se o arquivo existe e tem permissões de leitura
- Tentar formato mais simples (WAV PCM)

#### Erro: "Arquivo não encontrado"
- Verificar caminho completo
- Verificar permissões de diretório

#### Aplicação trava ao carregar
- Arquivo pode estar corrompido
- Tentar arquivo menor primeiro

---

## Checklist de Teste

- [ ] Abrir arquivo WAV mono
- [ ] Abrir arquivo stereo
- [ ] Abrir múltiplos arquivos
- [ ] Tentar arquivo duplicado
- [ ] Tentar arquivo inválido
- [ ] Verificar mensagens em português
- [ ] Verificar barra de status
- [ ] Verificar output no console
- [ ] Testar formato FLAC
- [ ] Testar formato OGG
- [ ] Fechar aplicação sem crash

---

**Última atualização**: 28 de Outubro de 2025
