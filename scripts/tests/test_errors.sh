#!/bin/bash

find scripts/tests/error_inputs -type f | sort | while read SRC_FILE;
do
    TMP_O=$(mktemp)
    echo "Testing error: ${SRC_FILE}"
    ./build/bp_jit -o "${TMP_O}" "${SRC_FILE}"
    [[ "$?" == "121" ]] || exit 1
done || (echo "FAIL: Errors" && exit 1)

echo "DONE: error testing"
