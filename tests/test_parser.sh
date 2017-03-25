#!/bin/bash
set -e

find examples -name "*.expect.parser" | while read RESULT_FILE;
do
    SRC_FILE=${RESULT_FILE%.expect.parser}
    echo "Testing parser: ${SRC_FILE} against ${RESULT_FILE}"
    ./build/bp_parser < "${SRC_FILE}" | diff - "${RESULT_FILE}"
done;
