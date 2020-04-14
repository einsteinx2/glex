#!/bin/bash

# Clean
rm -rf ./build/mac

# Build using max available threads
export THREADS=$(getconf _NPROCESSORS_ONLN) &&
mkdir -p ./build/mac && cd ./build/mac && 
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../../cmake/toolchains/MacOS.cmake ../../ && 
make VERBOSE=ON -j$THREADS

