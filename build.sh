#!/usr/bin/env bash

set -euo pipefail

WORK_DIR=`pwd`/`dirname $0`
IMAGE=emscripten/emsdk:3.1.13

mkdir -p "$WORK_DIR/build"

function run_emcc() {
    docker run \
    --rm \
    -v $WORK_DIR:$WORK_DIR \
    -v "$WORK_DIR"/.emscripten_cache:/emsdk/upstream/emscripten/cache \
    -w "$WORK_DIR/build" \
    $IMAGE \
    emcc \
        -fno-rtti \
        -fno-exceptions \
        -flto \
        -Wall \
        --bind \
        -O3 \
        --profiling \
        -g3 \
        -gsource-map \
        --source-map-base "./" \
        -DEMSCRIPTEN_HAS_UNBOUND_TYPE_NAMES=0 `# allows embind to work with rtti disabled` \
        -msimd128 \
        -s WASM_BIGINT `# skip link-time work to legalize i64 values on the JS/WASM boundary` \
        -s TOTAL_STACK=1048576 `# use a 1MB stack size instead of the default 5MB` \
        -s INITIAL_MEMORY=2097152 `# start with a 2MB allocation instead of 16MB, we will dynamically grow` \
        -s ALLOW_MEMORY_GROWTH=1 \
        -s DEMANGLE_SUPPORT=1 \
        -s "EXPORTED_FUNCTIONS=['_malloc', '_free']" \
        -s "EXPORTED_RUNTIME_METHODS=['ccall', 'cwrap']" \
        -s FILESYSTEM=0 `# we don't need filesystem support. This should reduce file sizes` \
        -s MODULARIZE=1 \
        --pre-js "$WORK_DIR/src/pre.js" \
        -s WARN_UNALIGNED=1 \
        -s WASM=1 \
        -s DISABLE_EXCEPTION_CATCHING=1 \
        -I"$WORK_DIR/build" `# for bz_version.h` \
        "$@"
}

run_emcc \
    -std=c++17 \
    "$WORK_DIR/cpp/edges.cpp" "$WORK_DIR/cpp/module.cpp" \
    -o "$WORK_DIR"/wasm/module.js
