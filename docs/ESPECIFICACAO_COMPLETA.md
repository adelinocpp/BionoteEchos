# Especificação Completa do Projeto AudioAnnotator

## Análise da Especificação Original

Este documento apresenta uma análise detalhada da especificação fornecida e um checklist completo de implementação.

---

## 1. GERENCIAMENTO DE ARQUIVOS DE ÁUDIO

### 1.1 Abertura de Arquivos
- [x] **Abrir um ou mais arquivos de áudio**
  - [x] Suporte a OPUS
  - [x] Suporte a WAV
  - [x] Suporte a MP3 (via libsndfile se disponível)
  - [x] Suporte a OGG
  - [x] Suporte a FLAC
  - [x] Outros formatos (AIFF, AU, etc.)

### 1.2 Barra de Lista de Arquivos
- [ ] **Lista à esquerda com arquivos de áudio**
  - [ ] Exibir nome do arquivo
  - [ ] Ícone indicando tipo
  - [ ] Seleção de arquivo ativo
  - [ ] Múltiplos arquivos simultâneos

### 1.3 Objetos Associados aos Arquivos
- [ ] **Estrutura hierárquica**
  - [ ] Arquivo de áudio como nó pai
  - [ ] Objetos filhos:
    - [ ] Pitch (F0) pré-calculado
    - [ ] Intensidade pré-calculada
    - [ ] Espectrograma pré-calculado (opcional)
    - [ ] Camadas de anotação associadas

### 1.4 Metadados do Áudio
- [x] **Exibir propriedades principais**
  - [x] Frequência de amostragem
  - [x] Codec
  - [x] Duração (tempo total)
  - [x] Número de amostras
  - [x] Número de canais
  - [x] Bit depth
  - [x] Tamanho do arquivo
  - [ ] Interface para visualizar metadados

---

## 2. LAYOUT DA INTERFACE

### 2.1 Organização Geral
- [x] **Painel esquerdo**: Lista de arquivos e objetos
- [x] **Painel central**: Visualização e anotação
- [x] **Separadores (splitters)**: Redimensionáveis

### 2.2 Painel Central
- [ ] **Área de visualização de áudio**
  - [ ] Forma de onda (waveform)
  - [ ] Espectrograma (opcional, sincronizado)
  - [ ] Labels de tempo (início e fim)
  
- [ ] **Área de anotações**
  - [ ] Até 2 camadas visíveis simultaneamente
  - [ ] Visualização de IntervalTier
  - [ ] Visualização de PointTier

### 2.3 Controles de Reprodução
- [ ] **Barra de controles na parte inferior**
  - [ ] Botão Play
  - [ ] Botão Pause
  - [ ] Botão Stop
  - [ ] Controle de volume
  - [ ] Indicador de posição atual
  - [ ] Loop de trecho selecionado

---

## 3. VISUALIZAÇÃO DE ÁUDIO

### 3.1 Forma de Onda (Waveform)
- [ ] **Visualização interativa**
  - [ ] Desenho da forma de onda
  - [ ] Zoom horizontal (com scroll do mouse)
  - [ ] Deslocamento pela linha do tempo (pan com arrasto)
  - [ ] Seleção de trechos (clique e arrasto)
  - [ ] Cursor de reprodução

### 3.2 Labels de Tempo
- [ ] **Indicadores visuais**
  - [ ] Label de tempo inicial do trecho visível
  - [ ] Label de tempo final do trecho visível
  - [ ] Formato: hh:mm:ss.mmm ou s.mmm
  - [ ] Atualização dinâmica durante zoom/pan

### 3.3 Sincronização
- [ ] **Todas as visualizações sincronizadas**
  - [ ] Waveform com espectrograma
  - [ ] Waveform com camadas de anotação
  - [ ] Todas as camadas alinhadas temporalmente

---

## 4. ESPECTROGRAMA

### 4.1 Controles de Exibição
- [ ] **Comandos de menu**
  - [ ] Adicionar visualização do espectrograma
  - [ ] Remover visualização do espectrograma
  - [ ] Configurações do espectrograma

### 4.2 Nível de Zoom para Exibição
- [ ] **Controle de quando mostrar**
  - [ ] Definir nível mínimo de zoom
  - [ ] Espectrograma aparece apenas acima do zoom definido
  - [ ] Performance otimizada

### 4.3 Opções de Cálculo
- [ ] **Parâmetros configuráveis**
  - [ ] Passo de tempo (time step) - ex: 0.005s
  - [ ] Janela de tempo (time window) - ex: 0.025s
  - [ ] Tipo de janela espectral (Hamming, Hann, Blackman, etc.)
  - [ ] Número de pontos FFT
  - [ ] Zero padding (sim/não, quantidade)

### 4.4 Opções de Visualização
- [ ] **Configurações visuais**
  - [ ] Faixa dinâmica (dynamic range) - ex: 60 dB
  - [ ] Paleta de cores:
    - [ ] Grayscale
    - [ ] Jet
    - [ ] Hot
    - [ ] Cool
    - [ ] Viridis
    - [ ] Personalizada
  - [ ] Faixa de frequência (0-8000 Hz, 0-22050 Hz, etc.)

### 4.5 Integração com FFTW3
- [ ] **Implementação**
  - [ ] Cálculo usando FFTW3
  - [ ] Cache de resultados
  - [ ] Cálculo assíncrono para não travar interface

---

## 5. SISTEMA DE ANOTAÇÕES

### 5.1 Compatibilidade com TextGrid do Praat
- [ ] **Estrutura compatível**
  - [x] Modelo de dados IntervalTier
  - [x] Modelo de dados PointTier (TextTier)
  - [ ] Import de arquivos .TextGrid
  - [ ] Export de arquivos .TextGrid

### 5.2 Gerenciamento de Camadas
- [ ] **Operações básicas**
  - [ ] Adicionar camadas (quantas necessárias)
  - [ ] Remover camadas
  - [ ] Renomear camadas
  - [ ] Reordenar camadas
  - [ ] Mostrar/ocultar camadas

### 5.3 Visualização e Edição
- [ ] **Restrição de até 2 camadas**
  - [ ] Seletor de qual camada visualizar
  - [ ] Edição simultânea de até 2 camadas
  - [ ] Camada ativa destacada visualmente

### 5.4 IntervalTier - Camadas de Intervalos
- [ ] **Interatividade completa**
  - [ ] Adicionar intervalo (clique duplo ou botão)
  - [ ] Mover fronteira esquerda do intervalo (arrastar)
  - [ ] Mover fronteira direita do intervalo (arrastar)
  - [ ] Editar texto do intervalo (duplo clique)
  - [ ] Remover intervalo (Del ou menu de contexto)
  - [ ] Validação de sobreposição

### 5.5 PointTier - Camadas de Pontos
- [ ] **Interatividade**
  - [ ] Adicionar ponto (clique)
  - [ ] Mover ponto horizontalmente (arrastar)
  - [ ] Editar texto do ponto (duplo clique)
  - [ ] Remover ponto (Del ou menu de contexto)

### 5.6 Edição de Texto
- [ ] **Editor inline**
  - [ ] Campo de texto inline
  - [ ] Confirmação com Enter
  - [ ] Cancelamento com Esc
  - [ ] Texto multi-linha (opcional)

### 5.7 Seleção de Trechos
- [ ] **Integração com reprodução**
  - [ ] Selecionar trecho correspondente a um intervalo
  - [ ] Reproduzir apenas o trecho selecionado
  - [ ] Ajustar visualização ao intervalo

---

## 6. CONTROLES DE REPRODUÇÃO

### 6.1 Controles Básicos
- [ ] **Botões principais**
  - [ ] Play - iniciar reprodução
  - [ ] Pause - pausar reprodução
  - [ ] Stop - parar e voltar ao início
  - [ ] Volume - slider de controle

### 6.2 Seleção de Trecho
- [ ] **Reprodução parcial**
  - [ ] Selecionar trecho com mouse na waveform
  - [ ] Play apenas do trecho selecionado
  - [ ] Indicação visual da seleção

### 6.3 Modo Loop
- [ ] **Reprodução em loop**
  - [ ] Botão/checkbox de loop
  - [ ] Reproduzir trecho selecionado repetidamente
  - [ ] Indicador de modo loop ativo

### 6.4 Sincronização Visual
- [ ] **Durante reprodução**
  - [ ] Cursor movendo em tempo real
  - [ ] Auto-scroll se cursor sair da visualização
  - [ ] Atualização de indicador de tempo

### 6.5 Integração com Anotações
- [ ] **Reprodução de intervalos**
  - [ ] Clicar em intervalo para reproduzir
  - [ ] Ajustar seleção ao intervalo
  - [ ] Loop automático do intervalo

---

## 7. ANÁLISE ACÚSTICA

### 7.1 Frequência Fundamental (Pitch/F0)
- [ ] **Algoritmo YAAPT**
  - [ ] Implementar ou integrar YAAPT
  - [ ] Referências:
    - https://ws.binghamton.edu/zahorian/yaapt.htm
    - https://bjbschmitt.github.io/AMFM_decompy/pYAAPT.html
  
- [ ] **Cálculo e visualização**
  - [ ] Diálogo de parâmetros YAAPT
  - [ ] Cálculo com barra de progresso
  - [ ] Visualização sobreposta no espectrograma
  - [ ] Linha/pontos indicando F0

- [ ] **Pré-cálculo e armazenamento**
  - [ ] Calcular e salvar como objeto
  - [ ] Associar ao arquivo de áudio
  - [ ] Carregar de arquivo salvo
  - [ ] Não recalcular se já existe

### 7.2 Intensidade
- [ ] **Cálculo RMS**
  - [ ] Implementar cálculo de intensidade
  - [ ] Parâmetros configuráveis (janela, passo)
  - [ ] Resultado em dB

- [ ] **Visualização**
  - [ ] Sobreposta no espectrograma
  - [ ] Curva indicando intensidade
  - [ ] Escala de cores opcional

- [ ] **Pré-cálculo e armazenamento**
  - [ ] Salvar como objeto
  - [ ] Associar ao arquivo de áudio
  - [ ] Reutilizar cálculo salvo

### 7.3 Visualização Integrada
- [ ] **Camada do espectrograma**
  - [ ] Pitch sobreposto
  - [ ] Intensidade sobreposta
  - [ ] Controle de visibilidade individual
  - [ ] Cores personalizáveis
  - [ ] Espessura de linha ajustável

---

## 8. GESTÃO DE PROJETO

### 8.1 Operações de Projeto
- [ ] **Novo Projeto**
  - [ ] Limpar dados atuais
  - [ ] Confirmar se há modificações não salvas
  - [ ] Inicializar projeto vazio

- [ ] **Salvar Projeto**
  - [ ] Definir formato de arquivo (JSON, XML, ou binário)
  - [ ] Salvar todos os arquivos de áudio (referências)
  - [ ] Salvar todas as camadas de anotação
  - [ ] Salvar objetos associados (pitch, intensidade)
  - [ ] Salvar configurações (zoom, visualização, etc.)
  - [ ] Extensão: .aap (AudioAnnotator Project)

- [ ] **Carregar Projeto**
  - [ ] Abrir arquivo de projeto
  - [ ] Restaurar arquivos de áudio
  - [ ] Restaurar camadas de anotação
  - [ ] Restaurar objetos associados
  - [ ] Restaurar configurações

- [ ] **Limpar Dados**
  - [ ] Fechar todos os arquivos
  - [ ] Limpar todas as anotações
  - [ ] Confirmar antes de limpar
  - [ ] Liberar memória

### 8.2 Estado de Modificação
- [ ] **Rastreamento de mudanças**
  - [ ] Indicador de projeto modificado (asterisco no título)
  - [ ] Prompt ao fechar sem salvar
  - [ ] Auto-save opcional

### 8.3 Projetos Recentes
- [ ] **Lista de projetos recentes**
  - [ ] Menu com últimos projetos
  - [ ] Abrir rapidamente
  - [ ] Limpar lista de recentes

---

## 9. EXPORTAÇÃO/IMPORTAÇÃO TextGrid

### 9.1 Formato TextGrid do Praat
- [ ] **Compatibilidade total**
  - [ ] Formato de texto longo (long text format)
  - [ ] Formato de texto curto (short text format)
  - [ ] Preservar estrutura de camadas
  - [ ] Preservar tipos (IntervalTier vs TextTier)

### 9.2 Exportação
- [ ] **Export de anotações**
  - [ ] Diálogo de exportação
  - [ ] Escolher formato (longo/curto)
  - [ ] Escolher camadas a exportar
  - [ ] Salvar como .TextGrid
  - [ ] Validação de dados antes de exportar

### 9.3 Importação
- [ ] **Import de TextGrid**
  - [ ] Abrir arquivo .TextGrid
  - [ ] Parser robusto
  - [ ] Criar camadas correspondentes
  - [ ] Associar ao arquivo de áudio correto
  - [ ] Tratamento de erros

### 9.4 Teste de Compatibilidade
- [ ] **Validação com Praat**
  - [ ] Exportar do AudioAnnotator
  - [ ] Abrir no Praat
  - [ ] Verificar integridade
  - [ ] Importar do Praat
  - [ ] Verificar fidelidade

---

## 10. ESTRUTURA PROFISSIONAL

### 10.1 Organização de Código
- [x] **Padrão MVC**
  - [x] Modelos (models/)
  - [x] Visualizações (views/)
  - [x] Controladores (controllers/)
  - [x] Processamento de áudio (audio/)
  - [x] Utilitários (utils/)

### 10.2 Build System
- [x] **CMake**
  - [x] CMakeLists.txt profissional
  - [x] Detecção de dependências
  - [x] Configuração multi-plataforma
  - [x] Scripts de build

### 10.3 Ambiente de Desenvolvimento
- [x] **VS Code**
  - [x] Configuração .vscode/
  - [x] Tasks de build
  - [x] Configuração de debug
  - [x] IntelliSense configurado

### 10.4 Documentação
- [x] **Documentação completa**
  - [x] README.md
  - [x] ARCHITECTURE.md
  - [x] DEVELOPMENT.md
  - [x] TODO.md
  - [x] CHANGELOG.md
  - [x] Este documento (ESPECIFICACAO_COMPLETA.md)

### 10.5 Controle de Versão
- [ ] **Git**
  - [x] .gitignore configurado
  - [ ] Histórico de commits organizado
  - [ ] Branches para features
  - [ ] Tags para versões

---

## 11. CHECKLIST DE IMPLEMENTAÇÃO

### Fase 1: Fundação (COMPLETO ✅)
- [x] Estrutura do projeto
- [x] Sistema de build CMake
- [x] Modelos de dados
- [x] Interface básica
- [x] Abertura de arquivos de áudio
- [x] Decodificação com libsndfile
- [x] Tradução para português

### Fase 2: Visualização Básica (PRÓXIMA)
- [ ] Desenho de waveform
- [ ] Zoom e pan interativos
- [ ] Labels de tempo
- [ ] Seleção de trechos
- [ ] Cursor de reprodução

### Fase 3: Reprodução
- [ ] Integração QMediaPlayer/QAudioSink
- [ ] Controles play/pause/stop
- [ ] Volume
- [ ] Loop de trecho
- [ ] Sincronização visual

### Fase 4: Anotações
- [ ] Visualização de camadas
- [ ] Adicionar/remover intervalos
- [ ] Mover fronteiras
- [ ] Edição de texto
- [ ] Adicionar/mover pontos

### Fase 5: Espectrograma
- [ ] Cálculo com FFTW3
- [ ] Visualização
- [ ] Configurações de parâmetros
- [ ] Paletas de cores
- [ ] Sincronização

### Fase 6: Análise Acústica
- [ ] Intensidade (RMS)
- [ ] Pitch (YAAPT)
- [ ] Visualização sobreposta
- [ ] Pré-cálculo e cache

### Fase 7: TextGrid
- [ ] Exportação
- [ ] Importação
- [ ] Testes de compatibilidade

### Fase 8: Gestão de Projeto
- [ ] Salvar projeto
- [ ] Carregar projeto
- [ ] Formato de arquivo

### Fase 9: Polimento
- [ ] Ícones
- [ ] Atalhos de teclado
- [ ] Tooltips
- [ ] Mensagens de ajuda
- [ ] Performance

### Fase 10: Testes e Entrega
- [ ] Testes unitários
- [ ] Testes de integração
- [ ] Manual do usuário
- [ ] Empacotamento

---

## 12. PRIORIDADES SUGERIDAS

### Curto Prazo (1-2 semanas)
1. ✅ Abertura de arquivos - COMPLETO
2. Visualização de waveform
3. Zoom e pan básicos
4. Reprodução de áudio básica

### Médio Prazo (3-4 semanas)
5. Sistema de anotações interativo
6. Export/import TextGrid
7. Espectrograma básico

### Longo Prazo (5-8 semanas)
8. Pitch (YAAPT)
9. Intensidade
10. Gestão completa de projeto
11. Polimento e otimização

---

## 13. RECURSOS NECESSÁRIOS

### Bibliotecas
- [x] Qt6 (Core, Widgets, Multimedia)
- [x] libsndfile
- [x] FFTW3
- [ ] YAAPT (implementação ou port)

### Documentação
- Praat TextGrid: https://www.fon.hum.uva.nl/praat/
- YAAPT: https://ws.binghamton.edu/zahorian/yaapt.htm
- FFTW3: https://www.fftw.org/

---

**Última atualização**: 28 de Outubro de 2025
**Progresso Geral**: ~15% completo
