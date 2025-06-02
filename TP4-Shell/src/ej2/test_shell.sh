#!/bin/bash

SHELL_EXEC=./shell
TEST_DIR=test_output

mkdir -p "$TEST_DIR"
rm -f "$TEST_DIR"/*

run_test() {
    desc=$1
    input=$2
    expected=$3

    echo "$input" | $SHELL_EXEC > "$TEST_DIR/out.txt"
    if [ -z "$expected" ]; then
        : > "$TEST_DIR/expected.txt"
    else
        printf "%s\n" "$expected" > "$TEST_DIR/expected.txt"
    fi

    if diff -w "$TEST_DIR/out.txt" "$TEST_DIR/expected.txt" > /dev/null; then
        echo "[PASS] $desc"
    else
        echo "[FAIL] $desc"
        diff -u "$TEST_DIR/expected.txt" "$TEST_DIR/out.txt"
    fi
}

# Tests
run_test "Simple echo" "echo hola" "hola"
run_test "Comillas dobles" "echo \"hola mundo\"" "hola mundo"
run_test "Comillas simples" "echo 'hola mundo'" "hola mundo"
run_test "Pipes básicos" "echo hola | grep hola | wc -l" "1"
run_test "Ruta absoluta" "/bin/echo 123" "123"
run_test "Comando inexistente" "asdfgh" ""
run_test "Pipe al final" "echo hola |" ""
run_test "Pipe doble" "echo hola || grep hola" ""
run_test "Múltiples vacíos" "echo hola |  | grep hola" ""
run_test "Comillas sin cerrar" "echo \"hola" ""
run_test "echo vacío" "echo \"\"" ""

# Stress test con 200 pipes
cmd="echo x"
for i in $(seq 1 199); do cmd="$cmd | cat"; done
cmd="$cmd | tail -n 1"
run_test "Stress 200 pipes" "$cmd" "x"

# Límite de argumentos
arglist="echo"
for i in $(seq 1 63); do arglist="$arglist arg"; done
expected=$(for i in $(seq 1 63); do echo -n "arg "; done)
run_test "63 args válidos" "$arglist" "$expected"

arglist="echo"
for i in $(seq 1 64); do arglist="$arglist arg"; done
run_test "64 args (exceso)" "$arglist" ""

echo "Todos los tests terminados."
