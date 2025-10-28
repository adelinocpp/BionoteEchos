# Scripts do AudioAnnotator

Este diretório contém scripts utilitários para o projeto AudioAnnotator.

## install-deps.sh

Script para instalar automaticamente as dependências necessárias.

### Uso

```bash
# Instalar todas as dependências
./scripts/install-deps.sh
```

O script detecta automaticamente a distribuição Linux e instala:
- Ferramentas de build (gcc, cmake, pkg-config)
- Qt6 (Core, Widgets, Multimedia)
- Bibliotecas de áudio (FFTW3, libsndfile, opus, vorbis, flac, mpg123)

Distribuições suportadas:
- Ubuntu/Debian/Linux Mint/Pop!_OS
- Fedora/RHEL/CentOS
- Arch Linux/Manjaro
- openSUSE

## build.sh

Script de build automatizado para compilar a aplicação em modo Debug ou Release.

### Uso Básico

```bash
# Build em modo Release (padrão)
./scripts/build.sh

# Build em modo Debug
./scripts/build.sh --debug

# Build em modo Debug e executar após build
./scripts/build.sh --debug --run

# Build limpo em modo Release
./scripts/build.sh --release --clean
```

### Opções

| Opção | Descrição |
|-------|-----------|
| `-d, --debug` | Compila em modo Debug (com símbolos de depuração) |
| `-r, --release` | Compila em modo Release (otimizado) |
| `-R, --run` | Executa a aplicação após o build (apenas Debug) |
| `-c, --clean` | Limpa o diretório de build antes de compilar |
| `-j, --jobs N` | Define número de jobs paralelos (padrão: nproc) |
| `-h, --help` | Exibe ajuda |

### Exemplos

```bash
# Build Debug e executar
./scripts/build.sh -d -R

# Build Release com 8 threads
./scripts/build.sh -r -j 8

# Build Debug limpo com 4 threads
./scripts/build.sh -d -c -j 4
```

### Diretórios de Build

O script cria diretórios separados para cada tipo de build:
- **build-debug**: Build em modo Debug
- **build-release**: Build em modo Release

Isso permite manter ambas as versões compiladas simultaneamente.

### Requisitos

- CMake 3.16+
- Qt6 (Core, Widgets, Multimedia)
- FFTW3
- libsndfile
- Compilador C++17 (GCC, Clang)

### Notas

- O script detecta automaticamente o número de cores disponíveis para compilação paralela
- No modo Debug, a opção `-R` executa automaticamente a aplicação após o build
- No modo Release, a opção `-R` é ignorada (apenas exibe mensagem de como executar)
- O script usa cores no terminal para melhor visualização (info: azul, sucesso: verde, erro: vermelho)

## quick-build.sh

Script interativo para builds rápidos com menu de opções.

### Uso

```bash
# Executar o menu interativo
./scripts/quick-build.sh
```

O script apresenta um menu com as opções mais comuns:
1. **Debug + Run**: Compila em modo debug e executa imediatamente (ideal para desenvolvimento)
2. **Debug**: Compila apenas em modo debug
3. **Release**: Compila em modo release (otimizado)
4. **Debug Clean + Run**: Limpa, compila em debug e executa
5. **Release Clean**: Limpa e compila em modo release

### Fluxo de Trabalho Recomendado

```bash
# Primeira vez - instalar dependências
./scripts/install-deps.sh

# Durante desenvolvimento - uso rápido
./scripts/quick-build.sh
# (escolha opção 1 para debug + run)

# Build manual com opções específicas
./scripts/build.sh -d -R -j 4

# Build de produção
./scripts/build.sh -r -c
```
