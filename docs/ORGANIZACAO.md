# Organização do Projeto BionoteEchos

## Estrutura de Diretórios

```
BionoteEchos/
├── README.md                    # 📋 Documentação principal (raiz)
│
├── docs/                        # 📚 Toda documentação .md (exceto README.md)
│   ├── INDEX.md                # Índice da documentação
│   ├── ARCHITECTURE.md         # Arquitetura do sistema
│   ├── DEVELOPMENT.md          # Guia de desenvolvimento
│   ├── CUSTOM_AUDIO_PLAYER.md  # Sistema de áudio
│   ├── CORRECAO_DOUBLE_FREE.md # Correções
│   ├── DIAGNOSTICO_*.md        # Diagnósticos
│   └── ...                     # Outros documentos
│
├── scripts/                     # 🔧 Scripts bash (.sh)
│   ├── build.sh                # Compilação
│   ├── install-deps.sh         # Instalação de dependências
│   ├── quick-build.sh          # Build rápido
│   └── test_stability.sh       # Teste de estabilidade
│
├── src/                         # 💻 Código fonte C++
│   ├── main.cpp
│   ├── views/                  # Interface gráfica
│   ├── models/                 # Modelos de dados
│   ├── controllers/            # Controladores
│   ├── audio/                  # Sistema de áudio
│   └── utils/                  # Utilitários
│
├── include/                     # 📦 Headers (.h)
│   ├── views/
│   ├── models/
│   ├── controllers/
│   ├── audio/
│   └── utils/
│
├── resources/                   # 🎨 Recursos
│   ├── icons/                  # Ícones e imagens
│   └── resources.qrc           # Arquivo de recursos Qt
│
├── logs/                        # 📋 Logs de execução (.log ignorados no git)
│   ├── .gitkeep                # Mantém diretório no git
│   ├── app.log                 # (ignorado)
│   ├── player.log              # (ignorado)
│   └── audio.log               # (ignorado)
│
├── build-debug/                 # 🔨 Build Debug (gerado)
│   └── bin/BionoteEchos
│
└── build-release/               # 🚀 Build Release (gerado)
    └── bin/BionoteEchos
```

## Convenções de Nomenclatura

### Documentação (.md)
- **Localização**: `docs/` (exceto README.md na raiz)
- **Formato**: `NOME_DESCRITIVO.md` (CAPS_SNAKE_CASE)
- **Exemplos**:
  - `CUSTOM_AUDIO_PLAYER.md`
  - `DIAGNOSTICO_ESTABILIDADE.md`
  - `CORRECAO_DOUBLE_FREE.md`

### Scripts (.sh)
- **Localização**: `scripts/`
- **Formato**: `nome-descritivo.sh` (kebab-case)
- **Exemplos**:
  - `build.sh`
  - `install-deps.sh`
  - `test_stability.sh`

### Código Fonte (.cpp, .h)
- **Localização**: `src/` e `include/`
- **Formato**: `PascalCase` para classes
- **Exemplos**:
  - `CustomAudioPlayer.cpp`
  - `MainWindow.h`
  - `AudioFile.cpp`

### Logs (.log)
- **Localização**: `logs/`
- **Formato**: `nome-descritivo.log` (kebab-case)
- **Git**: Ignorados (apenas diretório versionado)
- **Exemplos**:
  - `app.log`
  - `player.log`
  - `audio.log`

## Regras de Organização

### ✅ Fazer

1. **Documentação**:
   - Criar arquivos `.md` em `docs/`
   - Manter README.md na raiz
   - Atualizar `docs/INDEX.md` ao adicionar nova doc

2. **Scripts**:
   - Criar scripts `.sh` em `scripts/`
   - Tornar executável: `chmod +x scripts/nome.sh`
   - Adicionar comentários explicativos

3. **Código**:
   - Headers em `include/`
   - Implementação em `src/`
   - Manter estrutura de subdiretórios

4. **Recursos**:
   - Imagens/ícones em `resources/icons/`
   - Registrar em `resources/resources.qrc`

5. **Logs**:
   - Arquivos `.log` em `logs/`
   - Diretório versionado (com `.gitkeep`)
   - Arquivos `.log` ignorados no git

### ❌ Evitar

1. ❌ Arquivos `.md` na raiz (exceto README.md)
2. ❌ Scripts `.sh` fora de `scripts/`
3. ❌ Código fonte na raiz
4. ❌ Documentação misturada com código
5. ❌ Arquivos `.log` versionados no git

## Tipos de Documentação

### 📋 Documentação de Usuário
- `README.md` - Visão geral e instalação
- `docs/CHANGELOG.md` - Histórico de mudanças

### 🏗️ Documentação Técnica
- `docs/ARCHITECTURE.md` - Arquitetura
- `docs/DEVELOPMENT.md` - Desenvolvimento
- `docs/CUSTOM_AUDIO_PLAYER.md` - Componentes específicos

### 🐛 Diagnósticos e Correções
- `docs/DIAGNOSTICO_*.md` - Análises de problemas
- `docs/CORRECAO_*.md` - Correções aplicadas

### 📝 Especificações
- `docs/ESPECIFICACAO_*.md` - Requisitos e specs
- `docs/TODO.md` - Lista de tarefas

## Fluxo de Trabalho

### Adicionar Nova Documentação

1. Criar arquivo em `docs/`:
   ```bash
   touch docs/NOVO_DOCUMENTO.md
   ```

2. Editar conteúdo

3. Atualizar índice:
   ```bash
   # Adicionar link em docs/INDEX.md
   ```

4. Referenciar no README.md se relevante

### Adicionar Novo Script

1. Criar script em `scripts/`:
   ```bash
   touch scripts/novo-script.sh
   ```

2. Tornar executável:
   ```bash
   chmod +x scripts/novo-script.sh
   ```

3. Adicionar shebang e comentários:
   ```bash
   #!/bin/bash
   # Descrição do script
   ```

4. Documentar em `docs/SCRIPTS.md`

## Manutenção

### Limpeza Periódica

```bash
# Remover builds
rm -rf build-debug build-release

# Limpar logs (mantém diretório)
rm -f logs/*.log

# Manter apenas arquivos versionados
git clean -fdx
```

### Verificação de Organização

```bash
# Verificar .md fora de docs/
find . -maxdepth 1 -name "*.md" ! -name "README.md"

# Verificar .sh fora de scripts/
find . -maxdepth 1 -name "*.sh"

# Listar documentação
ls -la docs/

# Listar scripts
ls -la scripts/
```

## Benefícios da Organização

### ✅ Vantagens

1. **Clareza**: Fácil encontrar documentação
2. **Manutenibilidade**: Estrutura consistente
3. **Escalabilidade**: Fácil adicionar novos docs
4. **Profissionalismo**: Projeto bem organizado
5. **Colaboração**: Outros desenvolvedores entendem rápido

### 📊 Antes vs Depois

**Antes** (Desorganizado):
```
BionoteEchos/
├── README.md
├── CUSTOM_AUDIO_PLAYER.md
├── DIAGNOSTICO_ESTABILIDADE.md
├── CORRECAO_DOUBLE_FREE.md
├── test_stability.sh
├── src/
└── ...
```

**Depois** (Organizado):
```
BionoteEchos/
├── README.md
├── docs/
│   ├── INDEX.md
│   ├── CUSTOM_AUDIO_PLAYER.md
│   ├── DIAGNOSTICO_ESTABILIDADE.md
│   └── CORRECAO_DOUBLE_FREE.md
├── scripts/
│   └── test_stability.sh
├── src/
└── ...
```

## Checklist de Organização

- [x] Documentação em `docs/`
- [x] Scripts em `scripts/`
- [x] README.md na raiz
- [x] Índice criado (`docs/INDEX.md`)
- [x] README.md referencia `docs/`
- [x] Estrutura consistente
- [x] Convenções de nomenclatura definidas

## Contato

Para sugestões de melhoria na organização:

**Desenvolvedor**: Dr. Adelino Pinheiro Silva  
**Email**: adelinocpp@gmail.com  
**GitHub**: https://github.com/adelinocpp/BionoteEchos
