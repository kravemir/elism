#!/bin/bash
set -e

find examples -name "*.expect.out" | sort | while read RESULT_FILE;
do
    SRC_FILE=${RESULT_FILE%.expect.out}
    echo "Testing lexer: ${SRC_FILE} against ${RESULT_FILE}"
    ./build/bp_jit < "${SRC_FILE}" | diff - "${RESULT_FILE}"
done;
