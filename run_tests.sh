#!/bin/bash

COMPILER="./build/LSA_2025"
TEST_DIR="tests/errors"

if [ ! -f "$COMPILER" ]; then
    echo "Error: Compiler executable not found at $COMPILER"
    exit 1
fi

echo "Running tests..."
echo "---------------------------------------------------"

PASS_COUNT=0
FAIL_COUNT=0

for test_file in "$TEST_DIR"/*.lsa; do
    # Extract expected error code from the first line (format: // EXPECT: <code>)
    EXPECTED_CODE=$(grep -oP '// EXPECT: \K\d+' "$test_file" | head -n 1)

    if [ -z "$EXPECTED_CODE" ]; then
        echo "Warning: No expected code found in $test_file. Skipping."
        continue
    fi

    # Run compiler and capture stdout and stderr
    OUTPUT=$("$COMPILER" -in:"$test_file" 2>&1)
    
    # Check if the output contains "Ошибка <code" or "Error <code" depending on localization
    # Based on main.cpp: cout << "Ошибка " << e.id ...
    if echo "$OUTPUT" | grep -q "Ошибка $EXPECTED_CODE"; then
        echo -e "\e[32m[PASS]\e[0m $(basename "$test_file"): Caught error $EXPECTED_CODE"
        ((PASS_COUNT++))
    else
        echo -e "\e[31m[FAIL]\e[0m $(basename "$test_file"): Expected error $EXPECTED_CODE"
        echo "Output was:"
        echo "$OUTPUT"
        ((FAIL_COUNT++))
    fi
done

echo "---------------------------------------------------"
echo "Tests completed."
echo "Passed: $PASS_COUNT"
echo "Failed: $FAIL_COUNT"

if [ $FAIL_COUNT -eq 0 ]; then
    exit 0
else
    exit 1
fi
