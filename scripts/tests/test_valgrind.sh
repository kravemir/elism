#!/bin/bash
set -e

pushd `dirname $0` > /dev/null
cd ../..
ROOT_PATH=`pwd -P`
popd > /dev/null

sh -c "cd ${ROOT_PATH}/examples; make programs"

find ${ROOT_PATH}/examples/build/ -type f -regex ".*/[0-9][-0-9a-zA-Z]*" | sort | while read SRC_FILE;
do
    echo "Testing with valgrind: ${SRC_FILE}"
    valgrind --error-exitcode=133 --log-file="${SRC_FILE}.log" "${SRC_FILE}" > /dev/null
done;
