# Documentação do BionoteEchos

## Índice Geral

### 📋 Documentação Principal
- **[README.md](../README.md)** - Visão geral do projeto
- **[ARCHITECTURE.md](ARCHITECTURE.md)** - Arquitetura do sistema
- **[DEVELOPMENT.md](DEVELOPMENT.md)** - Guia de desenvolvimento

### 🚀 Especificações
- **[ESPECIFICACAO_COMPLETA.md](ESPECIFICACAO_COMPLETA.md)** - Especificação completa
- **[RESUMO_ESPECIFICACAO.md](RESUMO_ESPECIFICACAO.md)** - Resumo da especificação
- **[ENTREGA.md](ENTREGA.md)** - Critérios de entrega

### 🎵 Sistema de Áudio
- **[CUSTOM_AUDIO_PLAYER.md](CUSTOM_AUDIO_PLAYER.md)** - CustomAudioPlayer com PortAudio
- **[CONCLUSAO_QMEDIAPLAYER.md](CONCLUSAO_QMEDIAPLAYER.md)** - Análise do QMediaPlayer
- **[DIAGNOSTICO_AUDIO.md](DIAGNOSTICO_AUDIO.md)** - Diagnóstico de problemas de áudio
- **[PROBLEMA_REPRODUCAO.md](PROBLEMA_REPRODUCAO.md)** - Investigação de reprodução

### 🔧 Correções e Melhorias
- **[CORRECAO_DOUBLE_FREE.md](CORRECAO_DOUBLE_FREE.md)** - Correção do double free
- **[MELHORIAS_AUDACITY.md](MELHORIAS_AUDACITY.md)** - Melhorias inspiradas no Audacity
- **[DIAGNOSTICO_ESTABILIDADE.md](DIAGNOSTICO_ESTABILIDADE.md)** - Testes de estabilidade

### 🛠️ Implementações
- **[IMPLEMENTACAO_LISTA_ARQUIVOS.md](IMPLEMENTACAO_LISTA_ARQUIVOS.md)** - Lista de arquivos
- **[TESTE_ABERTURA_AUDIO.md](TESTE_ABERTURA_AUDIO.md)** - Testes de abertura

### 📝 Outros
- **[CHANGELOG.md](CHANGELOG.md)** - Histórico de mudanças
- **[TODO.md](TODO.md)** - Lista de tarefas
- **[SCRIPTS.md](SCRIPTS.md)** - Documentação dos scripts

## Estrutura do Projeto

```
BionoteEchos/
├── README.md                 # Documentação principal
├── CMakeLists.txt           # Configuração do CMake
├── docs/                    # 📚 Documentação
│   ├── INDEX.md            # Este arquivo
│   ├── ARCHITECTURE.md     # Arquitetura
│   ├── CUSTOM_AUDIO_PLAYER.md
│   └── ...
├── scripts/                 # 🔧 Scripts bash
│   ├── build.sh            # Script de compilação
│   ├── install-deps.sh     # Instalação de dependências
│   └── ...
├── src/                     # 💻 Código fonte
│   ├── main.cpp
│   ├── views/
│   ├── models/
│   ├── controllers/
│   ├── audio/
│   └── utils/
├── include/                 # 📦 Headers
│   ├── views/
│   ├── models/
│   ├── controllers/
│   ├── audio/
│   └── utils/
├── resources/               # 🎨 Recursos
│   ├── icons/
│   └── resources.qrc
└── logs/                    # 📋 Logs de execução

```

## Guias Rápidos

### Para Desenvolvedores
1. [DEVELOPMENT.md](DEVELOPMENT.md) - Como configurar ambiente
2. [ARCHITECTURE.md](ARCHITECTURE.md) - Entender a arquitetura
3. [SCRIPTS.md](SCRIPTS.md) - Usar os scripts de build

### Para Usuários
1. [README.md](../README.md) - Instalar e usar
2. [CHANGELOG.md](CHANGELOG.md) - Ver novidades

### Para Debugging
1. [DIAGNOSTICO_ESTABILIDADE.md](DIAGNOSTICO_ESTABILIDADE.md) - Testes de estabilidade
2. [DIAGNOSTICO_AUDIO.md](DIAGNOSTICO_AUDIO.md) - Problemas de áudio
3. [CORRECAO_DOUBLE_FREE.md](CORRECAO_DOUBLE_FREE.md) - Correções aplicadas

## Contribuindo

Ao adicionar nova documentação:
- ✅ Arquivos `.md` (exceto README.md) vão em `docs/`
- ✅ Scripts `.sh` vão em `scripts/`
- ✅ Atualizar este índice (INDEX.md)
- ✅ Usar nomes descritivos em CAPS_SNAKE_CASE.md

## Contato

**Desenvolvedor**: Dr. Adelino Pinheiro Silva  
**Email**: adelinocpp@gmail.com, adelinocpp@yahoo.com  
**Empresa**: Log Nature  
**GitHub**: https://github.com/adelinocpp/BionoteEchos
