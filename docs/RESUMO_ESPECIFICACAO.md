# Resumo Executivo - Especificação AudioAnnotator

## ✅ Status Atual: ~15% Completo

---

## 🎯 O Que Já Está Implementado

### 1. **Infraestrutura (100%)**
- ✅ Estrutura MVC profissional
- ✅ Sistema de build CMake
- ✅ Configuração VS Code
- ✅ Documentação completa

### 2. **Interface em Português (100%)**
- ✅ Menus traduzidos
- ✅ Ações traduzidas
- ✅ Mensagens em português
- ✅ Tooltips em português

### 3. **Abertura de Arquivos (100%)**
- ✅ Decodificador completo com libsndfile
- ✅ Suporte a 20+ formatos (WAV, FLAC, OGG, OPUS, AIFF, etc.)
- ✅ Leitura de metadados completos
- ✅ Verificação de duplicatas
- ✅ Feedback visual

### 4. **Modelos de Dados (100%)**
- ✅ AudioFile completo
- ✅ AnnotationTier (Interval e Point)
- ✅ Project
- ✅ Estrutura compatível com TextGrid

---

## 🚧 O Que Precisa Ser Implementado

### **PRIORIDADE ALTA - Próximas 2 Semanas**

#### 1. Visualização de Waveform (0%)
```
Funcionalidades necessárias:
- Desenhar forma de onda das amostras
- Downsampling para performance
- Zoom horizontal (scroll do mouse)
- Pan (arrastar para navegar)
- Seleção de trechos (clique e arrasto)
- Labels de tempo (início/fim do trecho visível)
- Cursor de reprodução
```

**Estimativa**: 3-5 dias

#### 2. Reprodução de Áudio (0%)
```
Funcionalidades necessárias:
- Integrar QMediaPlayer ou QAudioSink
- Botões Play, Pause, Stop
- Controle de volume
- Reprodução de trecho selecionado
- Modo loop
- Sincronização do cursor visual
```

**Estimativa**: 2-3 dias

#### 3. Sistema de Anotações Interativo (0%)
```
Funcionalidades necessárias:
- Desenhar camadas de anotação
- Adicionar intervalos (clique duplo)
- Mover fronteiras (arrastar bordas)
- Editar texto (duplo clique)
- Adicionar pontos (clique)
- Remover intervalos/pontos (Del)
- Visualizar até 2 camadas simultaneamente
```

**Estimativa**: 5-7 dias

---

### **PRIORIDADE MÉDIA - Semanas 3-4**

#### 4. Espectrograma Básico (0%)
```
Funcionalidades necessárias:
- Cálculo STFT com FFTW3
- Visualização sincronizada
- Comando mostrar/ocultar
- Configurações básicas (janela, passo)
- Uma paleta de cores inicial
```

**Estimativa**: 4-6 dias

#### 5. Export/Import TextGrid (0%)
```
Funcionalidades necessárias:
- Parser de formato TextGrid (long/short)
- Exportar anotações para .TextGrid
- Importar anotações de .TextGrid
- Validação de compatibilidade com Praat
```

**Estimativa**: 3-4 dias

#### 6. Lista de Arquivos e Objetos (30%)
```
Status: Widget criado mas sem funcionalidade
Necessário:
- Exibir arquivos de áudio carregados
- Estrutura hierárquica (arquivo + objetos)
- Seleção de arquivo ativo
- Exibir metadados do arquivo selecionado
```

**Estimativa**: 2-3 dias

---

### **PRIORIDADE BAIXA - Semanas 5-8**

#### 7. Análise Acústica (0%)
- Intensidade (RMS)
- Pitch/F0 (YAAPT - complexo)
- Visualização sobreposta
- Pré-cálculo e cache

**Estimativa**: 7-10 dias (YAAPT é complexo)

#### 8. Gestão de Projeto Completa (0%)
- Salvar projeto (.aap)
- Carregar projeto
- Formato de arquivo (JSON/XML)

**Estimativa**: 3-4 dias

#### 9. Configurações Avançadas (0%)
- Diálogo de configurações do espectrograma
- Múltiplas paletas de cores
- Configurações de pitch
- Preferências do usuário

**Estimativa**: 3-5 dias

---

## 📊 Checklist Visual

### Interface e Layout
```
[████████░░░░░░░░░░░░] 40% - Layout básico pronto
├─ [█████████████████████] 100% Menus e toolbars
├─ [█████████████████████] 100% Tradução PT-BR
├─ [███████░░░░░░░░░░░░░░] 30% Painel esquerdo
└─ [░░░░░░░░░░░░░░░░░░░░░] 0% Painel central (visualização)
```

### Funcionalidades Core
```
[████░░░░░░░░░░░░░░░░] 20% Funcionalidades principais
├─ [█████████████████████] 100% Abrir arquivos
├─ [░░░░░░░░░░░░░░░░░░░░░] 0% Visualizar waveform
├─ [░░░░░░░░░░░░░░░░░░░░░] 0% Reproduzir áudio
├─ [░░░░░░░░░░░░░░░░░░░░░] 0% Anotar (interativo)
└─ [░░░░░░░░░░░░░░░░░░░░░] 0% Export/Import TextGrid
```

### Recursos Avançados
```
[░░░░░░░░░░░░░░░░░░░░] 0% Recursos avançados
├─ [░░░░░░░░░░░░░░░░░░░░░] 0% Espectrograma
├─ [░░░░░░░░░░░░░░░░░░░░░] 0% Pitch (YAAPT)
├─ [░░░░░░░░░░░░░░░░░░░░░] 0% Intensidade
└─ [░░░░░░░░░░░░░░░░░░░░░] 0% Gestão de projeto
```

---

## 🎯 Roadmap Sugerido

### **Sprint 1: Visualização Básica** (1-2 semanas)
**Objetivo**: Ver e navegar pelo áudio

1. Implementar desenho de waveform
2. Zoom e pan
3. Labels de tempo
4. Seleção de trechos

**Entrega**: Usuário pode abrir e visualizar áudio

---

### **Sprint 2: Reprodução** (1 semana)
**Objetivo**: Ouvir o áudio

1. Integrar QAudioSink/QMediaPlayer
2. Controles play/pause/stop
3. Reprodução de trecho selecionado
4. Loop

**Entrega**: Usuário pode reproduzir áudio

---

### **Sprint 3: Anotações Básicas** (1-2 semanas)
**Objetivo**: Criar anotações simples

1. Desenhar camadas
2. Adicionar intervalos
3. Editar texto
4. Mover fronteiras

**Entrega**: Usuário pode anotar áudio

---

### **Sprint 4: TextGrid** (1 semana)
**Objetivo**: Compatibilidade com Praat

1. Export para .TextGrid
2. Import de .TextGrid
3. Testes de compatibilidade

**Entrega**: Usuário pode trocar dados com Praat

---

### **Sprint 5: Espectrograma** (1-2 semanas)
**Objetivo**: Visualização espectral

1. Cálculo STFT com FFTW3
2. Visualização básica
3. Configurações simples

**Entrega**: Usuário pode ver espectrograma

---

### **Sprint 6: Análise Acústica** (2-3 semanas)
**Objetivo**: Pitch e Intensidade

1. Intensidade (RMS)
2. Pitch (simplificado ou YAAPT)
3. Visualização sobreposta

**Entrega**: Usuário pode analisar propriedades acústicas

---

### **Sprint 7: Gestão de Projeto** (1 semana)
**Objetivo**: Salvar trabalho

1. Formato de projeto
2. Salvar/carregar

**Entrega**: Usuário pode salvar e retomar trabalho

---

### **Sprint 8: Polimento** (1-2 semanas)
**Objetivo**: Melhorias e refinamentos

1. Ícones
2. Atalhos
3. Performance
4. Testes

**Entrega**: Aplicação pronta para uso

---

## 📈 Estimativa Total

| Fase | Tempo | Acumulado |
|------|-------|-----------|
| Sprint 1 - Visualização | 2 semanas | 2 semanas |
| Sprint 2 - Reprodução | 1 semana | 3 semanas |
| Sprint 3 - Anotações | 2 semanas | 5 semanas |
| Sprint 4 - TextGrid | 1 semana | 6 semanas |
| Sprint 5 - Espectrograma | 2 semanas | 8 semanas |
| Sprint 6 - Análise | 3 semanas | 11 semanas |
| Sprint 7 - Projeto | 1 semana | 12 semanas |
| Sprint 8 - Polimento | 2 semanas | 14 semanas |

**Total estimado**: 14 semanas (~3,5 meses) de desenvolvimento

---

## 🚀 Próximo Passo Recomendado

### Implementar Visualização de Waveform (Sprint 1)

**Arquivos a modificar**:
- `src/views/AudioVisualizationWidget.cpp`
- `include/views/AudioVisualizationWidget.h`

**Funcionalidades**:
1. Desenhar amostras do áudio
2. Implementar downsampling para performance
3. Zoom com scroll do mouse
4. Pan com arrastar

**Complexidade**: Média
**Tempo estimado**: 3-5 dias
**Impacto**: Alto (fundamental para todas as outras funcionalidades)

---

## 💡 Notas Importantes

### Sobre YAAPT
O algoritmo YAAPT é complexo. Considere:
- **Opção 1**: Port da implementação Python
- **Opção 2**: Usar alternativa mais simples (autocorrelação, YIN)
- **Opção 3**: Deixar para última fase

### Sobre Espectrograma
Use FFTW3 conforme especificado, mas:
- Implemente cache para não recalcular
- Considere cálculo assíncrono
- Otimize para zoom

### Sobre TextGrid
Priorize compatibilidade total:
- Teste extensivamente com Praat
- Suporte formato longo e curto
- Valide encoding (UTF-8)

---

**Conclusão**: O projeto está bem estruturado e com fundação sólida. 
O próximo passo crítico é implementar a visualização de waveform,
que desbloqueia todas as outras funcionalidades visuais.

---

**Criado em**: 28 de Outubro de 2025
**Versão do Projeto**: 1.0.1-alpha
**Progresso**: 15% completo
