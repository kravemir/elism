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

ERROR_REGIONS="
fn main() -> i32 {
    var a1 = [0;10];
    on TMP = NewRegion() {
        var b1 = [0;10];
        a1 = b1;
    }
    return 0;
}"

testRegions() {
    TMP=$(mktemp)
    TMP_O=$(mktemp)

    echo "${ERROR_REGIONS}" >TMP
    build/bp_jit -o TMP_O TMP > /dev/null
    assertEquals 121 $?
}

# https://github.com/kward/shunit2#quickstart
. shunit2
