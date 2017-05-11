#!/bin/bash

cd "$(dirname $0)"
SCRIPT_PATH=`pwd -P`
cd - > /dev/null

sh ${SCRIPT_PATH}/test_jit.sh
sh ${SCRIPT_PATH}/test_errors.sh