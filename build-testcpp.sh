#!/usr/bin/env bash

set -euo pipefail

WORK_DIR=`pwd`/`dirname $0`

mkdir -p "$WORK_DIR/build"
clang++ -std=c++17 -g -o "$WORK_DIR/build/test" "$WORK_DIR/cpp/edges.cpp" "$WORK_DIR/cpp/test.cpp"
