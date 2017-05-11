#!/bin/bash

: ${COMPILER:=./build/elism}

cd "$(dirname $0)" > /dev/null
SCRIPT_PATH=`pwd -P`
cd - > /dev/null

printf "\nBEGIN: basic error testing\n"

find ${SCRIPT_PATH}/error_inputs -type f -name "*.bp.expect" | sort | while read RESULT_FILE;
do
    SRC_FILE=${RESULT_FILE%.expect}
    printf "Testing error handling: ${SRC_FILE}\n"
    ${COMPILER} "${SRC_FILE}" 2>&1 | diff - "${RESULT_FILE}" || exit 1
done || (printf "FAIL: Errors" && exit 1)

printf "DONE: basic error testing\n"

printf "\nBEGIN: basic error testing\n"

find ${SCRIPT_PATH}/error_regions -type f -name "*.bp.expect" | sort | while read RESULT_FILE;
do
    SRC_FILE=${RESULT_FILE%.expect}
    printf "Testing error handling: ${SRC_FILE}\n"
    ${COMPILER} "${SRC_FILE}" 2>&1 | diff - "${RESULT_FILE}" || exit 1
done || (printf "FAIL: Errors" && exit 1)

printf "DONE: regions safety error testing\n"