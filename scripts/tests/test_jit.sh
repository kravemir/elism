#!/bin/bash
set -e

: ${COMPILER:=./build/elism}

cd "$(dirname $0)/../../examples" > /dev/null
EXAMPLES_PATH=`pwd -P`
cd - > /dev/null

find ${EXAMPLES_PATH} -name "*.expect.out" | sort | while read RESULT_FILE;
do
    SRC_FILE=${RESULT_FILE%.expect.out}
    echo "Testing JIT execution: ${SRC_FILE} against ${RESULT_FILE}"
    ${COMPILER} "${SRC_FILE}" | diff - "${RESULT_FILE}"
done;
