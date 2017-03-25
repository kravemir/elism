#!/usr/bin/env bash

cd build || exit 1
make || exit 1
cd ..

SRC_FILE="examples/$1"
RESULT_FILE="${SRC_FILE}.expect.parser"

echo "Testing parser: ${SRC_FILE} against ${RESULT_FILE}"
./build/bp_parser < "${SRC_FILE}" 2>&1 | diff -y - "${RESULT_FILE}"
