#!/bin/bash
set -e

find examples -name "*.expect.lexer" | while read RESULT_FILE;
do
    SRC_FILE=${RESULT_FILE%.expect.lexer}
    echo "Testing lexer: ${SRC_FILE} against ${RESULT_FILE}"
    ./build/bp_lexer < "${SRC_FILE}" | diff - "${RESULT_FILE}"
done;
