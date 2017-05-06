#!/usr/bin/env bash

cloc "$TRAVIS_BUILD_DIR/src"
sh -c 'cd "$TRAVIS_BUILD_DIR/src" wc -l $(find examples/ -name '*.bp' | sort)'
sh -c 'cd "$TRAVIS_BUILD_DIR/src" wc -l $(find scripts/tests/ -name '*.bp' | sort)'
