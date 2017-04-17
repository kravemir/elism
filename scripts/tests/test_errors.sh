#!/bin/bash

ERROR_STR_TO_INT="
fn main() -> i32 {
    var i = 0;
    i = \"AAAA\";
    return 0;
}"

testType() {
    TMP=$(mktemp)
    TMP_O=$(mktemp)

    echo "${ERROR_STR_TO_INT}" >TMP
    build/bp_jit -o TMP_O TMP > /dev/null 2> /dev/null
    assertEquals 121 $?
}

# https://github.com/kward/shunit2#quickstart
. shunit2
