#!/bin/bash

# Clean
#rm -rf ./build/dc

# Build using max available threads
#export THREADS=$(getconf _NPROCESSORS_ONLN) &&
export THREADS=1 &&
mkdir -p ./build/dc && cd ./build/dc &&
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../../cmake/toolchains/Dreamcast.cmake ../../ &&
make VERBOSE=ON -j$THREADS

