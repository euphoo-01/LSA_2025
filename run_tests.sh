#!/bin/bash

COMPILER="./cmake-build-lsa_2025/LSA_2025"
TEST_DIR="tests/errors"

if [ ! -f "$COMPILER" ]; then
    echo "Ошибка! Исполнимый файл компилятора не найден по пути: $COMPILER"
    exit 1
fi

echo "Запускаем тесты..."
echo "---------------------------------------------------"

PASS_COUNT=0
FAIL_COUNT=0

for test_file in "$TEST_DIR"/*.lsa; do
    # извлекаем ожидаемый код ошибки из первой строки (формат: // EXPECT: <code>)
    EXPECTED_CODE=$(grep -oP '// EXPECT: \K\d+' "$test_file" | head -n 1)

    if [ -z "$EXPECTED_CODE" ]; then
        echo "Предупреждение: Не задан ожидаемый код ошибки в $test_file. Тест пропущен."
        continue
    fi

    # запуск компилятора, отслеживание stdin и stderr
    OUTPUT=$("$COMPILER" -in:"$test_file" 2>&1)

    # проверка на вывод "Ошибка <код>"
    if echo "$OUTPUT" | grep -q "Ошибка $EXPECTED_CODE"; then
        echo -e "\e[32m[PASS]\e[0m $(basename "$test_file"): Получена ошибка $EXPECTED_CODE"
        ((PASS_COUNT++))
    else
        echo -e "\e[31m[FAIL]\e[0m $(basename "$test_file"): Ожидалась ошибка $EXPECTED_CODE"
        echo "stdout теста:"
        echo "$OUTPUT"
        ((FAIL_COUNT++))
    fi
done

echo "---------------------------------------------------"
echo "Тесты выполнены."
echo "Коды совпали: $PASS_COUNT"
echo "Коды различны: $FAIL_COUNT"

if [ $FAIL_COUNT -eq 0 ]; then
    exit 0
else
    exit 1
fi
