# Salvamento e Carregamento de Projeto - BionoteEchos

## Implementação Completa ✅

### Formato do Arquivo

**Extensão**: `.becho` (BionoteEchos Project)  
**Formato**: JSON (legível e editável)

### Estrutura do JSON

```json
{
  "version": "1.0",
  "projectName": "Meu Projeto",
  "projectPath": "/caminho/para/projeto.becho",
  "audioFiles": [
    {
      "filePath": "/caminho/para/audio.wav",
      "sampleRate": 44100,
      "numChannels": 2,
      "duration": 10.5,
      "codec": "WAV"
    }
  ],
  "tiers": [
    {
      "name": "Palavras",
      "type": "interval",
      "intervals": [
        {
          "startTime": 0.0,
          "endTime": 1.5,
          "label": "olá"
        }
      ]
    },
    {
      "name": "Eventos",
      "type": "point",
      "points": [
        {
          "time": 2.5,
          "label": "clique"
        }
      ]
    }
  ]
}
```

## Funcionalidades Implementadas

### 1. Salvamento de Projeto

**Método**: `Project::saveProject(QString filePath)`

**Funcionalidades**:
- ✅ Salva em formato JSON
- ✅ Preserva caminhos dos arquivos de áudio
- ✅ Salva todas as camadas de anotação
- ✅ Salva intervalos e pontos
- ✅ Marca projeto como não modificado após salvar
- ✅ Atualiza caminho do projeto

**Uso**:
```cpp
// Salvar no caminho atual
project->saveProject();

// Salvar em novo caminho
project->saveProject("/caminho/novo.becho");
```

### 2. Carregamento de Projeto

**Método**: `Project::loadProject(QString filePath)`

**Funcionalidades**:
- ✅ Carrega de formato JSON
- ✅ Valida versão do arquivo
- ✅ Resolve caminhos relativos de áudio
- ✅ Restaura camadas de anotação
- ✅ Restaura intervalos e pontos
- ✅ Limpa projeto anterior antes de carregar
- ✅ Marca projeto como não modificado

**Uso**:
```cpp
if (project->loadProject("/caminho/projeto.becho")) {
    // Projeto carregado com sucesso
} else {
    // Erro ao carregar
}
```

### 3. Exportação/Importação JSON

**Métodos**:
- `QString Project::toJson()` - Exporta para JSON
- `bool Project::fromJson(QString json)` - Importa de JSON

**Uso**:
```cpp
// Exportar
QString jsonData = project->toJson();

// Importar
if (project->fromJson(jsonData)) {
    // Importado com sucesso
}
```

## Integração com MainWindow

### Ações do Menu

#### Novo Projeto
- **Atalho**: `Ctrl+N`
- **Ação**: Limpa projeto atual (com confirmação se modificado)
- **Método**: `MainWindow::onNewProject()`

#### Abrir Projeto
- **Atalho**: `Ctrl+O`
- **Ação**: Abre diálogo para selecionar arquivo `.becho`
- **Método**: `MainWindow::onOpenProject()`
- **Filtro**: `"Projetos BionoteEchos (*.becho)"`

#### Salvar Projeto
- **Atalho**: `Ctrl+S`
- **Ação**: Salva projeto (ou "Salvar Como" se novo)
- **Método**: `MainWindow::onSaveProject()`

#### Salvar Projeto Como
- **Atalho**: `Ctrl+Shift+S`
- **Ação**: Abre diálogo para escolher novo nome
- **Método**: `MainWindow::onSaveProjectAs()`
- **Extensão**: Adiciona `.becho` automaticamente

### Confirmação de Salvamento

**Método**: `MainWindow::maybeSave()`

Pergunta ao usuário se deseja salvar antes de:
- Criar novo projeto
- Abrir outro projeto
- Fechar aplicação

## Dados Salvos

### Informações do Projeto
- ✅ Nome do projeto
- ✅ Caminho do arquivo
- ✅ Versão do formato

### Arquivos de Áudio
- ✅ Caminho do arquivo
- ✅ Taxa de amostragem
- ✅ Número de canais
- ✅ Duração
- ✅ **Codec** (formato do áudio)

### Camadas de Anotação
- ✅ Nome da camada
- ✅ Tipo (interval ou point)
- ✅ Intervalos (início, fim, texto)
- ✅ Pontos (tempo, texto)

## Caminhos Relativos

O sistema tenta resolver caminhos de arquivos de áudio:

1. **Caminho absoluto**: Usa diretamente
2. **Caminho relativo**: Resolve em relação ao arquivo `.becho`
3. **Arquivo não encontrado**: Pula (não carrega o áudio)

**Exemplo**:
```
/home/user/projetos/
├── meu_projeto.becho
└── audios/
    └── audio1.wav
```

No JSON:
```json
{
  "audioFiles": [
    {
      "filePath": "audios/audio1.wav"  // Caminho relativo
    }
  ]
}
```

## Limitações Atuais

### ⚠️ Carregamento de Áudio

**Problema**: Arquivos de áudio não são recarregados automaticamente

**Solução Temporária**: Apenas metadados são salvos

**TODO**: Implementar carregamento assíncrono de áudio ao abrir projeto

### ⚠️ Configurações do Espectrograma

**Problema**: Configurações do espectrograma não são salvas

**TODO**: Adicionar seção de configurações no JSON

### ⚠️ Estado da Visualização

**Problema**: Zoom, posição de scroll não são salvos

**TODO**: Salvar estado da interface

## Exemplo de Uso Completo

```cpp
// Criar novo projeto
auto project = std::make_shared<Project>();

// Adicionar arquivo de áudio
auto audioFile = std::make_shared<AudioFile>();
audioFile->loadFromFile("/path/to/audio.wav");
project->addAudioFile(audioFile);

// Adicionar camada de anotação
auto tier = std::make_shared<AnnotationTier>("Palavras", TierType::IntervalTier);
tier->addInterval(0.0, 1.5, "olá");
tier->addInterval(1.5, 3.0, "mundo");
project->addTier(tier);

// Salvar projeto
if (project->saveProject("/path/to/projeto.becho")) {
    qDebug() << "Projeto salvo com sucesso!";
}

// Carregar projeto
auto loadedProject = std::make_shared<Project>();
if (loadedProject->loadProject("/path/to/projeto.becho")) {
    qDebug() << "Projeto carregado!";
    qDebug() << "Camadas:" << loadedProject->getTierCount();
}
```

## Melhorias Futuras

### Alta Prioridade
1. **Carregamento assíncrono de áudio**
   - Carregar áudio em background
   - Mostrar progresso
   - Permitir cancelamento

2. **Configurações do espectrograma**
   - Salvar parâmetros de cálculo
   - Salvar parâmetros de visualização
   - Restaurar ao abrir projeto

3. **Estado da interface**
   - Posição de zoom
   - Scroll da visualização
   - Camadas visíveis/ocultas

### Média Prioridade
1. **Backup automático**
   - Salvar cópia de segurança
   - Recuperar após crash

2. **Histórico de versões**
   - Manter versões anteriores
   - Permitir reverter mudanças

3. **Compactação**
   - Opção de salvar compactado
   - Reduzir tamanho do arquivo

### Baixa Prioridade
1. **Exportação para outros formatos**
   - TextGrid do Praat
   - ELAN (.eaf)
   - Audacity labels

2. **Metadados adicionais**
   - Autor do projeto
   - Data de criação
   - Notas do projeto

## Testes Recomendados

### Teste 1: Salvar e Carregar
1. Criar projeto
2. Adicionar áudio
3. Criar anotações
4. Salvar projeto
5. Fechar aplicação
6. Abrir aplicação
7. Carregar projeto
8. **Verificar**: Anotações preservadas

### Teste 2: Caminhos Relativos
1. Criar projeto em pasta A
2. Adicionar áudio da pasta B
3. Salvar projeto
4. Mover projeto e áudio para pasta C (mantendo estrutura)
5. Carregar projeto
6. **Verificar**: Áudio encontrado

### Teste 3: Múltiplas Camadas
1. Criar 5 camadas (3 interval, 2 point)
2. Adicionar 100 intervalos/pontos
3. Salvar projeto
4. Carregar projeto
5. **Verificar**: Todos os dados preservados

## Arquivos Modificados

1. ✅ `include/models/Project.h` - Declaração dos métodos
2. ✅ `src/models/Project.cpp` - Implementação completa
3. ✅ `src/views/MainWindow.cpp` - Integração com UI
4. ✅ `include/views/AudioMetadataDialog.h` - Diálogo de metadados
5. ✅ `src/views/AudioMetadataDialog.cpp` - Implementação
6. ✅ `src/views/AudioListWidget.cpp` - Menu de contexto

## Conclusão

O sistema de salvamento e carregamento de projeto está **completamente funcional** e permite:

- ✅ Salvar projetos em formato JSON legível
- ✅ Carregar projetos salvos
- ✅ Preservar anotações (intervalos e pontos)
- ✅ Salvar metadados dos arquivos de áudio
- ✅ Integração completa com a interface

**Status**: ✅ **IMPLEMENTADO E FUNCIONAL**

Próximos passos: Implementar carregamento assíncrono de áudio e salvar configurações do espectrograma.
