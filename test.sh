#!/bin/bash

gcc -o main main.c

if [ $? -ne 0 ]; then
    echo "Erro na compilação!"
    exit 1
fi

echo "===== TESTE DE EXPRESSÕES ARITMÉTICAS ====="
echo 

# Array de expressões a serem testadas
EXPRESSOES=(
    "5+2*3"
    "5*(2-4)/2)"
    "5-4*((3)+2)"
    "4*(9/3+2)-1"
    ")2*5"
    "5+3%3"
    "1+2+3+4"
    "8/4/2"
    "2*(3+4*5)"
    "9%4+2"
    "(((5)))"
)

for expr in "${EXPRESSOES[@]}"; do
    echo "Testando: $expr"
    echo "$expr" | ./main
    echo "----------------------------------------"
done

echo "===== FIM DOS TESTES ====="
