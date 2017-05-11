#!/bin/bash

pushd `dirname $0` > /dev/null
SCRIPT_PATH=`pwd -P`
popd > /dev/null

bash ${SCRIPT_PATH}/test_jit.sh
bash ${SCRIPT_PATH}/test_errors.sh