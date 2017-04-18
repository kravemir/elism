#!/bin/bash

echo -e  "\nBEGIN: basic error testing"

find scripts/tests/error_inputs -type f -name "*.bp.expect" | sort | while read RESULT_FILE;
do
    SRC_FILE=${RESULT_FILE%.expect}
    echo "Testing error: ${SRC_FILE}"
    ./build/bp_jit "${SRC_FILE}" 2>&1 | diff - "${RESULT_FILE}" || exit 1
done || (echo "FAIL: Errors" && exit 1)

echo -e "DONE: basic error testing\n"

echo -e "BEGIN: basic error testing"

find scripts/tests/error_regions -type f -name "*.bp.expect" | sort | while read RESULT_FILE;
do
    SRC_FILE=${RESULT_FILE%.expect}
    echo "Testing error: ${SRC_FILE}"
    ./build/bp_jit "${SRC_FILE}" 2>&1 | diff - "${RESULT_FILE}" || exit 1
done || (echo "FAIL: Errors" && exit 1)

echo -e "DONE: regions safety error testing\n"