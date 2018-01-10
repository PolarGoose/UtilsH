#!/bin/bash
set -e

build () {
    echo "====== Build: type=$1 enable_clang_tidy=$2"
    cmake -DCMAKE_BUILD_TYPE=$1 -Denable_clang_tidy=$2 -GNinja ../project
    cmake --build . --target run_tests
    rm -rf *
    echo "====== Finished: $1 build"
}

rm -rf build
mkdir build
cd build

build Debug ON
build Release OFF
