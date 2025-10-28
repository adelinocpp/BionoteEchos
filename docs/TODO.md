# TODO - AudioAnnotator

Este documento lista as funcionalidades que precisam ser implementadas para completar o AudioAnnotator.

## Prioridade Alta

### Decodificação de Áudio
- [ ] Implementar `AudioDecoder::decode()` usando QAudioDecoder
- [ ] Adicionar suporte a libsndfile para formatos adicionais
- [ ] Implementar detecção automática de formato por extensão
- [ ] Adicionar tratamento de erros robusto
- [ ] Implementar carregamento assíncrono para arquivos grandes
- [ ] Adicionar barra de progresso durante carregamento

### Visualização de Waveform
- [ ] Implementar `AudioVisualizationWidget::drawWaveform()` completo
- [ ] Adicionar downsampling para visualização eficiente
- [ ] Implementar zoom horizontal interativo
- [ ] Implementar navegação por arrastar (pan)
- [ ] Adicionar seleção de trechos com mouse
- [ ] Implementar cursor de reprodução animado
- [ ] Adicionar labels de tempo dinâmicos

### Reprodução de Áudio
- [ ] Implementar `AudioPlayer` usando QMediaPlayer ou QAudioSink
- [ ] Adicionar controles play/pause/stop
- [ ] Implementar reprodução de trecho selecionado
- [ ] Adicionar modo loop
- [ ] Implementar sincronização visual durante reprodução
- [ ] Adicionar controle de volume funcional

### Sistema de Anotações
- [ ] Implementar desenho de intervalos em `AnnotationLayerWidget`
- [ ] Implementar desenho de pontos em `AnnotationLayerWidget`
- [ ] Adicionar interatividade: adicionar intervalos com mouse
- [ ] Adicionar interatividade: mover fronteiras de intervalos
- [ ] Adicionar interatividade: adicionar pontos com mouse
- [ ] Implementar edição inline de texto
- [ ] Adicionar seleção de intervalos/pontos
- [ ] Implementar remoção de intervalos/pontos
- [ ] Adicionar validação de sobreposição de intervalos
- [ ] Implementar sincronização com visualização de áudio

### Gestão de Projeto
- [ ] Implementar `ProjectController::saveProject()`
- [ ] Implementar `ProjectController::loadProject()`
- [ ] Definir formato de arquivo de projeto (JSON ou binário)
- [ ] Adicionar serialização de AudioFile
- [ ] Adicionar serialização de AnnotationTier
- [ ] Implementar diálogo "Salvar mudanças?" ao fechar
- [ ] Adicionar lista de projetos recentes

### Exportação/Importação TextGrid
- [ ] Implementar `TextGridExporter::export()` completo
- [ ] Adicionar suporte a formato longo (long text)
- [ ] Adicionar suporte a formato curto (short text)
- [ ] Implementar `TextGridImporter::import()` completo
- [ ] Adicionar parser robusto para variações de formato
- [ ] Adicionar validação de arquivo TextGrid
- [ ] Testar compatibilidade com Praat

## Prioridade Média

### Espectrograma
- [ ] Implementar `SpectrogramCalculator::calculate()` com FFTW3
- [ ] Adicionar janelas espectrais (Hamming, Hann, Blackman)
- [ ] Implementar zero padding configurável
- [ ] Adicionar paletas de cores (Grayscale, Jet, Hot, Cool)
- [ ] Implementar controle de faixa dinâmica (dynamic range)
- [ ] Adicionar diálogo de configurações de espectrograma
- [ ] Implementar cache de espectrograma calculado
- [ ] Adicionar visualização sincronizada com waveform
- [ ] Implementar controle de nível de zoom para exibição

### Cálculo de Pitch (F0)
- [ ] Pesquisar implementações de YAAPT em C++
- [ ] Implementar algoritmo YAAPT ou alternativa (YIN, autocorrelação)
- [ ] Adicionar diálogo de configurações de pitch
- [ ] Implementar parâmetros configuráveis (F0 min/max, frame size, etc.)
- [ ] Adicionar visualização de pitch no espectrograma
- [ ] Implementar cache de dados de pitch
- [ ] Adicionar exportação de dados de pitch

### Cálculo de Intensidade
- [ ] Implementar `IntensityCalculator::calculate()` com RMS
- [ ] Adicionar parâmetros configuráveis (window size, hop size)
- [ ] Implementar visualização de intensidade no espectrograma
- [ ] Adicionar cache de dados de intensidade
- [ ] Adicionar exportação de dados de intensidade

### Interface Gráfica
- [ ] Adicionar ícones para botões e ações
- [ ] Implementar diálogo "Sobre" com informações do projeto
- [ ] Adicionar diálogo de preferências/configurações
- [ ] Implementar barra de status com informações contextuais
- [ ] Adicionar tooltips para todos os controles
- [ ] Implementar atalhos de teclado adicionais
- [ ] Adicionar tema escuro (opcional)

### Gerenciamento de Camadas
- [ ] Implementar diálogo "Adicionar Camada"
- [ ] Implementar diálogo "Renomear Camada"
- [ ] Adicionar seletor de camadas visíveis
- [ ] Implementar reordenação de camadas
- [ ] Adicionar cores personalizáveis para camadas
- [ ] Implementar ocultação temporária de camadas

## Prioridade Baixa

### Undo/Redo
- [ ] Implementar sistema de comandos (Command pattern)
- [ ] Adicionar undo para operações de anotação
- [ ] Adicionar redo para operações de anotação
- [ ] Implementar histórico de comandos
- [ ] Adicionar atalhos Ctrl+Z e Ctrl+Y

### Clipboard (Copiar/Colar)
- [ ] Implementar cópia de intervalos/pontos
- [ ] Implementar colagem de intervalos/pontos
- [ ] Adicionar suporte a clipboard do sistema
- [ ] Implementar formato de clipboard interno

### Busca e Navegação
- [ ] Implementar busca de texto em anotações
- [ ] Adicionar navegação entre intervalos (próximo/anterior)
- [ ] Implementar salto para tempo específico
- [ ] Adicionar marcadores (bookmarks)

### Análise Acústica Avançada
- [ ] Implementar cálculo de formantes
- [ ] Adicionar análise de energia
- [ ] Implementar detecção de voiced/unvoiced
- [ ] Adicionar análise de zero-crossing rate

### Exportação de Dados
- [ ] Adicionar exportação de waveform como imagem
- [ ] Adicionar exportação de espectrograma como imagem
- [ ] Implementar exportação de pitch como CSV
- [ ] Implementar exportação de intensidade como CSV
- [ ] Adicionar exportação de anotações como CSV/TSV

### Importação de Dados
- [ ] Adicionar importação de anotações de outros formatos (ELAN, Audacity labels)
- [ ] Implementar importação de pitch de arquivo CSV
- [ ] Implementar importação de intensidade de arquivo CSV

### Internacionalização (i18n)
- [ ] Adicionar suporte a múltiplos idiomas
- [ ] Criar arquivos de tradução (.ts)
- [ ] Traduzir interface para português brasileiro
- [ ] Traduzir interface para inglês
- [ ] Adicionar seletor de idioma nas preferências

### Documentação
- [ ] Adicionar documentação inline (Doxygen)
- [ ] Criar manual do usuário
- [ ] Adicionar tutoriais em vídeo
- [ ] Criar FAQ (Perguntas Frequentes)

### Testes
- [ ] Criar testes unitários para modelos
- [ ] Criar testes unitários para controladores
- [ ] Criar testes de integração
- [ ] Adicionar testes de compatibilidade com Praat
- [ ] Implementar testes de performance

### Performance
- [ ] Otimizar visualização de waveform para arquivos grandes
- [ ] Implementar carregamento incremental de áudio
- [ ] Adicionar cache de visualizações
- [ ] Otimizar cálculo de espectrograma com multithreading
- [ ] Implementar processamento em GPU (opcional)

### Plugins e Extensibilidade
- [ ] Definir API de plugins
- [ ] Implementar carregamento dinâmico de plugins
- [ ] Criar plugin de exemplo
- [ ] Adicionar suporte a scripts Python

## Bugs Conhecidos

- [ ] Nenhum bug conhecido no momento (projeto em fase inicial)

## Melhorias Futuras

### Versão 2.0
- [ ] Suporte a múltiplos arquivos de áudio simultâneos
- [ ] Sincronização de múltiplos arquivos de áudio
- [ ] Suporte a vídeo com áudio
- [ ] Análise de prosódia
- [ ] Integração com banco de dados de corpus
- [ ] Colaboração em tempo real
- [ ] Suporte a nuvem (salvar projetos online)

### Versão 3.0
- [ ] Machine learning para anotação automática
- [ ] Reconhecimento automático de fala
- [ ] Segmentação automática de palavras
- [ ] Detecção automática de eventos acústicos
- [ ] Análise de emoções em fala

## Notas

- As implementações stub atuais fornecem a estrutura básica
- Priorize funcionalidades de acordo com as necessidades do projeto
- Teste cada funcionalidade após implementação
- Mantenha compatibilidade com Praat TextGrid
- Documente código conforme implementa
- Adicione testes unitários para funcionalidades críticas

## Progresso

- [x] Estrutura do projeto
- [x] Sistema de build (CMake)
- [x] Modelos de dados básicos
- [x] Interface gráfica básica
- [x] Configuração VS Code
- [ ] Funcionalidades principais (em andamento)
- [ ] Testes
- [ ] Documentação completa
- [ ] Release 1.0

## Contribuindo

Para contribuir com a implementação:

1. Escolha um item da lista TODO
2. Crie uma branch para a feature
3. Implemente a funcionalidade
4. Adicione testes (se aplicável)
5. Atualize a documentação
6. Marque o item como concluído [x]
7. Faça um pull request

## Última Atualização

27 de outubro de 2025

