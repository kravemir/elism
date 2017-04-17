#!/bin/bash
set -e

sh -c "cd examples; make"

find examples/build/ -type f -regex ".*/[0-9][-0-9a-zA-Z]*" | sort | while read SRC_FILE;
do
    echo "Testing with valgrind: ${SRC_FILE}"
    valgrind --error-exitcode=133 --log-file="${SRC_FILE}.log" "${SRC_FILE}"
done;
