# Convenções de Organização - BionoteEchos

## Resumo das Regras

### 📚 Documentação (`.md`)
- **Localização**: `docs/`
- **Exceção**: `README.md` na raiz
- **Formato**: `NOME_DESCRITIVO.md` (CAPS_SNAKE_CASE)

### 🔧 Scripts (`.sh`)
- **Localização**: `scripts/`
- **Formato**: `nome-descritivo.sh` (kebab-case)
- **Permissão**: Executável (`chmod +x`)

### 📋 Logs (`.log`)
- **Localização**: `logs/`
- **Git**: Ignorados (diretório versionado com `.gitkeep`)
- **Formato**: `nome-descritivo.log` (kebab-case)

### 💻 Código (`.cpp`, `.h`)
- **Localização**: `src/` e `include/`
- **Formato**: `PascalCase` para classes

## Estrutura de Diretórios

```
BionoteEchos/
├── README.md              # ✅ Único .md na raiz
├── .gitignore             # ✅ Ignora logs/*.log
│
├── docs/                  # 📚 Toda documentação
│   ├── INDEX.md
│   ├── ORGANIZACAO.md
│   ├── CONVENCOES.md      # Este arquivo
│   └── ...
│
├── scripts/               # 🔧 Todos os scripts
│   ├── build.sh
│   ├── install-deps.sh
│   └── ...
│
├── logs/                  # 📋 Logs (ignorados)
│   ├── .gitkeep          # ✅ Mantém diretório no git
│   ├── app.log           # ❌ Ignorado no git
│   ├── player.log        # ❌ Ignorado no git
│   └── audio.log         # ❌ Ignorado no git
│
├── src/                   # 💻 Código fonte
├── include/               # 📦 Headers
└── resources/             # 🎨 Recursos
```

## .gitignore

```gitignore
# Logs
logs/*.log
*.log

# Build
build/
build-*/

# Outros...
```

## Checklist

### ✅ Fazer
- [x] Documentação em `docs/`
- [x] Scripts em `scripts/`
- [x] Logs em `logs/`
- [x] README.md na raiz
- [x] `.gitkeep` em `logs/`
- [x] `.log` ignorados no git

### ❌ Evitar
- [ ] `.md` na raiz (exceto README.md)
- [ ] `.sh` fora de `scripts/`
- [ ] `.log` versionados no git
- [ ] Documentação misturada com código

## Comandos Úteis

### Verificar Organização
```bash
# Verificar .md fora de docs/
find . -maxdepth 1 -name "*.md" ! -name "README.md"

# Verificar .sh fora de scripts/
find . -maxdepth 1 -name "*.sh"

# Verificar .log versionados
git ls-files | grep "\.log$"
```

### Limpar Logs
```bash
# Remover todos os logs
rm -f logs/*.log

# Verificar se diretório existe
ls -la logs/
```

### Adicionar Novo Arquivo

**Documentação**:
```bash
touch docs/NOVO_DOCUMENTO.md
# Editar e adicionar ao docs/INDEX.md
```

**Script**:
```bash
touch scripts/novo-script.sh
chmod +x scripts/novo-script.sh
# Adicionar shebang e documentar
```

**Log**:
```bash
# Logs são criados automaticamente pela aplicação
# Não precisa criar manualmente
```

## Benefícios

1. **Organização Clara**: Fácil encontrar arquivos
2. **Git Limpo**: Apenas código versionado
3. **Manutenção Fácil**: Estrutura consistente
4. **Profissional**: Projeto bem organizado
5. **Escalável**: Fácil adicionar novos arquivos

## Contato

**Desenvolvedor**: Dr. Adelino Pinheiro Silva  
**Email**: adelinocpp@gmail.com  
**GitHub**: https://github.com/adelinocpp/BionoteEchos
