#!/bin/bash

# AudioAnnotator Quick Build Script
# Atalho rápido para builds comuns

# Colors
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m'

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo -e "${BLUE}AudioAnnotator Quick Build Menu${NC}"
echo "=============================="
echo ""
echo "Escolha uma opção:"
echo ""
echo "  1) Debug + Run (desenvolvimento)"
echo "  2) Debug (apenas compilar)"
echo "  3) Release (produção)"
echo "  4) Debug Clean + Run"
echo "  5) Release Clean"
echo "  0) Sair"
echo ""
read -p "Opção [1]: " choice
choice=${choice:-1}

case $choice in
    1)
        echo -e "${GREEN}Compilando em modo Debug e executando...${NC}"
        "$SCRIPT_DIR/build.sh" -d -R
        ;;
    2)
        echo -e "${GREEN}Compilando em modo Debug...${NC}"
        "$SCRIPT_DIR/build.sh" -d
        ;;
    3)
        echo -e "${GREEN}Compilando em modo Release...${NC}"
        "$SCRIPT_DIR/build.sh" -r
        ;;
    4)
        echo -e "${GREEN}Compilando em modo Debug (clean) e executando...${NC}"
        "$SCRIPT_DIR/build.sh" -d -c -R
        ;;
    5)
        echo -e "${GREEN}Compilando em modo Release (clean)...${NC}"
        "$SCRIPT_DIR/build.sh" -r -c
        ;;
    0)
        echo "Saindo..."
        exit 0
        ;;
    *)
        echo "Opção inválida!"
        exit 1
        ;;
esac
