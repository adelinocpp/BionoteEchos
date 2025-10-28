# Changelog - AudioAnnotator

## [1.0.1] - 28 de Outubro de 2025

### ✅ Implementado

#### Internacionalização (i18n)
- **Interface em Português**: Toda a aplicação foi traduzida para português brasileiro
- **Estrutura para múltiplos idiomas**: Preparação para suporte futuro a outros idiomas
- Uso de `tr()` para todas as strings visíveis ao usuário
- Configuração de locale padrão para pt_BR
- Diretório `translations/` criado para arquivos de tradução futuros

#### Funcionalidade de Abertura de Arquivos de Áudio
- **Decodificador de Áudio Completo** (`AudioDecoder`)
  - Implementação usando libsndfile
  - Suporte a múltiplos formatos: WAV, FLAC, OGG, OPUS, AIFF, AU, e muitos outros
  - Decodificação com progresso em tempo real
  - Leitura de metadados (taxa de amostragem, canais, duração, codec, bit depth)
  - Desentrelaçamento automático de canais
  - Tratamento robusto de erros

- **Função onOpenAudioFiles() Implementada**
  - Diálogo de seleção de múltiplos arquivos
  - Filtros por tipo de arquivo
  - Verificação de duplicatas no projeto
  - Feedback visual com barra de status
  - Mensagens de sucesso/erro detalhadas
  - Contabilização de arquivos carregados vs. erros

#### Melhorias de Código
- Correção do erro "double free or corruption"
- Remoção de parent do Qt em objetos gerenciados por `shared_ptr`
- Adição de `objectName` para toolbars (elimina warnings)
- Includes adicionais necessários

### 📁 Arquivos Modificados

#### Novos Arquivos
- `translations/AudioAnnotator_pt_BR.ts` - Arquivo de tradução (preparado para futuro)
- `docs/CHANGELOG.md` - Este arquivo

#### Arquivos Modificados
- `src/audio/AudioDecoder.cpp` - Implementação completa do decodificador
- `include/audio/AudioDecoder.h` - Interface expandida com métodos funcionais
- `src/views/MainWindow.cpp` - Implementação de onOpenAudioFiles() e tradução
- `src/main.cpp` - Configuração de locale e sistema de tradução
- `CMakeLists.txt` - Preparação para sistema de tradução
- `src/models/AudioFile.cpp` - Já estava implementado
- `include/models/AudioFile.h` - Já estava implementado

### 🎯 Funcionalidades

#### Abertura de Arquivos
```cpp
// Menu: Arquivo → Abrir Arquivos de Áudio
// Atalho: Ctrl+O
// Suporta seleção múltipla
// Formatos: WAV, FLAC, OGG, OPUS, AIFF, AU, SND, etc.
```

#### Feedback ao Usuário
- **Barra de status**: Mostra arquivo sendo carregado
- **Mensagens de sucesso**: Confirmação com contagem de arquivos
- **Mensagens de erro**: Detalhamento de falhas com mensagem do erro
- **Verificação de duplicatas**: Avisa se arquivo já está aberto

### 🔧 Detalhes Técnicos

#### AudioDecoder
- **Biblioteca**: libsndfile
- **Formato de dados**: float32 (normalizado)
- **Buffer**: 8192 amostras por leitura
- **Progresso**: Emite sinais com percentual de conclusão
- **Memória**: Desentrelaça canais para acesso eficiente

#### Suporte a Formatos
| Formato | Extensões | Status |
|---------|-----------|--------|
| WAV | .wav | ✅ Completo |
| FLAC | .flac | ✅ Completo |
| OGG | .ogg, .opus | ✅ Completo |
| AIFF | .aiff, .aif | ✅ Completo |
| AU | .au, .snd | ✅ Completo |
| Outros | .voc, .w64, .mat, etc. | ✅ Completo |

### 📊 Estatísticas

- **Linhas de código adicionadas**: ~250
- **Arquivos modificados**: 8
- **Arquivos novos**: 2
- **Tamanho do binário Debug**: 6.9 MB

### 🚀 Próximos Passos

- [ ] Implementar visualização de waveform
- [ ] Implementar reprodução de áudio
- [ ] Adicionar sistema de anotações interativo
- [ ] Implementar exportação/importação TextGrid
- [ ] Adicionar preferências de idioma na interface
- [ ] Implementar carregamento assíncrono para arquivos grandes
- [ ] Adicionar barra de progresso visual durante carregamento

### 🐛 Bugs Corrigidos

- ✅ Double free or corruption ao fechar aplicação
- ✅ Warnings de objectName não definido para toolbars
- ✅ Falta de includes necessários

### 📝 Notas

- As traduções estão embutidas no código usando `tr()`
- Sistema preparado para adicionar `.qm` files no futuro
- Locale configurado para pt_BR por padrão
- Arquivo `.ts` criado como template para tradução profissional futura

---

**Desenvolvido por**: Adelino
**Data**: 28 de Outubro de 2025
**Versão**: 1.0.1-alpha
