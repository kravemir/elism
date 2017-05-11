#!/bin/bash

: ${COMPILER:=./build/elism}

pushd `dirname $0` > /dev/null
SCRIPT_PATH=`pwd -P`
popd > /dev/null

echo -e  "\nBEGIN: basic error testing"

find ${SCRIPT_PATH}/error_inputs -type f -name "*.bp.expect" | sort | while read RESULT_FILE;
do
    SRC_FILE=${RESULT_FILE%.expect}
    echo "Testing error handling: ${SRC_FILE}"
    ${COMPILER} "${SRC_FILE}" 2>&1 | diff - "${RESULT_FILE}" || exit 1
done || (echo "FAIL: Errors" && exit 1)

echo -e "DONE: basic error testing\n"

echo -e "BEGIN: basic error testing"

find ${SCRIPT_PATH}/error_regions -type f -name "*.bp.expect" | sort | while read RESULT_FILE;
do
    SRC_FILE=${RESULT_FILE%.expect}
    echo "Testing error handling: ${SRC_FILE}"
    ${COMPILER} "${SRC_FILE}" 2>&1 | diff - "${RESULT_FILE}" || exit 1
done || (echo "FAIL: Errors" && exit 1)

echo -e "DONE: regions safety error testing\n"