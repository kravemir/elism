#!/bin/bash
set -e

find examples -name "*.expect.lexer" | while read RESULT_FILE;
do
    SRC_FILE=${RESULT_FILE%.expect.lexer}
    ./build/bp < "${SRC_FILE}" | diff - "${RESULT_FILE}"
    echo "Passed: ${SRC_FILE} (with result ${RESULT_FILE})"
done;
