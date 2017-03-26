#!/bin/bash
set -e

find examples -name "*.bp" | while read SRC_FILE;
do
    echo "Testing parser: ${SRC_FILE} against ${SRC_FILE}"
    ./build/bp_parser < "${SRC_FILE}" | diff - "${SRC_FILE}"
done;
