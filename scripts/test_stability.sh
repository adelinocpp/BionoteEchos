#!/bin/bash

echo "=== Teste de Estabilidade do BionoteEchos ==="
echo "Iniciando aplicação..."
echo ""

# Executar com logs detalhados
export QT_LOGGING_RULES="*.debug=false;*.warning=true;*.critical=true"
export QT_DEBUG_PLUGINS=0

LOG_FILE="stability_test.log"
PID_FILE="app.pid"

# Limpar logs anteriores
> $LOG_FILE

# Iniciar aplicação em background
./build-debug/bin/BionoteEchos > $LOG_FILE 2>&1 &
APP_PID=$!
echo $APP_PID > $PID_FILE

echo "Aplicação iniciada com PID: $APP_PID"
echo "Monitorando por 30 segundos..."
echo ""

# Monitorar por 30 segundos
for i in {1..30}; do
    if ! ps -p $APP_PID > /dev/null 2>&1; then
        echo "❌ ERRO: Aplicação fechou inesperadamente após $i segundos!"
        echo ""
        echo "=== Últimas linhas do log ==="
        tail -20 $LOG_FILE
        echo ""
        echo "=== Verificando core dump ==="
        if [ -f core ]; then
            echo "Core dump encontrado!"
            gdb -batch -ex "bt" ./build-debug/bin/BionoteEchos core
        fi
        exit 1
    fi
    
    # Mostrar progresso
    if [ $((i % 5)) -eq 0 ]; then
        echo "  $i segundos... (ainda rodando)"
    fi
    
    sleep 1
done

echo ""
echo "✅ Aplicação rodou por 30 segundos sem problemas!"
echo ""
echo "=== Estatísticas ==="
echo "PID: $APP_PID"
ps -p $APP_PID -o pid,vsz,rss,cmd

echo ""
echo "=== Logs da aplicação ==="
tail -30 $LOG_FILE

echo ""
echo "Encerrando aplicação..."
kill $APP_PID 2>/dev/null
wait $APP_PID 2>/dev/null

rm -f $PID_FILE

echo ""
echo "Teste concluído!"
